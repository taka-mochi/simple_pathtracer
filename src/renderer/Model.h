#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "SceneObject.h"
#include "Material.h"
#include "Polygon.h"


namespace SimpleRenderer {

class Matrix;

class Model {
public:
  typedef Polygon * PolygonPtr;
  typedef std::vector<PolygonPtr> PolygonList;

public:
  Model();
  ~Model();

  //void setPosition(const Vector3 &pos);
  const Vector3 &GetPosition() const {
    return m_position;
  }
  void SetTransform(const Vector3 &pos, const Vector3 &scale = Vector3::One(), const Matrix &rot = Matrix::Identity());
  //void setRotation(const Matrix &matrix);

  bool ReadFromObj(const std::string &filename);

  size_t GetMaterialCount() const {
    return m_materials.size();
  }
  const Material &GetMaterial(size_t i) const {
    return m_materials.at(i);
  }
  const PolygonList &GetPolygonList(const Material &mat) const {
    return m_meshes.find(mat)->second;
  }

private:
  void Clear();
  bool LoadMaterialFile(const std::string &filename, std::unordered_map<std::string, Material> &materials);
  std::vector<PolygonPtr> load4verticesFace(const std::vector<std::string> &face,
    const std::vector<Vector3> &verticesInGroup, 
    const std::vector<Vector3> &normalsInGroup, 
    const std::vector<Vector3> &uvCoordinatesInGroup,
    const Material &mat);
  PolygonPtr Load3verticesFace(const std::vector<std::string> &face,
    const std::vector<Vector3> &verticesInGroup, 
    const std::vector<Vector3> &normalsInGroup, 
    const std::vector<Vector3> &uvCoordinatesInGroup,
    const Material &mat);

private:
  std::vector<Material> m_materials;
  std::unordered_map<Material, PolygonList, MaterialHash, MaterialEq> m_meshes;

  Vector3 m_position;
};

}
