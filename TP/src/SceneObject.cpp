#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>
namespace OM3D {

SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material) :
    _mesh(std::move(mesh)),
    _material(std::move(material)) {
}

bool testPlane(SphereBoundingBox bbox, glm::vec3 normal, glm::vec3 position) {
    return glm::dot(normal, bbox.center) > glm::dot(normal, position) - bbox.radius;
}


void SceneObject::render(Frustum frustum) const {
    if(!_material || !_mesh) {
        return;
    }

    _material->set_uniform(HASH("model"), transform());
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
