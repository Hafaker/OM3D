#include "SceneObject.h"
#include "StaticMesh.h"

namespace OM3D {

	class BVH {
	public:
		BVH(std::vector<SceneObject>);
		void split();

		void render(Frustum frustum, glm::vec3 pos);

		std::vector<SceneObject> _leaves;
		SphereBoundingBox _bbox;

	private:

		std::shared_ptr<BVH> _bvh1;
		std::shared_ptr<BVH> _bvh2;
	};
}