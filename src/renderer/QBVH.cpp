#include <vector>
#include <limits>
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
  }

  void QBVH::Construct(const std::vector<SceneObject *> &targets) {
  }

  bool QBVH::CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const {
    // initialize
    const int rayDirSign[3] = {
      ray.dir.x >= 0 ? 0 : 1,
      ray.dir.y >= 0 ? 0 : 1,
      ray.dir.z >= 0 ? 0 : 1
    };

    const float zerof = 0.0f, inff = std::numeric_limits<float>::max();
    const __m128 zero_m128 = _mm_load1_ps(&zerof);
    const __m128 inf_m128 = _mm_load1_ps(&inff);

    const float rayorg[3] = {
      static_cast<float>(ray.orig.x),
      static_cast<float>(ray.orig.y),
      static_cast<float>(ray.orig.z)
    };
    const __m128 rayOrg[3] = {
      _mm_load1_ps(rayorg+0), // x
      _mm_load1_ps(rayorg+1), // y
      _mm_load1_ps(rayorg+2)  // z
    };

    const float raydir[3] = {
      ray.dir.x == 0 ? INF : static_cast<float>(1.0f/ray.dir.x),
      ray.dir.y == 0 ? INF : static_cast<float>(1.0f/ray.dir.y),
      ray.dir.z == 0 ? INF : static_cast<float>(1.0f/ray.dir.z)
    };
    const __m128 inversedRayDir[3] = {
      _mm_load1_ps(raydir+0), // x
      _mm_load1_ps(raydir+1), // y
      _mm_load1_ps(raydir+2)  // z
    };

    // search loop
    bool intersection_results[4];
    std::vector<size_t> indicesStack;
    indicesStack.push_back(0);  // root index
    while (!indicesStack.empty()) {
      size_t nextIndex = indicesStack.back();
      indicesStack.pop_back();

      const QBVH_structure *node = &m_root[nextIndex];

      if (BoundingBox::CheckIntersection4floatAABB(
          node->bboxes, rayOrg, inversedRayDir, rayDirSign, zero_m128, inf_m128, intersection_results)) {
        // intersected
        // perform ordering according to axis and ray dir
        size_t ordering[4];
        const bool isLeft = rayDirSign[node->axis_top] == 0;
        const int leftIndexFirst = isLeft ? 0 : 2;
        const int rightIndexFirst = (leftIndexFirst+2)%4;
        // node->children[0,1] <- children in left side
        // node->children[2,3] <- children in right side

        // check left side ordering
        if (rayDirSign[node->axis_left] == 0) {
          ordering[leftIndexFirst] = 0; ordering[leftIndexFirst+1] = 1;
        } else {
          ordering[leftIndexFirst] = 1; ordering[leftIndexFirst+1] = 0;
        }
        // check right side ordering
        if (rayDirSign[node->axis_right] == 0) {
          ordering[rightIndexFirst] = 2; ordering[rightIndexFirst+1] = 3;
        } else {
          ordering[rightIndexFirst] = 3; ordering[rightIndexFirst+1] = 2;
        }

        for (int i = 0; i < 4; i++) {
          int childindex = ordering[i];
          if (intersection_results[childindex]) {
            // intersected. check it
            if (node->children[childindex] & (size_t)0x80000000) {
              // this child node is a leaf
              size_t leafstartindex = node->children[childindex]^((size_t)0x80000000);
              if (CheckIntersection_Leaf(ray, leafstartindex, info)) return true; // early return: no need further search
            } else {
              // this child node is not a leaf
              indicesStack.push_back(node->children[childindex]);
            }
          }
        }
      } else {
        // no intersection with these boxes (discards)
      }

    }
    return false;
  }

  bool QBVH::CheckIntersection_Leaf(const Ray &ray, size_t leafStartIndex, Scene::IntersectionInformation &hitResultDetail) const {
    double nearestDist = -1;
    HitInformation info;
    for (size_t i=leafStartIndex; m_leafObjectArray[i]; i++) {
      assert (i+1<m_leafObjectArray.size());

      const SceneObject *obj = m_leafObjectArray[i];
      if (obj->CheckIntersection(ray, info) && (nearestDist < 0 || info.distance < nearestDist)) {
        hitResultDetail.hit = info;
        hitResultDetail.object = m_leafObjectArray[i];
        nearestDist = info.distance;
      }
    }

    return (nearestDist > 0);
  }

  void QBVH::Construct_internal(const std::vector<SceneObject *> &targets, int index) {

    // leaf style:
    // [OBJ, OBJ, NULL, OBJ, NULL, OBJ, OBJ, OBJ, NULL, ...]
    // => m_leafObjectArray requires maximum size of "2*targets.size()" size
    m_leafObjectArray.clear(); m_leafObjectArray.reserve(targets.size()*2);
    //m_leafObjectArray.reset(new SceneObject*[targets.size()*2], std::default_delete<SceneObject *[]>());
  }
  void QBVH::MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index) {
  }

}
