#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED


#include "SceneObject.h"

void verifyCratesCollisions(bool user_can_move[], glm::vec4 camera_c_position,
                            glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene,
                            std::vector<glm::vec3> crates_translation_models);
void verifyFalling(glm::vec4* camera_c_position, float delta_t,bool *usuario_esta_caindo);
#endif // COLLISIONS_H_INCLUDED
