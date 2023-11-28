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
    glm::vec3 maxCorner = data.vertices[0].position;
    glm::vec3 minCorner = data.vertices[0].position;

    for (int i = 1; i < data.vertices.size(); ++i)
    {
        glm::vec3 vertex_pos = data.vertices[i].position;
        if (vertex_pos.x > maxCorner.x)
            maxCorner.x = vertex_pos.x;
        else if (vertex_pos.x < minCorner.x)
            minCorner.x = vertex_pos.x;

        if (vertex_pos.y > maxCorner.y)
            maxCorner.y = vertex_pos.y;
        else if (vertex_pos.y < minCorner.y)
            minCorner.y = vertex_pos.y;

        if (vertex_pos.z > maxCorner.z)
            maxCorner.z = vertex_pos.z;
        else if (vertex_pos.z < minCorner.z)
            minCorner.z = vertex_pos.z;
    }

    glm::vec3 center = (maxCorner + minCorner) / 2.0f;
    float radius = std::max({abs(maxCorner.x - center.x), abs(maxCorner.y - center.y), abs(maxCorner.z - center.z)});

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

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    if(audit_bindings_before_draw) {
        audit_bindings();
    }

    // Test the bounding box with the frustum before drawing
    glDrawElements(GL_TRIANGLES, int(_index_buffer.element_count()), GL_UNSIGNED_INT, nullptr);
}

}
