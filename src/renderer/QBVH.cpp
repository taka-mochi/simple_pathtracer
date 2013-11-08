#include <iostream>
#include <vector>
#include <limits>
#include <malloc.h>
#include "QBVH.h"
#include "BVH.h"

#include "SceneObject.h"

using namespace std;

namespace SimpleRenderer {

  QBVH::~QBVH() {
  }

  bool QBVH::CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const {
    // initialize
    const int rayDirSign[3] = {
      ray.dir.x >= 0 ? 0 : 1,
      ray.dir.y >= 0 ? 0 : 1,
      ray.dir.z >= 0 ? 0 : 1
    };

    __declspec(align(16)) const float zerof = 0.0f, inff = std::numeric_limits<float>::max();
    const __m128 zero_m128 = _mm_load1_ps(&zerof);
    const __m128 inf_m128 = _mm_load1_ps(&inff);

    __declspec(align(16)) const float rayorg[3] = {
      static_cast<float>(ray.orig.x),
      static_cast<float>(ray.orig.y),
      static_cast<float>(ray.orig.z)
    };
    const __m128 rayOrg[3] = {
      _mm_load1_ps(rayorg+0), // x
      _mm_load1_ps(rayorg+1), // y
      _mm_load1_ps(rayorg+2)  // z
    };

    __declspec(align(16)) const float raydir[3] = {
      ray.dir.x == 0 ? INF : static_cast<float>(1.0f/ray.dir.x),
      ray.dir.y == 0 ? INF : static_cast<float>(1.0f/ray.dir.y),
      ray.dir.z == 0 ? INF : static_cast<float>(1.0f/ray.dir.z)
    };
    const __m128 inversedRayDir[3] = {
      _mm_load1_ps(raydir+0), // x
      _mm_load1_ps(raydir+1), // y
      _mm_load1_ps(raydir+2)  // z
    };

    const QBVH_structure *root = m_root.get();

    // search loop
    bool intersection_results[4];
    std::vector<size_t> indicesStack;
    indicesStack.push_back(0);  // root index
    while (!indicesStack.empty()) {
      size_t nextIndex = indicesStack.back();
      indicesStack.pop_back();

      assert (nextIndex < m_usedNodeCount);

      const QBVH_structure *node = &root[nextIndex];

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
          if (intersection_results[childindex] && IsValidIndex(node->children[childindex])) {
            // intersected. check it
            if (IsChildindexLeaf(node->children[childindex])) {
              // this child node is a leaf
              size_t leafstartindex = GetIndexOfObjectInChildLeaf(node->children[childindex]);
              Scene::IntersectionInformation infoTmp;
              if (CheckIntersection_Leaf(ray, leafstartindex, infoTmp)) {
                  info = infoTmp;
                  return true; // early return: no need further search
              }
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

    return (nearestDist >= 0);
  }

  void QBVH::Construct(const std::vector<SceneObject *> &targets) {
    // at first, construct BVH
    BVH bvh;
    bvh.Construct(BVH::CONSTRUCTION_OBJECT_SAH, targets);

    // allocate memory
    m_allocatedQBVHNodeSize = bvh.GetBVHNodeCount();
    // +16 -> for 16byte alignment
    //m_memoryPoolForQBVH.reset(new unsigned char[sizeof(QBVH_structure)*m_allocatedQBVHNodeSize+16], std::default_delete<unsigned char[]>());
    //unsigned char *poolRoot = m_memoryPoolForQBVH.get();
    //unsigned int offset = 16-reinterpret_cast<unsigned int>(poolRoot)%16;
    //m_root = new(poolRoot+offset) QBVH_structure[m_allocatedQBVHNodeSize];
    QBVH_structure *alignedRoot = new(_aligned_malloc(sizeof(QBVH_structure)*m_allocatedQBVHNodeSize, 16)) QBVH_structure[m_allocatedQBVHNodeSize];
    m_root.reset(alignedRoot, [](void *p){_aligned_free(p);});

    memset(m_root.get(), 0, sizeof(QBVH_structure)*m_allocatedQBVHNodeSize);
    m_usedNodeCount = 1; // for the root node

    // leaf style:
    // [OBJ, OBJ, NULL, OBJ, NULL, OBJ, OBJ, OBJ, NULL, ...]
    // => m_leafObjectArray requires maximum size of "2*targets.size()" size
    m_leafObjectArray.clear(); m_leafObjectArray.reserve(targets.size()*2);

    // flatten the BVH structure to QBVH structure
    const BVH::BVH_structure *bvh_root = bvh.GetRootNode();

    Construct_internal(0, bvh, bvh_root);
  }

  void QBVH::Construct_internal(size_t nextindex, const BVH &bvh, const BVH::BVH_structure *nextTarget) {
    if (bvh.IsLeaf(nextTarget)) {
      cerr << "In QBVH::Construct_internal, the nextindex cannot be the index of leaf node!!" << endl;
      exit(-1);
    }
    if (nextindex >= m_allocatedQBVHNodeSize) {
      cerr << "Allocated QBVH nodes are not sufficient!!" << endl;
      exit(-1);
    }

    QBVH_structure *current = &m_root.get()[nextindex];
    current->axis_top = nextTarget->axis;

    const BVH::BVH_structure *children[2] = {bvh.GetFirstChild(nextTarget), bvh.GetSecondChild(nextTarget)};

    current->axis_left = bvh.IsLeaf(children[0]) ? 0 : children[0]->axis;
    current->axis_right = bvh.IsLeaf(children[1]) ? 0 : children[1]->axis;

    __declspec(align(16)) float four_boxes[2][3][4]; // min-max * xyz * 4box

    auto insertOneBox_to_four_boxes = [&four_boxes](const float box[][3], size_t box_index) {
      for (int xyz=0; xyz<3; xyz++) {four_boxes[0][xyz][box_index] = box[0][xyz]; four_boxes[1][xyz][box_index] = box[1][xyz];}
    };

    for (int i=0; i<2; i++) {
      const BVH::BVH_structure *child = children[i];

      if (bvh.IsLeaf(child)) {
        // child is a leaf
        size_t validchildindex = 0+i*2;
        MakeLeaf_internal(nextindex, child, validchildindex);
        current->children[1+validchildindex] = GetInvalidChildIndex();
        insertOneBox_to_four_boxes(child->box, validchildindex);
      } else {
        // child is not a leaf (has its children)
        const BVH::BVH_structure *grandchildren[2] = {
          bvh.GetFirstChild(child), bvh.GetSecondChild(child)
        };

        for (int j=0; j<2; j++) {
          const BVH::BVH_structure *grandchild = grandchildren[j];

          size_t childindex = i*2+j;
          insertOneBox_to_four_boxes(grandchild->box, childindex);

          if (bvh.IsLeaf(grandchild)) {
            // grandchild is a leaf => MakeLeaf
            MakeLeaf_internal(nextindex, grandchild, childindex);
          } else {
            // grandchild is not a leaf => make new nodes and recursively call this function
            size_t nextNewNodeIndex = m_usedNodeCount;
            current->children[childindex] = nextNewNodeIndex;
            m_usedNodeCount++;
            Construct_internal(nextNewNodeIndex, bvh, grandchild);
          }
        }
      }
    }

    // convert bounding boxes to simd style
    for (int min_max=0; min_max<2; min_max++) for (int xyz=0; xyz<3; xyz++) {
      current->bboxes[min_max][xyz] = _mm_load_ps(four_boxes[min_max][xyz]);
    }
  }
  void QBVH::MakeLeaf_internal(size_t index, const BVH::BVH_structure *leaf, size_t childindex) {
    // child is a leaf
    size_t startindexofobjects = m_leafObjectArray.size();
    for (int j=0; leaf->objects[j] != NULL; j++) {
      m_leafObjectArray.push_back(leaf->objects[j]);
    }
    m_leafObjectArray.push_back(NULL);
    m_root.get()[index].children[childindex] = SetChildindexAsLeaf(startindexofobjects);
  }

  size_t QBVH::SetChildindexAsLeaf(size_t childindex) {
    return static_cast<size_t>(0x80000000) | childindex;
  }
  size_t QBVH::GetInvalidChildIndex() {
    return static_cast<size_t>(-1);
  }
  bool QBVH::IsChildindexLeaf(size_t childindex) {
    return (static_cast<size_t>(0x80000000) & childindex) != 0;
  }
  bool QBVH::IsValidIndex(size_t index) {
    return index != static_cast<size_t>(-1);
  }
  size_t QBVH::GetIndexOfObjectInChildLeaf(size_t childleafindex) {
    return static_cast<size_t>(0x80000000) ^ childleafindex;
  }

  void QBVH::CollectBoundingBoxes(int depth, std::vector<BoundingBox> &result) {
    // for Visualization
    result.clear();
    result.reserve(m_usedNodeCount);
    CollectBoundingBoxes_internal(0, depth, 0, result);
  }

  void QBVH::CollectBoundingBoxes_internal(int currentDepth, int targetDepth, int index, std::vector<BoundingBox> &result) {
    if (targetDepth < currentDepth) return;

    QBVH_structure *current = &m_root.get()[index];

    if (targetDepth == currentDepth)
    {
      __declspec(align(16)) float bboxes[2][3][4];
      for (int min_max=0; min_max<2; min_max++) for (int xyz=0; xyz<3; xyz++) {
        _mm_store_ps(bboxes[min_max][xyz], current->bboxes[min_max][xyz]);
      }

      for (int i=0; i<4; i++) {
        BoundingBox box(Vector3(bboxes[0][0][i], bboxes[0][1][i], bboxes[0][2][i]), Vector3(bboxes[1][0][i], bboxes[1][1][i], bboxes[1][2][i]));
        result.push_back(box);
      }
      return;
    }

    for (int i=0; i<4; i++) {
      if (!IsChildindexLeaf(current->children[i])) {
        CollectBoundingBoxes_internal(currentDepth+1, targetDepth, current->children[i], result);
      }
    }
  }
}
