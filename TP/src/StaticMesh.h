#ifndef STATICMESH_H
#define STATICMESH_H

#include <graphics.h>
#include <TypedBuffer.h>
#include <Vertex.h>

#include <vector>

namespace OM3D {

struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};

struct SphereBoundingBox {
    glm::vec3 center;
    float radius; 
};

class StaticMesh : NonCopyable {

    public:
        StaticMesh() = default;
        StaticMesh(StaticMesh&&) = default;
        StaticMesh& operator=(StaticMesh&&) = default;

        StaticMesh(const MeshData& data);

        float getSignedDistanceToPlane(glm::vec3& plane, const glm::vec3& point) const;
        bool isOnOrForwardPlane(glm::vec3& plane) const;
        bool frustumTest();

        void draw() const;

        SphereBoundingBox _bbox;

    private:
        TypedBuffer<Vertex> _vertex_buffer;
        TypedBuffer<u32> _index_buffer;
};

}

#endif // STATICMESH_H
