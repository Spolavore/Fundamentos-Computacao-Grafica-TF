#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo s�o espec�ficos de C++
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Cria��o de janelas do sistema operacional

// Headers da biblioteca GLM: cria��o de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>
#include "SceneObject.h"
#define distancia_de_contato_caixa 15 // quanto maior essa constante mais perto da caixa a camera poder� chegar
bool isFallingCrate = false; // verifica se abaixo do player est� presente uma caixa (usado tambem para plataformas)
bool applyInstantFall = false; // controla se o jogador deve receber queda instant�nea ( �til para situa��o como contato superior com objeto)
                               // * bater a "cabeca" em algo *
float gravityAceleration = 1.1f;
#define CRATE 0
#define PLATAFORM 1

float norma(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt( vx*vx + vy*vy + vz*vz )*distancia_de_contato_caixa;
}

// Funcao que compara com a bbox do objeto e retorna true se o ponto previsto estar� em contato
// com a caixa e falso se ele n�o estar� em contato com a caixa
bool compareToBBox(glm::vec4 point_predicted, glm::vec3 bbox_min, glm::vec3 bbox_max){
     if(point_predicted.x >= bbox_min.x && point_predicted.x <= bbox_max.x &&
       point_predicted.y >= bbox_min.y && point_predicted.y <= bbox_max.y &&
       point_predicted.z >= bbox_min.z && point_predicted.z <= bbox_max.z
       ){
            return true;
        } else{
            return false;
        }

}

// Fun��o que verifica se ocorreu coliss�es com qualquer uma das caixas (objs) do jogo
void verifyCratesCollisions(bool user_can_move[], glm::vec4 camera_c_position,
                            glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene,
                            std::vector<glm::vec3> translation_models, bool *pode_pular, std::vector<glm::vec3> dinamic_crate_translation_model){


    // Pega a reta para onde o usuario est� indo
    camera_view_vector = glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f);
    glm::vec3 camera_view_vector_withou_w = glm::vec3(camera_view_vector.x, 0.0f, camera_view_vector.z);

    // camera_right � o vetor lateral a camera_c_point e do view_vect

    glm::vec3 camera_right = glm::normalize(glm::cross(camera_view_vector_withou_w, glm::vec3(0.0f, 1.0f, 0.0f)));
    // Aplica a coordenada w na camera_right
    glm::vec4 camera_right_4_coord = glm::vec4(camera_right.x*0.1, camera_right.y*0.1, camera_right.z*0.1, 0.0);


    // ponto de compara��o frontal
    glm::vec4 foward_comparation = camera_view_vector/(norma(camera_view_vector));
    foward_comparation = glm::vec4(foward_comparation.x, 0.0f, foward_comparation.z, 0.0f);

    // Pontos da camera previstos pela movimentacao do usu�rio
    glm::vec4 foward_point_predicted = camera_c_position + foward_comparation;
    glm::vec4 left_point_predicted = camera_c_position + (-camera_right_4_coord);
    glm::vec4 backwards_point_predicted = camera_c_position + -(camera_view_vector/(norma(camera_view_vector)));
    glm::vec4 right_point_predicted = camera_c_position + (camera_right_4_coord);
    glm::vec4 down_point_predicted =  camera_c_position - glm::vec4(0.0f, 0.2f, 0.0f, 0.0f);
    glm::vec4 up_point_predicted = camera_c_position + glm ::vec4(0.0f, 0.2f, 0.0f, 0.0f);




    glm::vec3 bbox_min_create;
    glm::vec3 bbox_max_create;


    bbox_min_create =  g_VirtualScene["Crate_Plane.005"].bbox_min;
    bbox_max_create =  g_VirtualScene["Crate_Plane.005"].bbox_max;

    // Aplica a Matriz de Escalamento no bbox
    bbox_min_create *= glm::vec3(0.1f,0.1f,0.1f);
    bbox_max_create *= glm::vec3(0.1f,0.1f,0.1f);



    // Expande levemente a bbox a fim de evitar bordas sens�ves, impedindo que o usu�rio entre parcialmente
    // no objeto
    bbox_min_create = glm::vec3(bbox_min_create.x*1.5 , bbox_min_create.y*1.5, bbox_min_create.z*1.5 );
    bbox_max_create = glm::vec3(bbox_max_create.x*1.5 , bbox_max_create.y*1.5, bbox_max_create.z*1.5);


    // Pega a primeira bbox da primeira create renderizada, eh nas vari�veis abaixo que ser�o
    // somadas os pesos de transla��o, visto que ao desenharmo o obj sempre aplicamos com base
    // na origem
    glm::vec3 origin_bbox_min_create = glm::vec3(bbox_min_create.x,bbox_min_create.y,bbox_min_create.z);
    glm::vec3 origin_bbox_max_create = glm::vec3(bbox_max_create.x,bbox_max_create.y,bbox_max_create.z);

    // assume que nada esta acontecendo no usuario, ent�o as variaveis est�o funcionando normalmente
    applyInstantFall = false;
    bool can_jump = *pode_pular;
    bool can_move_straight = true;
    bool can_move_left = true;
    bool can_move_right = true;
    bool can_move_behind = true;
    bool hittedBoxOnFall = false;
    if(translation_models.size() != 0){
        for(int i = 0; i < translation_models.size() + 1; i++){

            // Caso todas as caixas nomais j� tenham sido verificadas, verifica a caixa que o usu�rio pode mover
            if(i >= translation_models.size()){
                bbox_min_create = origin_bbox_min_create + dinamic_crate_translation_model[0];
                bbox_max_create = origin_bbox_max_create + dinamic_crate_translation_model[0];
            } else {
                // Aplica as matrizes de transla��o no objeto
                bbox_min_create = origin_bbox_min_create + translation_models[i];
                bbox_max_create = origin_bbox_max_create + translation_models[i];
            }


            // Aplica as matrizes de rota��o
            bbox_min_create *= glm::vec3(1.0f, 1.0f, 1.0f);
            bbox_max_create *= glm::vec3(1.0f, 1.0f, 1.0f);



            // Se em qualquer uma das caixas de false ent�o impossibilita a movimenta��o
            // para aquela dire��o
            if(compareToBBox(foward_point_predicted, bbox_min_create, bbox_max_create)){
                can_move_straight = false;

               }
            if(compareToBBox(left_point_predicted, bbox_min_create, bbox_max_create)){
                can_move_left = false;

            }

            if(compareToBBox(backwards_point_predicted, bbox_min_create, bbox_max_create)){
                can_move_behind = false;
            }

            if(compareToBBox(right_point_predicted, bbox_min_create, bbox_max_create)){
                can_move_right = false;
            }

            if(compareToBBox(down_point_predicted, bbox_min_create, bbox_max_create)){
                hittedBoxOnFall = true;

            }
            if(compareToBBox(up_point_predicted, bbox_min_create, bbox_max_create)){
                can_jump = false;
                applyInstantFall = true;
                hittedBoxOnFall = false; // como ser� negado abaixo isso ir� para true


            }

        }

        // Muda os Status da Vari�vel
        user_can_move[0] = can_move_straight;
        user_can_move[1] = can_move_left;
        user_can_move[2] = can_move_behind;
        user_can_move[3] = can_move_right;
        *pode_pular = can_jump;
        isFallingCrate = !hittedBoxOnFall;


    }

}



