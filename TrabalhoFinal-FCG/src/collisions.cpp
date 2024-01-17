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
void verifyChestCollisions(bool user_can_move[], glm::vec4 camera_c_position, glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene){

    glm::vec3 bbox_min_create =  g_VirtualScene["Crate_Plane.005"].bbox_min;
    glm::vec3 bbox_max_create =  g_VirtualScene["Crate_Plane.005"].bbox_max;

    // Expande levemente a bbox a fim de evitar bordas sensíves, impedindo que o usuário entre parcialmente
    // no objeto
    bbox_min_create = glm::vec3(bbox_min_create.x*1.5, bbox_min_create.y, bbox_min_create.z*1.5);
    bbox_max_create = glm::vec3(bbox_max_create.x*1.5, bbox_max_create.y, bbox_max_create.z*1.5);

    // Pega a reta para onde o usuario está indo
    camera_view_vector = glm::vec4(camera_view_vector.x, 0.0f, camera_view_vector.z, 0.0f);
    glm::vec3 camera_view_vector_withou_w = glm::vec3(camera_view_vector.x, 0.0f, camera_view_vector.z);

    // camera_right é o vetor lateral a camera_c_point e do view_vect
    // FONTE: Adaptação do código do ChatGPT
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_view_vector_withou_w, glm::vec3(0.0f, 1.0f, 0.0f)));
    // Aplica a coordenada w na camera_right
    glm::vec4 camera_right_4_coord = glm::vec4(camera_right.x*0.2, camera_right.y*0.2, camera_right.z*0.2, 0.0);


    glm::vec4 foward_point_predicted = camera_c_position + (camera_view_vector/(norma(camera_view_vector)));
    glm::vec4 left_point_predicted = camera_c_position + (-camera_right_4_coord);
    glm::vec4 backwards_point_predicted = camera_c_position + -(camera_view_vector/(norma(camera_view_vector)));
    glm::vec4 right_point_predicted = camera_c_position + (camera_right_4_coord);

    if(compareToBBox(foward_point_predicted, bbox_min_create, bbox_max_create)){
        user_can_move[0] = false;

       } else{
        user_can_move[0] = true;
       }
    if(compareToBBox(left_point_predicted, bbox_min_create, bbox_max_create)){
        user_can_move[1] = false;
    } else{
        user_can_move[1] = true;
    }

    if(compareToBBox(backwards_point_predicted, bbox_min_create, bbox_max_create)){
        user_can_move[2] = false;
    } else {
        user_can_move[2] = true;
    }
    if(compareToBBox(right_point_predicted, bbox_min_create, bbox_max_create)){
        user_can_move[3] = false;
    } else{
        user_can_move[3] =true;
    }


}

