#pragma once

// include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

namespace Minecraft
{
    std::vector<glm::vec3> FastVoxelTraversal(glm::vec3 ray_start, glm::vec3 ray_end);
}