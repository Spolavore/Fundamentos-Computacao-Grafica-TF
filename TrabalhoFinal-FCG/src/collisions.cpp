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
#define distancia_de_contato_caixa 3.5 // quanto maior essa constante mais perto da caixa a camera poderá chegar

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


    glm::vec4 foward_point_predicted = camera_c_position + (camera_view_vector/(norma(camera_view_vector)));
    glm::vec4 left_point_predicted =  camera_c_position + glm::vec4(-0.015f, 0.0f, 0.0f, 0.0f);
    // Abaixo convertemos o bbox global do objeto para as coordenadas
    // locais, multiplicando pela matrix de escalamento e a matrix de transformação aplicadas no objeto
    bbox_max_create *= glm::vec3(0.1f,0.1,0.1f);
    bbox_min_create *= glm::vec3(0.1f,0.1,0.1f);

      //printf("x:%f, y:%f, z:%f", bbox_min_create.x, bbox_min_create.y, bbox_min_create.z); //bbox_min
      //printf("x:%f, y:%f, z:%f", bbox_max_create.x, bbox_max_create.y, bbox_max_create.z);// bbox_max
      //printf("x:%f, y:%f, z:%f", camera_c_position.x, camera_c_position.y, camera_c_position.z); //CAMERA
      //printf("x:%f, y:%f, z:%f", point_predicted.x, point_predicted.y, point_predicted.z);//point predicted

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

}

