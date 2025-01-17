#ifndef SCENE_H
#define SCENE_H

#include <SceneObject.h>
#include <BVH.h>
#include <PointLight.h>
#include <Camera.h>

#include <vector>
#include <memory>

namespace OM3D {

class Scene : NonMovable {

    public:
        Scene();

        static Result<std::unique_ptr<Scene>> from_gltf(const std::string& file_name);

        void render() const;

        void init_bvh();

        void add_object(SceneObject obj);
        void add_light(PointLight obj);

        Span<const SceneObject> objects() const;
        Span<const PointLight> point_lights() const;

        Camera& camera();
        const Camera& camera() const;

        void set_sun(glm::vec3 direction, glm::vec3 color);
        const glm::vec3 get_sun_dir();
        const glm::vec3 get_sun_col();

    private:
        
        std::vector<SceneObject> _objects;
        std::vector<PointLight> _point_lights;
        std::shared_ptr<BVH> _bvh;

        glm::vec3 _sun_direction;
        glm::vec3 _sun_color;

        Camera _camera;
};

}

#endif // SCENE_H
