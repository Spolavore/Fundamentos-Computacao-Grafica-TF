#ifndef COLLISIONS_H_INCLUDED
#define COLLISIONS_H_INCLUDED


#include "SceneObject.h"

void verifyChestCollisions(bool user_can_move[], glm::vec4 camera_c_position,glm::vec4 camera_view_vector, std::map<std::string, SceneObject>& g_VirtualScene);
#endif // COLLISIONS_H_INCLUDED
