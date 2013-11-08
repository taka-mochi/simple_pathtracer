#include <iostream>
#include "BVH.h"
#include <stack>
#include <algorithm>
#include <emmintrin.h>

using namespace std;

namespace {
};

namespace SimpleRenderer {

BVH::~BVH()
{
  //delete [] m_root;
  m_root.clear();
}

bool BVH::CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const {
  info.hit.distance = INF;
  info.object = NULL;

  std::vector<size_t> next_list;
  next_list.reserve(m_root.size());
  next_list.push_back(0);

  const float rayDir[3] = {static_cast<float>(ray.dir.x), static_cast<float>(ray.dir.y), static_cast<float>(ray.dir.z)};
  const float rayOrig[3] = {static_cast<float>(ray.orig.x), static_cast<float>(ray.orig.y), static_cast<float>(ray.orig.z)};

  while (!next_list.empty()) {
    const BVH_structure *next = &m_root[next_list.back()];
    next_list.pop_back();

    if (next->children[0] == -1) {
      // leaf
      HitInformation hit;
      bool isHit = false;
      for (size_t i=0; next->objects[i]; i++) {
        if (next->objects[i]->CheckIntersection(ray, hit)) {
          if (info.hit.distance > hit.distance) {
            isHit = true;
            info.hit = hit;
            info.object = next->objects[i];
          }
        }
      }
      if (isHit) {
        return true;
      }
    } else {
      // internal node
      // check intersection with children
      float dist1 = INF, dist2 = INF;
      const BVH_structure *next1 = &m_root[next->children[0]];
      bool hit1 = BoundingBox::CheckIntersection(rayDir, rayOrig, next1->box[0], next1->box[1], dist1);//next1->box.Intersect(ray, dist1);
      bool hit2 = false;
      if (next->children[1] >= 0) {
        const BVH_structure *next2 = &m_root[next->children[1]];
        hit2 = BoundingBox::CheckIntersection(rayDir, rayOrig, next2->box[0], next2->box[1], dist1); //m_root[next->children[1]].box.Intersect(ray, dist2);
      }
      if (hit1 && hit2) {
        if (dist1 < dist2) {
          // check child1 at first
          next_list.push_back(next->children[1]);
          next_list.push_back(next->children[0]);
        } else {
          // check child2 at first
          next_list.push_back(next->children[0]);
          next_list.push_back(next->children[1]);
        }
      } else if (hit1) {
        next_list.push_back(next->children[0]);
      } else if (hit2) {
        next_list.push_back(next->children[1]);
      }
    }
  }

  return info.hit.distance != INF;
}

void BVH::Construct(const BVH::CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets)
{
  //if (m_root != NULL) {
  //  delete [] m_root;
  //}
  m_root.clear();

  // how many nodes are required????
  // Should I use dynamic array (vector)???

  //m_root = new BVH_structure[8*targets.size()+1]; // max size is 2*N+1
  m_root.reserve(8*targets.size());
  //m_bvh_node_size = targets.size()*targets.size()+1;
  //m_bvh_node_size = 8*targets.size()+1;
  m_root.push_back(BVH_structure());

  Construct_internal(type, targets, 0);
}

namespace {
void CalcBoundingBoxOfObjects(const std::vector<SceneObject *> &objects, BoundingBox &boxResult)
{
  Vector3 box_min(objects[0]->boundingBox.min());
  Vector3 box_max(objects[0]->boundingBox.max());
  std::for_each(objects.begin(), objects.end(),
    [&box_min, &box_max](const SceneObject * const &a) {
      if (a->boundingBox.min().x < box_min.x) box_min.x = a->boundingBox.min().x;
      if (a->boundingBox.min().y < box_min.y) box_min.y = a->boundingBox.min().y;
      if (a->boundingBox.min().z < box_min.z) box_min.z = a->boundingBox.min().z;
      if (a->boundingBox.max().x > box_max.x) box_max.x = a->boundingBox.max().x;
      if (a->boundingBox.max().y > box_max.y) box_max.y = a->boundingBox.max().y;
      if (a->boundingBox.max().z > box_max.z) box_max.z = a->boundingBox.max().z;
  });
  boxResult.SetBox(box_min, box_max);
}

template <typename FLOATING> void CalcBoundingBoxOfObjects(const std::vector<SceneObject *> &objects, FLOATING min[3], FLOATING max[3])
{
  min[0] = static_cast<FLOATING>(objects[0]->boundingBox.min().x);
  min[1] = static_cast<FLOATING>(objects[0]->boundingBox.min().y);
  min[2] = static_cast<FLOATING>(objects[0]->boundingBox.min().z);
  max[0] = static_cast<FLOATING>(objects[0]->boundingBox.max().x);
  max[1] = static_cast<FLOATING>(objects[0]->boundingBox.max().y);
  max[2] = static_cast<FLOATING>(objects[0]->boundingBox.max().z);
  std::for_each(objects.begin(), objects.end(),
    [&min, &max](const SceneObject * const &a) {
      if (a->boundingBox.min().x < min[0]) min[0] = static_cast<FLOATING>(a->boundingBox.min().x);
      if (a->boundingBox.min().y < min[1]) min[1] = static_cast<FLOATING>(a->boundingBox.min().y);
      if (a->boundingBox.min().z < min[2]) min[2] = static_cast<FLOATING>(a->boundingBox.min().z);
      if (a->boundingBox.max().x > max[0]) max[0] = static_cast<FLOATING>(a->boundingBox.max().x);
      if (a->boundingBox.max().y > max[1]) max[1] = static_cast<FLOATING>(a->boundingBox.max().y);
      if (a->boundingBox.max().z > max[2]) max[2] = static_cast<FLOATING>(a->boundingBox.max().z);
  });

}
}

void BVH::MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index)
{
  BVH_structure *st = &m_root[index];
  st->children[0] = st->children[1] = -1;
  for (size_t i=0; i<targets.size(); i++)
    st->objects[i] = targets[i];
  st->objects[targets.size()] = NULL;
  CalcBoundingBoxOfObjects<float>(targets, st->box[0], st->box[1]);
}


