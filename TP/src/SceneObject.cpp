#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace OM3D {

SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material) :
    _mesh(std::move(mesh)),
    _material(std::move(material)) {
}


glm::mat4 quatToRotationMatrix(glm::vec4 q) {
    return glm::mat4(
        1.0 - 2.0 * (q.y * q.y + q.z * q.z), 2.0 * (q.x * q.y - q.w * q.z), 2.0 * (q.x * q.z + q.w * q.y), 0.0,
        2.0 * (q.x * q.y + q.w * q.z), 1.0 - 2.0 * (q.x * q.x + q.z * q.z), 2.0 * (q.y * q.z - q.w * q.x), 0.0,
        2.0 * (q.x * q.z - q.w * q.y), 2.0 * (q.y * q.z + q.w * q.x), 1.0 - 2.0 * (q.x * q.x + q.y * q.y), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
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

std::vector<size_t> findSurroundingTimestamps(double currentTime, std::vector<double> timestamps) {

    size_t before = 0;
    size_t after = 0;

    // Iterate through timestamps to find the surrounding
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

glm::vec3 interpolateTransformation(std::vector<std::vector<double>> transformations, std::vector<double> timestamps, std::vector<size_t> timeIndexes, double time) {
    
    double timestamp1 = timestamps[timeIndexes[0]];
    double timestamp2 = timestamps[timeIndexes[1]];
    double t = (time - timestamp1) / (timestamp2 - timestamp1);

    glm::vec3 transform1 = glm::vec3(transformations[timeIndexes[0]][0], transformations[timeIndexes[0]][1], transformations[timeIndexes[0]][2]);
    glm::vec3 transform2 = glm::vec3(transformations[timeIndexes[1]][0], transformations[timeIndexes[1]][1], transformations[timeIndexes[1]][2]);

    glm::vec3 interpolatedTranslation = transform1 + static_cast<float>(t) * (transform2 - transform1);

    return interpolatedTranslation;
}

glm::vec4 interpolateRotation(std::vector<std::vector<double>> rotations, std::vector<double> timestamps) {

    double time = fmod(program_time(), 2.0);

    std::vector<size_t> timeIndexes = findSurroundingTimestamps(time, timestamps);
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

bool testPlane(SphereBoundingBox bbox, glm::vec3 normal, glm::vec3 position) {
    return glm::dot(normal, bbox.center) > glm::dot(normal, position) - bbox.radius;
}



void SceneObject::render(Frustum frustum) const {
    if(!_material || !_mesh) {
        return;
    }

    double time = fmod(program_time(), 2.0);
    std::vector<size_t> timeIndexes = findSurroundingTimestamps(time, _timestamps);


    if (_modelMatrices.size() > 0) {
        for (int i = 0; i < _modelMatrices.size(); i++) {
            glm::mat4 u_jointMat_i = _modelMatrices[i]._mat;
            if (_modelMatrices[i]._translations.size() > 0) {
                glm::vec3 translation = interpolateTransformation(_modelMatrices[i]._translations, _timestamps, timeIndexes, time);
                u_jointMat_i = glm::translate(u_jointMat_i, translation);
            }
            if (_modelMatrices[i]._rotations.size() > 0) {
                glm::vec4 rotation = interpolateRotation(_modelMatrices[i]._rotations, _timestamps);
                u_jointMat_i = quatToRotationMatrix(rotation) * u_jointMat_i;
            }
            if (_modelMatrices[i]._scales.size() > 0) {
                glm::vec3 scale = interpolateTransformation(_modelMatrices[i]._scales, _timestamps, timeIndexes, time);
                u_jointMat_i = glm::scale(u_jointMat_i, scale);
            }

            u_jointMat_i =  u_jointMat_i * _inverseMatrices[i];
            std::string uniformName= "u_jointMat" + std::to_string(i);
            _material->set_uniform(str_hash(uniformName), u_jointMat_i);
        }
        _material->set_uniform(HASH("model"), transform());
    }
    else {
        glm::vec3 scale = glm::vec3(1.0);
        glm::vec3 translation = glm::vec3(0.0);
        glm::vec4 rotation = glm::vec4(0.0);


        if (_scales.size() > 0) {
            scale = interpolateTransformation(_scales, _timestamps, timeIndexes, time);
        }
        if (_translations.size() > 0) {
            translation = interpolateTransformation(_translations, _timestamps, timeIndexes, time);
        }
        if (_rotations.size() > 0) {
            rotation = interpolateRotation(_rotations, _timestamps);
        }
        _material->set_uniform(HASH("model"), glm::translate(glm::scale(quatToRotationMatrix(rotation) * transform(), scale), translation));
    }
    
    _material->bind();

    _mesh->draw();
    

}

void SceneObject::set_transform(const glm::mat4& tr) {
    _transform = tr;
}

const glm::mat4& SceneObject::transform() const {
    return _transform;
}

}
