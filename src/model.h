#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "SceneObject.h"
#include "material.h"
#include "polygon.h"
#include "quad.h"

class Model {
public:
  typedef Polygon * PolygonPtr;
  typedef std::vector<PolygonPtr> PolygonList;

public:
  Model();
  ~Model();

  bool readFromObj(const std::string &filename);

  size_t getMaterialCount() const {
    return m_materials.size();
  }
  const Material &getMaterial(size_t i) const {
    return m_materials.at(i);
  }
  const PolygonList &getPolygonList(const Material &mat) const {
    return m_meshes.find(mat)->second;
  }

private:
  void clear();
  std::vector<PolygonPtr> Model::load4verticesFace(const std::vector<std::string> &face,
    std::vector<Vector3> verticesInGroup, 
    std::vector<Vector3> normalsInGroup, 
    std::vector<Vector3> uvCoordinatesInGroup,
    const Material &mat);
  PolygonPtr Model::load3verticesFace(const std::vector<std::string> &face, std::vector<Vector3> verticesInGroup, 
    std::vector<Vector3> normalsInGroup, 
    std::vector<Vector3> uvCoordinatesInGroup,
    const Material &mat);

private:
  std::vector<Material> m_materials;
  std::unordered_map<Material, PolygonList, MaterialHash, MaterialEq> m_meshes;
};
