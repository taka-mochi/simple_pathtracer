#include <iostream>
#include <fstream>
#include "model.h"
#include "polygon.h"
#include "quad.h"
#include "utils.h"

using namespace std;

Model::Model()
  : m_materials()
  , m_meshes()
{
}

Model::~Model()
{
  clear();
}

void Model::clear()
{
  std::vector<Material>::iterator it, end = m_materials.end();
  for (it = m_materials.begin(); it!=end; it++) {
    PolygonList l = getPolygonList(*it);
    for (size_t i=0; i<l.size(); i++) {
      delete l[i];
    }
  }
  m_materials.clear();
  m_meshes.clear();
}

void Model::setPosition(const Vector3 &pos) {
  m_position = pos;

  std::vector<Material>::iterator it, end = m_materials.end();
  for (it = m_materials.begin(); it!=end; it++) {
    PolygonList l = getPolygonList(*it);
    for (size_t i=0; i<l.size(); i++) {
      l[i]->position = pos;
    }
  }
}

bool Model::readFromObj(const std::string &filename) {
  ifstream ifs(filename.c_str());

  if (!ifs) return false;

  clear();

  const static std::string defaultMaterialName = "__default__material__";

  std::unordered_map<string, Material> materialNames;
  materialNames[defaultMaterialName] = Material(Material::REFLECTION_TYPE_LAMBERT, Vector3(0,0,0), Vector3(0.99, 0.99, 0.99));
  m_meshes[materialNames[defaultMaterialName]] = PolygonList();

  string currentMaterialName = defaultMaterialName;
  string currentGroupName;

  vector<Vector3> verticesInGroup;
  vector<Vector3> normalsInGroup;
  vector<Vector3> uvCoordinatesInGroup;

  Material *currentMaterial = &materialNames[currentMaterialName];
  PolygonList *currentPolygonList = &m_meshes[materialNames[defaultMaterialName]];

  while (!ifs.eof()) {
    string line;
    std::getline(ifs, line);

    if (line.find("#") == 0) {
      // comment
    } else if (line.find("mtllib ") == 0) {
      // material name
      //static int index=0;
      //materialNames[line.substr(string("mtllib ").length())] = Material(Material::REFLECTION_TYPE_LAMBERT, Vector3(0,0,0), Vector3(0.999+index*0.001,0.99,0.99));
      //index++;
      // materialÇÉçÅ[ÉhÇ∑ÇÈ
      if (!loadMaterialFile(line.substr(string("mtlib ").length()+1), materialNames)) {
        cerr << "failed to load material file: " << line.substr(string("mtlib ").length()) << endl;
        return false;
      }

    } else if (line.find("g ") == 0) {
      // group name
      // begining of a new group
      currentGroupName = line.substr(string("g ").length());
      verticesInGroup.clear(); normalsInGroup.clear(); uvCoordinatesInGroup.clear();
      currentMaterialName = defaultMaterialName;
      currentMaterial = &materialNames[currentMaterialName];
      currentPolygonList = &m_meshes[*currentMaterial];
    } else if (line.find("usemtl ") == 0) {
      // current material name
      currentMaterialName = line.substr(string("usemtl ").length());
      if (materialNames.find(currentMaterialName) == materialNames.end()) {
        currentMaterialName = defaultMaterialName;
      }
      currentMaterial = &materialNames[currentMaterialName];
      if (m_meshes.find(*currentMaterial) != m_meshes.end()) {
        currentPolygonList = &m_meshes[*currentMaterial];
      } else {
        m_meshes[*currentMaterial] = PolygonList();
        currentPolygonList = &m_meshes[*currentMaterial];
      }
    } else if (line.find("v ") == 0) {
      // vertex
      std::vector<string> vertex = Utils::split(line.substr(string("v ").length()), ' ');
      if (vertex.size() != 3) {
        cerr << "not correct .obj file!!: " << line << endl;
        return false;
      } else {
        verticesInGroup.push_back(Vector3(atof(vertex[0].c_str()), atof(vertex[1].c_str()), atof(vertex[2].c_str())));
      }
    } else if (line.find("vt ") == 0) {
      // texture uv
      std::vector<string> vertex = Utils::split(line.substr(string("vt ").length()), ' ');
      if (vertex.size() != 2) {
        cerr << "not correct .obj file!!: " << line << endl;
        return false;
      } else {
        uvCoordinatesInGroup.push_back(Vector3(atof(vertex[0].c_str()), atof(vertex[1].c_str()), 0.0));
      }
    } else if (line.find("vn ") == 0) {
      // normal coordinate
      std::vector<string> vertex = Utils::split(line.substr(string("vn ").length()), ' ');
      if (vertex.size() != 3) {
        cerr << "not correct .obj file!!: " << line << endl;
        return false;
      } else {
        normalsInGroup.push_back(Vector3(atof(vertex[0].c_str()), atof(vertex[1].c_str()), atof(vertex[2].c_str())));
      }
    } else if (line.find("f ") == 0) {
      // face
      vector<string> faces = Utils::split(line.substr(string("f ").length()), ' ');
      if (faces.size() >= 5) {
        cerr << "5 or more vertices faces are not supported!!!: " << line << endl;
        return false;
      } else if (faces.size() == 4) {
        vector<PolygonPtr> polygons = load4verticesFace(faces, verticesInGroup, normalsInGroup, uvCoordinatesInGroup, materialNames[currentMaterialName]);
        currentPolygonList->push_back(polygons[0]);
        currentPolygonList->push_back(polygons[1]);
      } else if (faces.size() == 3) {
        PolygonPtr polygon(load3verticesFace(faces, verticesInGroup, normalsInGroup, uvCoordinatesInGroup, materialNames[currentMaterialName]));
        currentPolygonList->push_back(polygon);
      }
    }
  }

  unordered_map<string, Material>::iterator it,end = materialNames.end();
  m_materials.clear();
  for (it=materialNames.begin(); it!=end; it++) {
    m_materials.push_back(it->second);
  }

  return true;
}