void BVH::Construct_internal(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets, int index)
{
  //assert (index < m_bvh_node_size);

  const double T_aabb = 1.0; // cost of check intersection of AABB
  const double T_tri = 1.0; // cost of check intersection of Triangle

  // calculate this node's bounding box
  CalcBoundingBoxOfObjects(targets, m_root[index].box[0], m_root[index].box[1]);
  double currentBoxSurface = BoundingBox::CalcSurfaceArea(m_root[index].box[0], m_root[index].box[1]);
  double currentBoxSurfaceInverse = 1.0/currentBoxSurface;

  std::vector<SceneObject *> axisSortedLeft[3], axisSortedRight[3];

  int bestAxis = -1;
  int bestIndex = -1;
  double bestCost = -1;

  for (int axis=0; axis<3; axis++) {

    axisSortedLeft[axis] = targets;

    // sort objects by the axis
    std::sort(axisSortedLeft[axis].begin(), axisSortedLeft[axis].end(),
      [&axis](SceneObject * const &a, SceneObject * const &b) -> bool {
        switch (axis)
        {
        case 0: return a->boundingBox.position().x < b->boundingBox.position().x;
        case 1: return a->boundingBox.position().y < b->boundingBox.position().y;
        case 2: return a->boundingBox.position().z < b->boundingBox.position().z;
        }
        assert(false);
        return a->boundingBox.position().x < b->boundingBox.position().x;
    });

    switch (type) {
    case CONSTRUCTION_OBJECT_MEDIAN:
      if (targets.size() > 2)
      {
        // select the media of the objects
        int select = axisSortedLeft[axis].size()/2-1;
        SceneObject *selectedObj = axisSortedLeft[axis][select];
        SceneObject *leftest = axisSortedLeft[axis][0];
        SceneObject *rightest = axisSortedLeft[axis][axisSortedLeft[axis].size()-1];
        double dist = (selectedObj->boundingBox.position()-leftest->boundingBox.position()).lengthSq() + 
          (selectedObj->boundingBox.position()-rightest->boundingBox.position()).lengthSq();

        if (dist > bestCost) {
          bestCost = dist;
          bestIndex = select;
          bestAxis = axis;
        }
      }
      break;
    case CONSTRUCTION_OBJECT_SAH: 
    {
      double *leftAreas = new double[axisSortedLeft[axis].size()];
      double *rightAreas = new double[axisSortedLeft[axis].size()];
      rightAreas[axisSortedLeft[axis].size()-1] = INF;

      BoundingBox boxTmp;
      axisSortedRight[axis].push_back(axisSortedLeft[axis].back());
      axisSortedLeft[axis].pop_back();

      // calculate right area
      CalcBoundingBoxOfObjects(axisSortedRight[axis], boxTmp);
      for (int i=axisSortedLeft[axis].size()-1; i>=0; i--) {
        rightAreas[i] = boxTmp.CalcSurfaceArea();
        SceneObject *obj = axisSortedLeft[axis][i];
        boxTmp.MergeAnotherBox(obj->boundingBox);

        axisSortedRight[axis].push_back(axisSortedLeft[axis].back());
        axisSortedLeft[axis].pop_back();
      }

      axisSortedLeft[axis].push_back(axisSortedRight[axis].back());
      axisSortedRight[axis].pop_back();
      CalcBoundingBoxOfObjects(axisSortedLeft[axis], boxTmp);
      
      const double leafCost = T_tri * targets.size();
      for (int i=axisSortedRight[axis].size()-1, cutIndex=0; i>=0; i--, cutIndex++) {
        // calculate both surface area
        leftAreas[cutIndex] = boxTmp.CalcSurfaceArea();
        SceneObject *nextObj = axisSortedRight[axis].back();
        boxTmp.MergeAnotherBox(nextObj->boundingBox);

        // move right to left
        axisSortedLeft[axis].push_back(axisSortedRight[axis].back());
        axisSortedRight[axis].pop_back();

        // calc SAH
        double cost = 2*T_aabb + (leftAreas[cutIndex]*axisSortedLeft[axis].size() + rightAreas[cutIndex]*axisSortedRight[axis].size())*currentBoxSurfaceInverse * T_tri;

        if ((cost < bestCost || bestCost == -1) && cost < leafCost) {
          bestCost = cost;
          bestIndex = cutIndex;
          bestAxis = axis;
        } else {
          int a = 10;
        }

      }
      delete [] leftAreas;
      delete [] rightAreas;
    }
      break;
    }
  }

  if (bestAxis == -1) {
    // make leaf
    MakeLeaf_internal(targets, index);
    return;
  }


  std::vector<SceneObject *> lefts = targets;
  std::sort(lefts.begin(), lefts.end(),
    [&bestAxis](SceneObject * const &a, SceneObject * const &b) -> bool {
      switch (bestAxis)
      {
      case 0: return a->boundingBox.position().x < b->boundingBox.position().x;
      case 1: return a->boundingBox.position().y < b->boundingBox.position().y;
      case 2: return a->boundingBox.position().z < b->boundingBox.position().z;
      }
      assert(false);
      return a->boundingBox.position().x < b->boundingBox.position().x;
  });

  std::vector<SceneObject *> rights(lefts.begin()+bestIndex+1, lefts.end());
  lefts.resize(bestIndex+1);

  BVH_structure *current = &m_root[index];
  current->axis = bestAxis;
  current->children[0] = m_root.size();
  current->children[1] = m_root.size()+1;
  m_root.push_back(BVH_structure()); m_root.push_back(BVH_structure());

  // constructs children
  Construct_internal(type, lefts, current->children[0]);
  Construct_internal(type, rights, current->children[1]);
}

