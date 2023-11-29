#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>

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

void SceneObject::render(Frustum frustum) const {
    if(!_material || !_mesh) {
        return;
    }

    _material->set_uniform(HASH("model"), transform());
    _material->bind();

    if (testPlane(_mesh, frustum._near_normal, frustum._point) &&
        testPlane(_mesh, frustum._top_normal, frustum._point) &&
        testPlane(_mesh, frustum._bottom_normal, frustum._point) &&
        testPlane(_mesh, frustum._right_normal, frustum._point) &&
        testPlane(_mesh, frustum._left_normal, frustum._point)
        ) {
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