// Fun��o que verifica se o player est� caindo, se ele estiver vai reduzindo o y da camera_c_position
// ate chegar em contato com algo ou com o ponto de origem y = 0.5
void verifyFalling(glm::vec4 *camera_c_position, float delta_t, bool *usuario_esta_caindo, bool *pode_pular, bool *usuario_esta_pulando){
    float fallingSpeed = 8.0;


    // se a posi��o y da camera for maior do que 0.5 ( ela nao esta no chao ) ou est� caindo


    // Caso o usu�rio bata a cabe�a em um objeto ele cai instantaneamente
    if(camera_c_position->y > 0.5 && isFallingCrate && applyInstantFall){
        *usuario_esta_pulando = false;
        *pode_pular = false;
        *camera_c_position -= glm::vec4(0.0f, 0.2f, 0.0f, 0.0f) * fallingSpeed * delta_t * gravityAceleration;
        gravityAceleration += 0.025f;
    }else if(camera_c_position->y > 0.5 && isFallingCrate && !*usuario_esta_pulando){
        *pode_pular = false;
        *camera_c_position -= glm::vec4(0.0f, 0.2f, 0.0f, 0.0f) * fallingSpeed * delta_t * gravityAceleration;;
        gravityAceleration += 0.025f;
    } else {
        gravityAceleration = 1.0f;
        *usuario_esta_caindo = false;
        *pode_pular = true;
    }


    // Se o valor y da camera for menor do 0.5 ent�o for�a ele para 0.5 para garantir a permanencia
    // do funcionamento do contato com as caixas. Ainda, se y for menor do que 0.5, significa que ele
    // ta no "solo" ou seja a altura padrao da camera DEVE ser 0.5
    if(camera_c_position->y < 0.5){
        *camera_c_position = glm::vec4(camera_c_position->x, 0.5f, camera_c_position->z, 1.0f);
    }

}


