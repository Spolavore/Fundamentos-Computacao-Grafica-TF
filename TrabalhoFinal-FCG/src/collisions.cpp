#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
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
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include <stb_image.h>
#include "SceneObject.h"
#define distancia_de_contato_caixa 15 // quanto maior essa constante mais perto da caixa a camera poderá chegar
bool isFallingCrate = false; // verifica se abaixo do player está presente uma caixa

bool applyInstantFall = false; // controla se o jogador deve receber queda instantânea ( útil para situação como contato superior com objeto)
                               // * bater a "cabeca" em algo *


float norma(glm::vec4 v)
{
    float vx = v.x;
    float vy = v.y;
    float vz = v.z;

    return sqrt( vx*vx + vy*vy + vz*vz )*distancia_de_contato_caixa;
}

// Funcao que compara com a bbox do objeto e retorna true se o ponto previsto estará em contato
// com a caixa e falso se ele não estará em contato com a caixa
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

// Função que verifica se ocorreu colissões com qualquer uma das caixas (objs) do jogo
void verifyCratesCollisions(bool user_can_move[], glm::vec4 camera_c_position,
                            glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene,
                            std::vector<glm::vec3> crates_translation_models, bool *pode_pular){


    // Pega a reta para onde o usuario está indo
    camera_view_vector = glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f);
    glm::vec3 camera_view_vector_withou_w = glm::vec3(camera_view_vector.x, 0.0f, camera_view_vector.z);

    // camera_right é o vetor lateral a camera_c_point e do view_vect

    glm::vec3 camera_right = glm::normalize(glm::cross(camera_view_vector_withou_w, glm::vec3(0.0f, 1.0f, 0.0f)));
    // Aplica a coordenada w na camera_right
    glm::vec4 camera_right_4_coord = glm::vec4(camera_right.x*0.1, camera_right.y*0.1, camera_right.z*0.1, 0.0);


    // ponto de comparação frontal
    glm::vec4 foward_comparation = camera_view_vector/(norma(camera_view_vector));
    foward_comparation = glm::vec4(foward_comparation.x, 0.0f, foward_comparation.z, 0.0f);

    // Pontos da camera previstos pela movimentacao do usuário
    glm::vec4 foward_point_predicted = camera_c_position + foward_comparation;
    glm::vec4 left_point_predicted = camera_c_position + (-camera_right_4_coord);
    glm::vec4 backwards_point_predicted = camera_c_position + -(camera_view_vector/(norma(camera_view_vector)));
    glm::vec4 right_point_predicted = camera_c_position + (camera_right_4_coord);
    glm::vec4 down_point_predicted =  camera_c_position - glm::vec4(0.0f, 0.2f, 0.0f, 0.0f);
    glm::vec4 up_point_predicted = camera_c_position + glm ::vec4(0.0f, 0.2f, 0.0f, 0.0f);


    glm::vec3 bbox_min_create =  g_VirtualScene["Crate_Plane.005"].bbox_min;
    glm::vec3 bbox_max_create =  g_VirtualScene["Crate_Plane.005"].bbox_max;

    // Aplica a Matriz de Escalamento no bbox
    bbox_min_create *= glm::vec3(0.1f,0.1f,0.1f);
    bbox_max_create *= glm::vec3(0.1f,0.1f,0.1f);


    // Expande levemente a bbox a fim de evitar bordas sensíves, impedindo que o usuário entre parcialmente
    // no objeto
    bbox_min_create = glm::vec3(bbox_min_create.x*1.5 , bbox_min_create.y*1.5, bbox_min_create.z*1.5 );
    bbox_max_create = glm::vec3(bbox_max_create.x*1.5 , bbox_max_create.y*1.5, bbox_max_create.z*1.5);


    // Pega a primeira bbox da primeira create renderizada, eh nas variáveis abaixo que serão
    // somadas os pesos de translação, visto que ao desenharmo o obj sempre aplicamos com base
    // na origem
    glm::vec3 origin_bbox_min_create = glm::vec3(bbox_min_create.x,bbox_min_create.y,bbox_min_create.z);
    glm::vec3 origin_bbox_max_create = glm::vec3(bbox_max_create.x,bbox_max_create.y,bbox_max_create.z);

    // assume que nada esta acontecendo no usuario, então as variaveis estão funcionando normalmente
    applyInstantFall = false;
    bool can_jump = *pode_pular;
    bool can_move_straight = true;
    bool can_move_left = true;
    bool can_move_right = true;
    bool can_move_behind = true;
    bool hittedBoxOnFall = false;
    if(crates_translation_models.size() != 0){
        for(int i = 0; i < crates_translation_models.size(); i++){

            // Aplica as matrizes de translação no objeto
            bbox_min_create = origin_bbox_min_create + crates_translation_models[i];
            bbox_max_create = origin_bbox_max_create + crates_translation_models[i];



            // Se em qualquer uma das caixas de false então impossibilita a movimentação
            // para aquela direção
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
                hittedBoxOnFall = false; // como será negado abaixo isso irá para true


            }

        }

        // Muda os Status da Variável
        user_can_move[0] = can_move_straight;
        user_can_move[1] = can_move_left;
        user_can_move[2] = can_move_behind;
        user_can_move[3] = can_move_right;
        *pode_pular = can_jump;
        isFallingCrate = !hittedBoxOnFall;
    }

}



// Função que verifica se o player está caindo, se ele estiver vai reduzindo o y da camera_c_position
// ate chegar em contato com algo ou com o ponto de origem y = 0.5
void verifyFalling(glm::vec4 *camera_c_position, float delta_t, bool *usuario_esta_caindo, bool *pode_pular, bool *usuario_esta_pulando){
    float fallingSpeed = 8.0;


    // se a posição y da camera for maior do que 0.5 ( ela nao esta no chao ) ou está caindo
    if(camera_c_position->y > 0.5 && isFallingCrate && applyInstantFall){
        *usuario_esta_pulando = false;
        *pode_pular = false;
        *camera_c_position -= glm::vec4(0.0f, 0.2f, 0.0f, 0.0f) * fallingSpeed * delta_t;
    }else if(camera_c_position->y > 0.5 && isFallingCrate && !*usuario_esta_pulando){
        *pode_pular = false;
        *camera_c_position -= glm::vec4(0.0f, 0.2f, 0.0f, 0.0f) * fallingSpeed * delta_t;

    } else {
        *usuario_esta_caindo = false;
        *pode_pular = true;
    }


    // Se o valor y da camera for menor do 0.5 então força ele para 0.5 para garantir a permanencia
    // do funcionamento do contato com as caixas. Ainda, se y for menor do que 0.5, significa que ele
    // ta no "solo" ou seja a altura padrao da camera DEVE ser 0.5
    if(camera_c_position->y < 0.5){
        *camera_c_position = glm::vec4(camera_c_position->x, 0.5f, camera_c_position->z, 1.0f);
    }

}
