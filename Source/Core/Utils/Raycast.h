#pragma once

// include glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

namespace Minecraft
{
    void FastVoxelTraversal(glm::vec3 ray_start, glm::vec3 ray_end, std::vector<glm::vec3>& visited_voxels);
}