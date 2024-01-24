#include "StaticMesh.h"
#include "Camera.h"

#include <glad/glad.h>

namespace OM3D {

extern bool audit_bindings_before_draw;

StaticMesh::StaticMesh(const MeshData& data) :
    _vertex_buffer(data.vertices),
    _index_buffer(data.indices) {


    // Create a bounding sphere

    // Search the center of the sphere
    glm::vec3 sum = glm::vec3(0.0);
    for (auto i : data.vertices) {
        sum += i.position;
    }
    glm::vec3 center = sum / (float)data.vertices.size();
    float max = 0.0;
    for (auto vert : data.vertices) {
        float n_dist = glm::distance(vert.position, center);
        max = std::max(n_dist, max);
    }
    float radius = max;
    _bbox.center = center;
    _bbox.radius = radius;

}

void StaticMesh::draw() const {
    _vertex_buffer.bind(BufferUsage::Attribute);
    _index_buffer.bind(BufferUsage::Index);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
    // Vertex normal
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(3 * sizeof(float)));
    // Vertex uv
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(6 * sizeof(float)));
    // Tangent / bitangent sign
    glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(8 * sizeof(float)));
    // Vertex color
    glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(12 * sizeof(float)));

    // Joint
    glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(15 * sizeof(float)));
    // Weight
    glVertexAttribPointer(6, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(19 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    if(audit_bindings_before_draw) {
        audit_bindings();
    }

    glDrawElements(GL_TRIANGLES, int(_index_buffer.element_count()), GL_UNSIGNED_INT, nullptr);
}

}
