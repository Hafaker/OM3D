#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

namespace OM3D {

SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material) :
    _mesh(std::move(mesh)),
    _material(std::move(material)) {
}

bool testPlane(std::shared_ptr<StaticMesh> mesh, glm::vec3& plane_normal)
{
    SphereBoundingBox bbox = mesh->_bbox;
    return glm::dot(plane_normal, bbox.center) + bbox.radius > 0;
}

void SceneObject::render(Frustum frustum) const {
    if(!_material || !_mesh) {
        return;
    }

    _material->set_uniform(HASH("model"), transform());
    _material->bind();

    if (testPlane(_mesh, frustum._near_normal) &&
        testPlane(_mesh, frustum._top_normal) &&
        testPlane(_mesh, frustum._bottom_normal) &&
        testPlane(_mesh, frustum._right_normal) &&
        testPlane(_mesh, frustum._left_normal)) {
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
