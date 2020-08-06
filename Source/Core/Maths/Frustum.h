#pragma once

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Minecraft
{
    struct FrustumAABB 
    {
        FrustumAABB(const glm::vec3& dimensions, const glm::vec3& position)
            : p_Dimensions(dimensions), p_Position(position)
        {

        }

        glm::vec3 GetVN(const glm::vec3& normal) const
        {
            glm::vec3 res = p_Position;

            if (normal.x < 0) 
            {
                res.x += p_Dimensions.x;
            }

            if (normal.y < 0) 
            {
                res.y += p_Dimensions.y;
            }

            if (normal.z < 0) 
            {
                res.z += p_Dimensions.z;
            }

            return res;
        }

        glm::vec3 GetVP(const glm::vec3& normal) const
        {
            glm::vec3 res = p_Position;

            if (normal.x > 0) 
            {
                res.x += p_Dimensions.x;
            }

            if (normal.y > 0) 
            {
                res.y += p_Dimensions.y;
            }
            if (normal.z > 0) 
            {
                res.z += p_Dimensions.z;
            }

            return res;
        }

        const glm::vec3 p_Dimensions;
        const glm::vec3 p_Position;
    };

    struct Plane 
    {
        float distanceToPoint(const glm::vec3& point) const;

        float distanceToOrigin;
        glm::vec3 normal;
    };

    class ViewFrustum
    {
    public:
        void Update(const glm::mat4& VP_Matrix);
        bool BoxInFrustum(const FrustumAABB& box) const;

    private:
        std::array<Plane, 6> m_Planes;
    };
}