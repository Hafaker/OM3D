#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <StaticMesh.h>
#include <Material.h>

#include "Camera.h"

#include <memory>

#include <glm/matrix.hpp>

namespace OM3D {

class modelMatrix {
    public:
        
        modelMatrix(glm::mat4 mat, int node_i) : 
            _mat(mat),
            _node(node_i) {
        }
        int _node;
        glm::mat4 _mat;
        std::vector<std::vector<double>> _translations;
        std::vector<std::vector<double>> _rotations;
        std::vector<std::vector<double>> _scales; 
};

class SceneObject {

    public:
        SceneObject(std::shared_ptr<StaticMesh> mesh = nullptr, std::shared_ptr<Material> material = nullptr);

        void render(Frustum frustum) const;

        void set_transform(const glm::mat4& tr);
        void add_translation(glm::vec3 tr);
        const glm::mat4& transform() const;
        std::vector<std::vector<double>> _scales;
        std::vector<std::vector<double>> _translations;
        std::vector<std::vector<double>> _rotations;
        std::vector<modelMatrix> _modelMatrices;
        std::vector<glm::mat4> _inverseMatrices;
        std::vector<double> _timestamps;
        

    private:
        glm::mat4 _transform = glm::mat4(1.0f);

        std::shared_ptr<StaticMesh> _mesh;
        std::shared_ptr<Material> _material;
};

}

#endif // SCENEOBJECT_H