// Testa se o usu�rio est� em cima de uma plataforma, como s� importa saber se quando o usu�rio
// est� caindo e bate numa plataforma, ent�o realiza apenas o teste de quedra( down_predicted_point)
void verifyPlataformCollisions(glm::vec4 camera_c_position,  glm::vec4 camera_view_vector,
                                std::map<std::string, SceneObject>& g_VirtualScene,
                                std::vector<glm::vec3> translation_models, bool user_can_move_in_platforms[]){


    glm::vec3 bbox_min_create =  g_VirtualScene["cartoon wooden floor"].bbox_min;
    glm::vec3 bbox_max_create =  g_VirtualScene["cartoon wooden floor"].bbox_max;

    // Aplica a matriz de escalamento nas bbox
    bbox_min_create *= glm::vec3(0.1f,0.1f,0.1f);
    bbox_max_create *= glm::vec3(0.1f,0.1f,0.1f);

    // Expande levemente a bbox a fim de evitar bordas sens�ves, impedindo que o usu�rio entre parcialmente
    // no objeto
    bbox_min_create = glm::vec3(bbox_min_create.x*1.5 , bbox_min_create.y*1, bbox_min_create.z*1.5);
    bbox_max_create = glm::vec3(bbox_max_create.x*1.5 , bbox_max_create.y*1, bbox_max_create.z*1.5);


    // ponto de compara��o frontal
    glm::vec4 foward_comparation = camera_view_vector/(norma(camera_view_vector));
    foward_comparation = glm::vec4(foward_comparation.x, 0.0f, foward_comparation.z, 0.0f);

    glm::vec4 down_predicted_vector = camera_c_position - glm::vec4(0.0f, 0.3f, 0.0f, 0.0f);
    glm::vec4 foward_point_predicted = camera_c_position + foward_comparation;

    glm::vec3 origin_bbox_min_create = glm::vec3(bbox_min_create.x,bbox_min_create.y,bbox_min_create.z);
    glm::vec3 origin_bbox_max_create = glm::vec3(bbox_max_create.x,bbox_max_create.y,bbox_max_create.z);



    bool hittedInFront = false;
    if(translation_models.size() != 0){
        for(int i = 0; i < translation_models.size(); i++){
            // Aplica as matrizes de transla��o no objeto
            bbox_min_create = origin_bbox_min_create + translation_models[i];
            bbox_max_create = origin_bbox_max_create + translation_models[i];



            if(compareToBBox(down_predicted_vector, bbox_min_create, bbox_max_create)){
                isFallingCrate = false;
            }

            if(compareToBBox(foward_point_predicted,bbox_min_create, bbox_max_create)){
                hittedInFront = true;
            }
        }
    }

    user_can_move_in_platforms[0] = !hittedInFront;
}


// Verifica se o usu�rio entrou em contato com a caixa
void verifyIfHittedCow(glm::vec4 *camera_c_position, std::vector<glm::vec3> translation_models){
    // Aqui pegamos a posi��o 0 na matriz de transla��o pois s� h� UMA vaca sendo renderizada,
    // OU SEJA, caso queira-se mudar para haver duas vacas no jogo o c�digo abaixo funcionar�
    // apenas para a primeira
    if(translation_models.size() != 0){
        glm::vec3 cow_vector_position = translation_models[0];
        float x_center_circle = cow_vector_position.x;
        float y_center_circle = cow_vector_position.y;
        float z_center_circle = cow_vector_position.z;



    // Raio do Circulo
    float circle_r = 0.7f;


    // distancia entre o ponto e o centro do circulo
    float distance_camera_circle = std::sqrt( std::pow(camera_c_position->x - x_center_circle, 2) +
                                              std::pow(camera_c_position->y - y_center_circle, 2) +
                                              std::pow(camera_c_position->z - z_center_circle, 2));
    // se a distancia entre o ponto entre a camera e o circulo for menor que o raio ent�o reinicia
    // a posi��o do jogador para a posi��o inicial
    if(distance_camera_circle <= circle_r){
        *camera_c_position = glm::vec4(0.0f, 0.5f,-0.5f,1.0f);
    }

  }
}