void BVH::CollectBoundingBoxes(int depth, std::vector<BoundingBox> &result)
{
  result.clear();
  result.reserve(m_root.size());
  CollectBoundingBoxes_internal(0, depth, 0, result);
}

void BVH::CollectBoundingBoxes_internal(int currentDepth, int targetDepth, int index, std::vector<BoundingBox> &result)
{
  if (targetDepth < currentDepth) return;

  BVH_structure *current = &m_root[index];

  if (targetDepth == currentDepth)
  {
    BoundingBox box(current->box[0], current->box[1]);
    result.push_back(box);
    return;
  }

  if (current->children[0] == -1) return;
  CollectBoundingBoxes_internal(currentDepth+1, targetDepth, current->children[0], result);

  if (current->children[1] == -1) return;
  CollectBoundingBoxes_internal(currentDepth+1, targetDepth, current->children[1], result);
}

const BVH::BVH_structure *BVH::GetRootNode() const {
  return &m_root[0];
}

size_t BVH::GetBVHNodeCount() const {
  return m_root.size();
}

const BVH::BVH_structure *BVH::GetFirstChild(const BVH::BVH_structure *parent) const {
  assert (parent);
  if (IsLeaf(parent)) return NULL;
  assert (m_root.size() > parent->children[0]);
  return &m_root[parent->children[0]];
}

const BVH::BVH_structure *BVH::GetSecondChild(const BVH::BVH_structure *parent) const {
  assert (parent);
  if (IsLeaf(parent)) return NULL;
  assert (m_root.size() > parent->children[1]);
  return &m_root[parent->children[1]];
}

bool BVH::IsLeaf(const BVH_structure *node) const {
  assert (node);
  return node->children[0] == static_cast<size_t>(-1);
}

}
