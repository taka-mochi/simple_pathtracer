#include <vector>
#include "QBVH.h"

#include "SceneObject.h"

namespace SimpleRenderer {

  struct QBVH::QBVH_structure {
    __m128 bboxes[2][3];//4 float min-max xyz
    size_t children[4]; //4 children
    int axis_top;       //top axis
    int axis_left;      //left axis
    int axis_right;     //right axis
    int reserved;       //padding 
  };

  QBVH::~QBVH() {
    delete [] m_root;
    m_root = NULL;
  }

  void QBVH::Construct(const std::vector<SceneObject *> &targets) {
  }

  bool QBVH::CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const {
    // initialize
    int rayDirSign[3] = {
      ray.dir.x >= 0 ? 0 : 1,
      ray.dir.y >= 0 ? 0 : 1,
      ray.dir.z >= 0 ? 0 : 1
    };

    float zerof = 0.0f, inff = INF;
    __m128 zero_m128 = _mm_load1_ps(&zerof);
    __m128 inf_m128 = _mm_load1_ps(&inff);

    float rayorg[3] = {ray.orig.x, ray.orig.y, ray.orig.z};
    __m128 rayOrg[3] = {
      _mm_load1_ps(rayorg+0), // x
      _mm_load1_ps(rayorg+1), // y
      _mm_load1_ps(rayorg+2)  // z
    };


    float raydir[3] = {
      ray.dir.x == 0 ? INF : 1.0f/ray.dir.x,
      ray.dir.y == 0 ? INF : 1.0f/ray.dir.y,
      ray.dir.z == 0 ? INF : 1.0f/ray.dir.z
    };
    __m128 inversedRayDir[3] = {
      _mm_load1_ps(raydir+0), // x
      _mm_load1_ps(raydir+1), // y
      _mm_load1_ps(raydir+2)  // z
    };

    // search loop
    bool intersection_results[4];
    std::vector<int> indicesStack;
    indicesStack.push_back(0);  // root index
    while (!indicesStack.empty()) {
      int nextIndex = indicesStack.back();
      indicesStack.pop_back();

      QBVH_structure *node = &m_root[nextIndex];

      if (BoundingBox::CheckIntersection4floatAABB(
          node->bboxes, rayOrg, inversedRayDir, rayDirSign, zero_m128, inf_m128, intersection_results)) {
        // intersected
        // perform ordering according to axis and ray dir
      } else {
        // no intersection with these boxes (discards)
      }

    }
  }

  void QBVH::Construct_internal(const std::vector<SceneObject *> &targets, int index) {
  }
  void QBVH::MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index) {
  }

}