bool Model::loadMaterialFile(const std::string &filename, std::unordered_map<string, Material> &materials) {
  ifstream ifs(filename.c_str());

  if (!ifs) return false;

  string currentMaterialName = "";
  Material currentMaterial;

  Color ambient, diffuse, specular;

  while (!ifs.eof()) {
    string line;
    getline(ifs, line);

    if (line.find("#") == 0) {
      // comment
    } else if (line.find("newmtl ") == 0) {
      // new material
      if (currentMaterialName != "") {
        // register current material
        switch (currentMaterial.reflection_type) {
        case Material::REFLECTION_TYPE_LAMBERT:
        case Material::REFLECTION_TYPE_REFRACTION:
          currentMaterial.color = diffuse;
          break;
        case Material::REFLECTION_TYPE_SPECULAR:
          currentMaterial.color = specular;
          break;
        }
        materials[currentMaterialName] = currentMaterial;
      }
      currentMaterialName = line.substr(string("newmtl ").length());
    } else if (line.find("Ns ") == 0) {
      // Shininess
      // ignore
    } else if (line.find("d ") == 0) {
      // ???
    } else if (line.find("Ni ") == 0) {
      // ???
    } else if (line.find("illum ") == 0) {
      // type
      // 0: no diffuse
      // 1: diffuse & ambient (but ambient is ignored)
      // 2: specular (original setting: full lighting)
      // 3: refractance REFRACTANCE ratio
      vector<string> values(Utils::split(line.substr(string("illum ").length()), ' '));
      string value = values[0];
      switch (atoi(value.c_str())) {
      case 0:
      case 1:
        currentMaterial.reflection_type = Material::REFLECTION_TYPE_LAMBERT;
        break;
      case 2:
        currentMaterial.reflection_type = Material::REFLECTION_TYPE_SPECULAR;
        break;
      case 3:
        assert (values.size() == 2);
        currentMaterial.reflection_type = Material::REFLECTION_TYPE_REFRACTION;
        currentMaterial.refraction_rate = atof(values[1].c_str());
        break;
      }
    } else if (line.find("Ka ") == 0) {
      // ambient color
      // ignore
    } else if (line.find("Kd ") == 0) {
      // diffuse color
      vector<string> values(Utils::split(line.substr(string("Kd ").length()), ' '));
      assert (values.size() == 3);
      diffuse.x = atof(values[0].c_str());
      diffuse.y = atof(values[1].c_str());
      diffuse.z = atof(values[2].c_str());
    } else if (line.find("Ks ") == 0) {
      // specular color
      vector<string> values(Utils::split(line.substr(string("Ks ").length()), ' '));
      assert (values.size() == 3);
      specular.x = atof(values[0].c_str());
      specular.y = atof(values[1].c_str());
      specular.z = atof(values[2].c_str());
    } else if (line.find("map_Kd ") == 0) {
      // texture name
    }
  }

  switch (currentMaterial.reflection_type) {
  case Material::REFLECTION_TYPE_LAMBERT:
  case Material::REFLECTION_TYPE_REFRACTION:
    currentMaterial.color = diffuse;
    break;
  case Material::REFLECTION_TYPE_SPECULAR:
    currentMaterial.color = specular;
    break;
  }
  materials[currentMaterialName] = currentMaterial;

  return true;
}

