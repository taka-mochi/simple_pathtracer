#pragma once

#include <vector>
#include <memory>
#include "scenes/Scene.h"
#include "BVH.h"

namespace SimpleRenderer {
  class SceneObject;
  class Ray;
  struct Scene::IntersectionInformation;

  class QBVH {
  public:
    explicit QBVH() : m_root(NULL), m_allocatedQBVHNodeSize(0), m_usedNodeCount(0), m_leafObjectArray(NULL) {}
    ~QBVH();

    void Construct(const std::vector<SceneObject *> &targets);
    bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

    void CollectBoundingBoxes(int depth, std::vector<BoundingBox> &result); // for Visualization

  private:
    void Construct_internal(size_t nextindex, const BVH &bvh, const BVH::BVH_structure *nextTarget);
    void MakeLeaf_internal(size_t index, const BVH::BVH_structure *leaf, size_t childindex);

    bool CheckIntersection_Leaf(const Ray &ray, size_t leafStartIndex, Scene::IntersectionInformation &hitResultDetail) const;

    static size_t SetChildindexAsLeaf(size_t childindex);
    static size_t GetInvalidChildIndex();
    static bool IsChildindexLeaf(size_t childindex);
    static bool IsValidIndex(size_t index);
    static size_t GetIndexOfObjectInChildLeaf(size_t childleafindex);

    void CollectBoundingBoxes_internal(int currentDepth, int targetDepth, int index, std::vector<BoundingBox> &result);

    void ReallocateQBVH_root(size_t addSize);

  private:
    struct QBVH_structure  {
      __m128 bboxes[2][3];//4 float min-max xyz
      size_t children[4]; //4 children
      int axis_top;       //top axis
      int axis_left;      //left axis
      int axis_right;     //right axis
      int reserved;       //padding 
    };
    std::shared_ptr<QBVH_structure> m_root;
    size_t m_allocatedQBVHNodeSize, m_usedNodeCount;
    std::vector<SceneObject *> m_leafObjectArray;
    
  };
}
