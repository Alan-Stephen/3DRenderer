#include "Model.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "Material.h"
#include <vector>
#include "glm/glm.hpp"

void m_remove_tabs(std::string& str) {
    size_t pos;
    while ((pos = str.find('\t')) != std::string::npos) {
        str.erase(pos, 1); 
    }
}


// splits a string into a vector of strings (tokens) based on a delimiter
std::vector<std::string> m_split(std::string s, std::string delim) {
    size_t start = 0; 
	size_t end; 
	size_t len = delim.length();
    std::string token;
    std::vector<std::string> res;

    while ((end = s.find(delim, start)) != std::string::npos) {
        token = s.substr(start, end - start);
		m_remove_tabs(token);
        start = end + len;
		if (token == "") {
			continue;
		}
        res.push_back(token);
    }

    res.push_back(s.substr (start));
    return res;
}

int m_mtl_parse(char* filename, std::vector<Material> &mtls)
{
	std::ifstream infile(filename);
	
	// finds prefix for mtl file
	size_t lastSlashPos = std::string(filename).find_last_of('/');
	std::string prefix = std::string(filename).substr(0, lastSlashPos);

	std::string name;
	std::string ambient_fil  = "";
	std::string specular_fil = "";
	std::string diffuse_fil  = "";
	RGBA ambient_color  = RGBA(1.0f,1.0f,1.0f,1.0f);
	RGBA diffuse_color  = RGBA(1.0f,1.0f,1.0f,1.0f);
	RGBA specular_color = RGBA(1.0f,1.0f,1.0f,1.0f);
	float shininess;

	bool currently_parsing = false;
	for (std::string line; std::getline(infile, line);) {
		std::vector<std::string> tokens = m_split(line, " ");

		if (tokens.at(0) == "newmtl") {
			// fail if token size is less than 2
			if (currently_parsing) {

				// Material(const Texture &ambient, const Texture &specular, const Texture &diffuse, float shininess, const std::string &name);

				std::cout << "LOAD MTL WITH NAME " << name << std::endl;
				mtls.emplace_back(
					Texture(ambient_fil, ambient_fil == "", ambient_color),
					Texture(specular_fil, specular_fil == "", specular_color),
					Texture(diffuse_fil, diffuse_fil == "", diffuse_color),
					shininess,
					name
				);
			}

			name = tokens.at(1);
			currently_parsing = true;
		}
		else if (tokens.at(0) == "Kd") {
			diffuse_color = RGBA(stof(tokens.at(1)) * 255, stof(tokens.at(2)) * 255, stof(tokens.at(3)) * 255, 255.0f);
		}
		else if (tokens.at(0) == "map_Kd") {
			diffuse_fil =  prefix + "/" + tokens.at(1);
		}
		else if (tokens.at(0) == "Ka") {
			ambient_color = RGBA(stof(tokens.at(1)) * 255, stof(tokens.at(2)) * 255, stof(tokens.at(3)) * 255, 255.0f);
		}
		else if (tokens.at(0) == "map_Ka") {
			ambient_fil =  prefix + "/" + tokens.at(1);
		}
		else if (tokens.at(0) == "Ks") {
			specular_color = RGBA(stof(tokens.at(1)) * 255, stof(tokens.at(2)) * 255, stof(tokens.at(3)) * 255, 255.0f);
		}
		else if (tokens.at(0) == "map_Ks") {
			specular_fil =  prefix + "/" + tokens.at(1);
		}
		else if (tokens.at(0) == "Ns") {
			shininess = stof(tokens.at(1));
		}
	}

	// emplace back the last mtl
	std::cout << "emplacing a material with mtl name " << name << std::endl;
	mtls.emplace_back(
		Texture(ambient_fil, ambient_fil == "", ambient_color),
		Texture(specular_fil, specular_fil == "", specular_color),
		Texture(diffuse_fil, diffuse_fil == "", diffuse_color),
		shininess,
		name
	);

	//success
	return 1;
}