std::vector<Model::PolygonPtr> Model::load4verticesFace(const vector<string> &face, const vector<Vector3> &verticesInGroup, 
  const vector<Vector3> &normalsInGroup, 
  const vector<Vector3> &uvCoordinatesInGroup, const Material &mat) {
  vector<string> face1, face2;
  face1.push_back(face[0]); face1.push_back(face[1]); face1.push_back(face[2]);
  face2.push_back(face[0]); face2.push_back(face[2]); face2.push_back(face[3]);
  PolygonPtr p1 = load3verticesFace(face1, verticesInGroup, normalsInGroup, uvCoordinatesInGroup, mat);
  PolygonPtr p2 = load3verticesFace(face2, verticesInGroup, normalsInGroup, uvCoordinatesInGroup, mat);

  vector<PolygonPtr> ret;
  ret.push_back(p1); ret.push_back(p2);

  return ret;
}

Model::PolygonPtr Model::load3verticesFace(const vector<string> &face, const vector<Vector3> &verticesInGroup, 
  const vector<Vector3> &normalsInGroup, 
  const vector<Vector3> &uvCoordinatesInGroup, const Material &mat) {
  Vector3 vec[3], normals[3];

  for (size_t i=0; i<face.size(); i++) {
    vector<string> data(Utils::split(face[i], '/'));

    int vertex_number = -1; int uv_numver = -1; int normal_number = -1;
    if (data.size() == 1) {
      vertex_number = atoi(data[0].c_str())-1;
    }
    if (data.size() == 2) {
      vertex_number = atoi(data[0].c_str())-1;
      uv_numver = atoi(data[1].c_str())-1;
    } else if (data.size() == 3) {
      vertex_number = atoi(data[0].c_str())-1;
      if (data[1].length() != 0) uv_numver = atoi(data[1].c_str())-1;
      normal_number = atoi(data[2].c_str())-1;
    }

    assert (vertex_number != -1);
    size_t index = i;
    vec[index] = verticesInGroup[vertex_number];
    //vec[index].z *= -1;
    if (normal_number != -1) {
      normals[index] = normalsInGroup[normal_number];
      //normals[index].z *= -1;
    }
  }

  Vector3 averaged_normal;
  for (int i=0; i<3; i++) {
    averaged_normal += normals[i];
  }

  PolygonPtr ret_p;
  if (averaged_normal.x == 0 && averaged_normal.y == 0 && averaged_normal.z == 0) {
    // normal ÇÃéwíËÇ»Ç©Ç¡ÇΩ
    ret_p = ( new Polygon(vec[0], vec[1], vec[2], Polygon::calculateNormal(vec[0], vec[1], vec[2]), mat, Vector3(0,0,0)) );
  } else {
    averaged_normal /= 3.0;
    ret_p = ( new Polygon(vec[0], vec[1], vec[2], averaged_normal*-1, mat, Vector3(0,0,0)) );
  }

  return ret_p;
}
