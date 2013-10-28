#include <iostream>
#include "bvh.h"
#include <stack>
#include <algorithm>
#include <emmintrin.h>

using namespace std;

//#pragma pack(push,4)
class BVH::BVH_structure {
public:
  BoundingBox box;
  int children[2];
  //BVH_structure *children[2];
  std::vector<SceneObject *> objects;
};
//#pragma pack(pop)

BVH::~BVH()
{
  delete [] m_root;
}

bool BVH::CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const {
  info.hit.distance = INF;
  info.object = NULL;

  std::stack<BVH_structure *> next_list;
  next_list.push(m_root);

  while (!next_list.empty()) {
    BVH_structure *next = next_list.top();
    next_list.pop();

    if (next->children[0] == -1) {
      // leaf
      HitInformation hit;
      bool isHit = false;
      for (size_t i=0; i<next->objects.size(); i++) {
        if (next->objects[i]->Intersect(ray, hit)) {
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
      double dist1 = INF, dist2 = INF;
      BVH_structure *next1 = &m_root[next->children[0]];
      bool hit1 = next1->box.Intersect(ray, dist1);
      bool hit2 = false;
      if (next->children[1] >= 0) {
        hit2 = m_root[next->children[1]].box.Intersect(ray, dist2);
      }
      if (hit1 && hit2) {
        if (dist1 < dist2) {
          // check child1 at first
          next_list.push(&m_root[next->children[1]]);
          next_list.push(&m_root[next->children[0]]);
        } else {
          // check child2 at first
          next_list.push(&m_root[next->children[0]]);
          next_list.push(&m_root[next->children[1]]);
        }
      } else if (hit1) {
        next_list.push(&m_root[next->children[0]]);
      } else if (hit2) {
        next_list.push(&m_root[next->children[1]]);
      }
    }
  }

  return info.hit.distance != INF;
}

void BVH::Construct(const BVH::CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets)
{
  if (m_root != NULL) {
    delete [] m_root;
  }

  m_root = new BVH_structure[targets.size()*2+1]; // max size is 2*N+1

  Construct_internal(type, targets, 0);
}

void BVH::MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index)
{
  BVH_structure *st = &m_root[index];
  st->children[0] = st->children[1] = -1;
  st->objects = targets;
  if (targets.size() == 1) st->box = targets[0]->boundingBox;
  else st->box = BoundingBox::CompoundBoxes(targets[0]->boundingBox, targets[1]->boundingBox);
}


void BVH::Construct_internal(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets, int index)
{
  if (targets.size() <= 2) {
    // make leaf
    MakeLeaf_internal(targets, index);
    return;
  }

  // calculate this node's bounding box
  Vector3 box_min(targets[0]->boundingBox.min());
  Vector3 box_max(targets[0]->boundingBox.max());
  std::for_each(targets.begin(), targets.end(),
    [&box_min, &box_max](const SceneObject * const &a) {
      if (a->boundingBox.min().x < box_min.x) box_min.x = a->boundingBox.min().x;
      if (a->boundingBox.min().y < box_min.y) box_min.y = a->boundingBox.min().y;
      if (a->boundingBox.min().z < box_min.z) box_min.z = a->boundingBox.min().z;
      if (a->boundingBox.max().x > box_max.x) box_max.x = a->boundingBox.max().x;
      if (a->boundingBox.max().y > box_max.y) box_max.y = a->boundingBox.max().y;
      if (a->boundingBox.max().z > box_max.z) box_max.z = a->boundingBox.max().z;
  });
  m_root[index].box = BoundingBox(box_min, box_max);

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
    case OBJECT_MEDIAN:
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
    //case OBJECT_VOLUME:
    //for (int i=axisSortedLeft[axis].size()-1; i>=0; i--) {


    //}
      //break;
    }
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
  // heap structure
  current->children[0] = index*2+1;
  current->children[1] = index*2+2;

  // constructs children
  Construct_internal(type, lefts, current->children[0]);
  Construct_internal(type, rights, current->children[1]);
}
