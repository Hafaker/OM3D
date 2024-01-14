#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace OM3D {

SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material) :
    _mesh(std::move(mesh)),
    _material(std::move(material)) {
}

bool testPlane(std::shared_ptr<StaticMesh> mesh, glm::vec3& plane_normal, glm::vec3 plane_position)
{
    SphereBoundingBox bbox = mesh->_bbox;

    float distance_normal = glm::length(plane_normal);
    
    glm::vec3 vect_origin(
        plane_normal.x < 0.0f ? -distance_normal : distance_normal,
        plane_normal.y < 0.0f ? -distance_normal : distance_normal,
        plane_normal.z < 0.0f ? -distance_normal : distance_normal
    );
    return dot(plane_normal, vect_origin - plane_position) > 0;
}

glm::vec4 slerp(glm::vec4 previousRotation, glm::vec4 nextRotation, double interpolationValue)
{
        double dotProduct = glm::dot(previousRotation, nextRotation);
        
        //make sure we take the shortest path in case dot Product is negative
        if(dotProduct < 0.0) {
            nextRotation = -nextRotation;
            dotProduct = -dotProduct;
        }
            

        //perform the spherical linear interpolation
        double theta_0 = acos(dotProduct);
        double theta = interpolationValue * theta_0;
        double sin_theta = sin(theta);
        double sin_theta_0 = sin(theta_0);
        
        double scalePreviousQuat = cos(theta) - dotProduct * sin_theta / sin_theta_0;
        double scaleNextQuat = sin_theta / sin_theta_0;

        return static_cast<float>(scalePreviousQuat) * previousRotation + static_cast<float>(scaleNextQuat) * nextRotation;
}

std::vector<size_t> findKeyFrames(double currentTime, std::vector<double> timestamps) {

    size_t before = 0;
    size_t after = 2;

    // Iterate through timestamps to find the keyframes
    for (size_t i = 0; i < timestamps.size() - 1; i++) {
        if (timestamps[i] <= currentTime && timestamps[i + 1] >= currentTime) {
            before = i;
            after = i + 1;
            break;
        }
    }

    std::vector<size_t> timeIndexes = {before, after};
    return timeIndexes;
}

glm::vec4 interpolateRotation(std::vector<std::vector<double>> rotations, std::vector<double> timestamps) {

    double time = fmod(program_time(), 2.0);

    std::vector<size_t> timeIndexes = findKeyFrames(time, timestamps);
    std::vector<double> previousRotationVector = rotations[timeIndexes[0]];
    glm::vec4 previousRotation = glm::vec4(previousRotationVector[0], previousRotationVector[1], previousRotationVector[2], previousRotationVector[3]);

    std::vector<double> nextRotationVector = rotations[timeIndexes[1]];
    glm::vec4 nextRotation = glm::vec4(nextRotationVector[0], nextRotationVector[1], nextRotationVector[2], nextRotationVector[3]);


    
    double previousTime = timestamps[timeIndexes[0]];
    double nextTime = timestamps[timeIndexes[1]];

    double interpolateValue = (time - previousTime) / (nextTime - previousTime);

    glm::vec4 rotation = slerp(previousRotation, nextRotation, interpolateValue);
    return rotation;

} 

void SceneObject::render(Frustum frustum) const {
    if(!_material || !_mesh) {
        return;
    }

    glm::vec4 rotation = interpolateRotation(_rotations, _timestamps);

    _material->set_uniform(HASH("rotation"), rotation);

    _material->set_uniform(HASH("model"), transform());
    _material->bind();

    /*if (testPlane(_mesh, frustum._near_normal, frustum._point) &&
        testPlane(_mesh, frustum._top_normal, frustum._point) &&
        testPlane(_mesh, frustum._bottom_normal, frustum._point) &&
        testPlane(_mesh, frustum._right_normal, frustum._point) &&
        testPlane(_mesh, frustum._left_normal, frustum._point)
        )*/ {
        _mesh->draw();
    }
    
}

void SceneObject::set_transform(const glm::mat4& tr) {
    _transform = tr;
}

const glm::mat4& SceneObject::transform() const {
    return _transform;
}

}
