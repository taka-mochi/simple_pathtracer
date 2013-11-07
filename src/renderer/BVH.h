
#pragma once

#include <vector>
#include "scenes/Scene.h"
#include "SceneObject.h"

namespace SimpleRenderer {

class BVH {
public:
  enum CONSTRUCTION_TYPE {
    CONSTRUCTION_OBJECT_MEDIAN,
    CONSTRUCTION_OBJECT_SAH,
  };

public:
  static const int MAX_LEAF_COUNT_IN_ONE_BVH_NODE = 22;

  class BVH_structure {
  public:
    //BoundingBox box;
    float box[2][3];
    unsigned int children[2];
    //BVH_structure *children[2];
    //std::vector<SceneObject *> objects;
    int axis;
    SceneObject *objects[MAX_LEAF_COUNT_IN_ONE_BVH_NODE+1];

    BVH_structure() {objects[0] = NULL;}
  };

public:
  explicit BVH() : m_root() {}
  ~BVH();

  void Construct(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets);
  bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

  const BVH_structure *GetRootNode() const;
  size_t GetBVHNodeCount() const;
  const BVH_structure *GetFirstChild(const BVH_structure *parent) const;
  const BVH_structure *GetSecondChild(const BVH_structure *parent) const;
  bool IsLeaf(const BVH_structure *node) const;

private:
  void Construct_internal(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets, int index);
  void MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index);

private:
  std::vector<BVH_structure> m_root;
  //int m_bvh_node_size;
};

}
