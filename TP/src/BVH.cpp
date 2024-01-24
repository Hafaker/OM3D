#include "BVH.h"
#include <cmath>
#include <algorithm>

namespace OM3D {

    BVH::BVH(std::vector<SceneObject> leaves)
    {
        _leaves = leaves;
    }

    bool test_Plane(SphereBoundingBox bbox, glm::vec3 normal, glm::vec3 position) {
        return glm::dot(normal, bbox.center) > glm::dot(normal, position) - bbox.radius;
    }

    void BVH::render(Frustum frustum, glm::vec3 pos)
    {
        if (!_bvh1 && !_bvh2)
        {
            for (const SceneObject& obj : _leaves) {
                obj.render(frustum);
            }
        }
        else
        {
            /*if (test_Plane(_bbox, frustum._near_normal, pos) &&
                test_Plane(_bbox, frustum._top_normal, pos) &&
                test_Plane(_bbox, frustum._bottom_normal, pos) &&
                test_Plane(_bbox, frustum._right_normal, pos) &&
                test_Plane(_bbox, frustum._left_normal, pos))
            {
                if (_bvh1)
                    _bvh1->render(frustum, pos);
                if (_bvh2)
                    _bvh2->render(frustum, pos);
            }*/
            if (_bvh1)
                _bvh1->render(frustum, pos);
            if (_bvh2)
                _bvh2->render(frustum, pos);
        }
    }

    SceneObject get_furthest(SceneObject v1, std::vector<SceneObject> list)
    {
        SceneObject res = list.at(0);
        double max = 0.0;
        for (const SceneObject& obj : list) {
            double dist = glm::distance(v1._mesh->_bbox.center, obj._mesh->_bbox.center);
            if (max < dist)
            {
                max = dist;
                res = obj;
            }
        }
        return res;
    }

    void BVH::split() {
        if (_leaves.size() <= 2)
            return;

        SceneObject obj1 = _leaves.at(0);
        SceneObject obj2 = _leaves.at(1);
        double maxdist = 0.0;
        for (const SceneObject& obj : _leaves) {
            auto furthest = get_furthest(obj, _leaves);
            double dist = glm::distance(obj._mesh->_bbox.center, furthest._mesh->_bbox.center) + obj._mesh->_bbox.radius + furthest._mesh->_bbox.radius;
            if (dist > maxdist) {
                obj1 = obj;
                obj2 = furthest;
                maxdist = dist;
            }
            
        }
        glm::vec3 A = obj1._mesh->_bbox.center;
        glm::vec3 B = obj2._mesh->_bbox.center;
        
        glm::vec3 middle = glm::vec3((A[0] + B[0]) / 2, (A[1] + B[1]) / 2, (A[2] + B[2]) / 2);
        double distmid = glm::distance(A, middle);
        std::vector<SceneObject> low = {  };
        std::vector<SceneObject> high = {  };

        glm::vec3 AB = glm::vec3(B[0] - A[0], B[1] - A[1], B[2] - A[2]);

        float max_radius_low = 0.0;
        float max_radius_high = 0.0;
        for (const SceneObject& obj : _leaves) {
            glm::vec3 C = obj._mesh->_bbox.center;
            double tmp = (AB[0] * (C[0] - A[0]) + AB[1] * (C[1] - A[1]) + AB[2] * (C[2] - A[2])) / (pow(AB[0], 2) + pow(AB[1], 2) + pow(AB[2], 2));
            glm::vec3 H = glm::vec3(A[0] + AB[0] * tmp, A[1] + AB[1] * tmp, A[1] + AB[1] * tmp);
            double dist = glm::distance(A, H);
            if (dist < distmid)
            {
                low.push_back(obj);
                max_radius_high = std::max(max_radius_high, obj._mesh->_bbox.radius);
            }
            else
            {
                high.push_back(obj);
                max_radius_low = std::max(max_radius_low, obj._mesh->_bbox.radius);
            }
        }



        if (low.size() != 0)
        {
            _bvh1 = std::make_shared<BVH>(BVH(low));
            _bvh1->_bbox.center = glm::vec3((A[0] + middle[0]) / 2, (A[1] + middle[1]) / 2, (A[2] + middle[2]) / 2);
            _bvh1->_bbox.radius = (float)(max_radius_low + distmid) / 2;
            if (high.size() != 0) {
                _bvh1->split();
            }

        }
        if (high.size() != 0)
        {
            _bvh2 = std::make_shared<BVH>(BVH(high));
            _bvh2->_bbox.center = glm::vec3((B[0] + middle[0]) / 2, (B[1] + middle[1]) / 2, (B[2] + middle[2]) / 2);
            _bvh2->_bbox.radius = (float)(max_radius_high + distmid) / 2;
            if (low.size() != 0) {
                _bvh2->split();
            }
        }
    }
}