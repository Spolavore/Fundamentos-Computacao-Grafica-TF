#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED


#include "SceneObject.h"

void verifyCratesCollisions(bool user_can_move[], glm::vec4 camera_c_position,
                            glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene,
                            std::vector<glm::vec3> translation_models, bool *pode_pular,  std::vector<glm::vec3> rotation_models);



void verifyPlataformCollisions(glm::vec4 camera_c_position,  glm::vec4 camera_view_vector,
                                std::map<std::string, SceneObject>& g_VirtualScene,
                                std::vector<glm::vec3> translation_models, bool user_can_move_in_platforms[]);

void verifyIfHittedCow(glm::vec4 *camera_c_position, std::vector<glm::vec3> translation_models);

void verifyFalling(glm::vec4* camera_c_position, float delta_t,bool *usuario_esta_caindo,  bool *pode_pular, bool *usuario_esta_pulando);

#endif // COLLISIONS_H_INCLUDED