// populate map with copies of material so they can be referenced by name.
void m_create_mappings(const std::vector<Material>& materials, std::unordered_map<std::string, int> &map) {
	for (int i = 0; i < materials.size(); i++) {
		map[materials.at(i).get_name()] = i;
	}
}

// creates a vertex given face indicies e.g 1/1/1
Vertex m_create_vertex_from_indicies(const std::string &indicies, const std::vector<glm::vec3> &vecs,const std::vector<glm::vec2> &uvs, const std::vector<glm::vec3> &norms) {
	std::vector<std::string> tokens = m_split(indicies, "/");

	glm::vec3 vec = vecs.at(std::stoi(tokens.at(0)) - 1);
	glm::vec2 tex = uvs.at(std::stoi(tokens.at(1)) - 1);
	glm::vec3 norm = norms.at(std::stoi(tokens.at(2)) - 1);

	return Vertex(vec,tex, norm);
}
Model::Model(std::string filename)
{
	std::unordered_map<std::string, int> name_to_index;

	std::vector<glm::vec3> vecs;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;

	std::vector<Vertex> verticies;
	bool parsing_faces = false;

	unsigned int curr_material_ref = -1;

	std::vector<Vertex> current_verticies; 

	//find prefix of obj file, this is the current dir the obj file is in
	size_t lastSlashPos = std::string(filename).find_last_of('/');
	std::string prefix = std::string(filename).substr(0, lastSlashPos);
	
	std::ifstream infile(filename.c_str());

	// check if the file is valid
//	assert(!infile);

	// iterate through each line in the obj and parse
	for (std::string line; std::getline(infile, line);) 
	{
		// make list of tokens form line splitting using space
		std::vector<std::string> tokens = m_split(line, " ");
		// get first token in line, this is the type of information stored in the line
		std::string &line_type = tokens.at(0);

		if (line_type == "mtllib") {
			// mtl_string is a path to the mtl file
			std::string mtl_string = (prefix + "/" + tokens.at(1));
			char* mtlname = &mtl_string[0];
			m_mtl_parse(mtlname, _materials);
			
			// map names  to material objects
			m_create_mappings(_materials, name_to_index);
		}
		else if (line_type == "v") {
			vecs.emplace_back(std::stof(tokens.at(1)),std::stof(tokens.at(2)),std::stof(tokens.at(3)));
		}
		else if (line_type == "vn") {
			norms.emplace_back(std::stof(tokens.at(1)),std::stof(tokens.at(2)),std::stof(tokens.at(3)));
			continue;
		}
		else if (line_type == "vt") {
			// value of uvs is inversed so textures are the right way up.
			uvs.emplace_back(1 - std::stof(tokens.at(1)), 1 - std::stof(tokens.at(2)));
		}
		else if (line_type == "usemtl") {
			if (parsing_faces) {
				// if accessing a new object put previous object into list of objects
				assert(curr_material_ref != -1);
				_meshes.emplace_back(current_verticies, curr_material_ref);
			}

			// create new object with material retrieved from hashmap using name as key
			curr_material_ref = name_to_index[tokens.at(1)];
			parsing_faces = true;
		}
		else if (line_type == "f") {
			Vertex v1 = m_create_vertex_from_indicies(tokens.at(1), vecs, uvs, norms);
			Vertex v2 = m_create_vertex_from_indicies(tokens.at(2), vecs, uvs, norms);
			Vertex v3 = m_create_vertex_from_indicies(tokens.at(3), vecs, uvs, norms);
			
			current_verticies.push_back(v1);
			current_verticies.push_back(v2);
			current_verticies.push_back(v3);
		}
	}

	// place the last object into the list of objects
	assert(curr_material_ref != -1);
	_meshes.emplace_back(current_verticies, curr_material_ref);
}

Model::~Model()
{
	for (Material& mat: _materials) {
		mat.deinit();
	}

	for (Mesh& mesh: _meshes) {
		mesh.deinit();
	}
}

void Model::draw(const Shader& shader)
{
	for (const Mesh& mesh : _meshes) {
		_materials.at(mesh.get_material_ref()).bind();
		mesh.draw();
	}
}
