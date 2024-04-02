#pragma once
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <errno.h>
#include <fstream>
#include <unordered_map>
#include <vector>

class vec3
{
public:
	float x, y, z;

	vec3() {}
	vec3(float l, float m, float r)
	{
		x = l;
		y = m;
		z = r;
	}
	~vec3() {}
};

class vec2
{
public:
	float x, y;

	vec2() {}
	vec2(float l, float r)
	{ 
		x = l;
		y = r;
	}
	~vec2() {}
};

struct oldVertex
{
public:
	vec3 vc;
	vec3 tc;

	oldVertex() {}
	oldVertex(vec3 vc_in, vec3 tc_in)
	{
		vc = vc_in;
		tc = tc_in;
	}
	~oldVertex() {}
};
struct triangle
{
public:
	oldVertex verts[3];

	triangle() {}
	triangle(oldVertex v0, oldVertex v1, oldVertex v2)
	{
		verts[0] = v0;
		verts[1] = v1;
		verts[2] = v2;
	}
	~triangle() {}
};

class OldMaterial
{
public:
	char mtl_name[256];
	char fil_name[256];
	vec3 ambient_color;
	vec3 defuse_color;
	vec3 specular_color;
	double disolve;


	OldMaterial() {}
	OldMaterial(char* n, char* f, vec3 ambient_color, vec3 defuse_color, vec3 specular_color, double disolve) :
		ambient_color(ambient_color), defuse_color(defuse_color), specular_color(specular_color), disolve(disolve)
	{
		strcpy(mtl_name, n); 
		strcpy(fil_name, f);
	}

	~OldMaterial() 
	{
	}
};

class OldObject
{
public:
	unsigned int VAO;
	unsigned int VBO;
	std::vector<triangle> tris;
	OldMaterial mtl;
	Texture texture;


	OldObject() {}
	OldObject(OldMaterial m)
	{
		strcpy(mtl.fil_name, m.fil_name);
		strcpy(mtl.mtl_name, m.mtl_name);
		this->mtl.defuse_color = m.defuse_color;
	}
	~OldObject()
	{
		texture.deinit();
	}
};

// remove instances of tab character '\t' from a string
void removeTabs(std::string& str) {
    size_t pos;
    while ((pos = str.find('\t')) != std::string::npos) {
        str.erase(pos, 1); 
    }
}


// splits a string into a vector of strings (tokens) based on a delimiter
std::vector<std::string> split(std::string s, std::string delim) {
    size_t start = 0; 
	size_t end; 
	size_t len = delim.length();
    std::string token;
    std::vector<std::string> res;

    while ((end = s.find(delim, start)) != std::string::npos) {
        token = s.substr(start, end - start);
		removeTabs(token);
        start = end + len;
		if (token == "") {
			continue;
		}
        res.push_back(token);
    }

    res.push_back(s.substr (start));
    return res;
}

int mtl_parse(char* filename, std::vector<OldMaterial> *mtls)
{
	std::ifstream infile(filename);

	// if file not found return 0 (failure)
	if (!infile) {
		std::cout << "REFERENCED OBJECT FILE : " << filename << " NOT FOUND\n";
		return 0;
	}
	
	// finds prefix for mtl file
	size_t lastSlashPos = std::string(filename).find_last_of('/');
	std::string prefix = std::string(filename).substr(0, lastSlashPos);

	char name[256] = "";
	char img[256] = "";
	vec3 ambient_color;
	vec3 defuse_color;
	vec3 specular_color;
	double disolve;
	bool currently_parsing = false;
	for (std::string line; std::getline(infile, line);) {
		std::vector<std::string> tokens = split(line, " ");

		if (tokens.at(0) == "newmtl") {
			// fail if token size is less than 2
			if (!currently_parsing) {
				currently_parsing = true;
				std::string mtl_name = tokens.at(1);
				strncpy(name, mtl_name.c_str(), sizeof(name));
				continue;
			}


			std::cout << "emplacing a material with mtl name " << name << std::endl;
			mtls->emplace_back(name, img,ambient_color, defuse_color, specular_color, disolve);

			std::string mtl_name = tokens.at(1);
			strncpy(name, mtl_name.c_str(), sizeof(name));
		}
		else if (tokens.at(0) == "Kd") {
			defuse_color = vec3(stof(tokens.at(1)), stof(tokens.at(2)), stof(tokens.at(3)));
		}
		else if (tokens.at(0) == "map_Kd") {

			std::string img_file_path = prefix + "/" + tokens.at(1);
			strncpy(img, img_file_path.c_str(), sizeof(name));
		}
	}

	// emplace back the last mtl
	std::cout << "emplacing a material with mtl name " << name << std::endl;
	mtls->emplace_back(name, img,ambient_color, defuse_color, specular_color, disolve);

	//success
	return 1;
}



// populate map with copies of material so they can be referenced by name.
void create_mappings(const std::vector<OldMaterial>& materials, std::unordered_map<std::string, OldMaterial> &map) {
	for (OldMaterial m : materials) {
		map[m.mtl_name] = m;
	}
}

// creates a vertex given face indicies e.g 1/1/1
oldVertex create_vertex_from_indicies(const std::string &indicies, const std::vector<vec3> &vecs,const std::vector<vec3> &uvs) {
	std::vector<std::string> tokens = split(indicies, "/");

	vec3 vec = vecs.at(std::stoi(tokens.at(0)) - 1);
	vec3 uv2 = uvs.at(std::stoi(tokens.at(1)) - 1);
	vec3 uv = vec3(uv2.x, uv2.y, 0.0F);

	return oldVertex(vec,uv);
}

int obj_parse(const char * filename, std::vector<OldObject> *objs)
{
	std::vector<OldMaterial> materials;
	std::unordered_map<std::string, OldMaterial> name_to_mat;
	std::vector<vec3> vecs;
	std::vector<vec3> uvs;
	OldObject obj;
	bool parsing_faces = false;

	//find prefix of obj file, this is the current dir the obj file is in
	size_t lastSlashPos = std::string(filename).find_last_of('/');
	std::string prefix = std::string(filename).substr(0, lastSlashPos);
	
	std::ifstream infile(filename);

	// check if the file is valid
	if (!infile) {
		printf("[ERROR] when parsing obj %s, file not found", filename);
		return 0;
	}

	// iterate through each line in the obj and parse
	for (std::string line; std::getline(infile, line);) 
	{
		// make list of tokens form line splitting using space
		std::vector<std::string> tokens = split(line, " ");
		// get first token in line, this is the type of information stored in the line
		std::string &line_type = tokens.at(0);

		if (line_type == "mtllib") {
			// mtl_string is a path to the mtl file
			std::string mtl_string = (prefix + "/" + tokens.at(1));
			char* mtlname = &mtl_string[0];
			
			// if mtl_parse fails return 0 (failure)
			if (!mtl_parse(mtlname, &materials)) {
				return 0;
			}

			// map names  to material objects
			create_mappings(materials, name_to_mat);
		}
		else if (line_type == "v") {
			vecs.emplace_back(std::stof(tokens.at(1)),std::stof(tokens.at(2)),std::stof(tokens.at(3)));
		}
		else if (line_type == "vn") {
			// ignore normals
			continue;
		}
		else if (line_type == "vt") {
			// value of uvs is inversed so textures are the right way up.
			uvs.emplace_back(1 - std::stof(tokens.at(1)), 1 - std::stof(tokens.at(2)),0.0F);
		}
		else if (line_type == "usemtl") {
			if (parsing_faces) {
				// if accessing a new object put previous object into list of objects
				objs->push_back(obj);
			}

			std::cout << "CREATING NEW OBJECT FROM MTL " << tokens.at(1) << "\n";
			// create new object with material retrieved from hashmap using name as key
			obj = OldObject(name_to_mat[tokens.at(1)]);
			parsing_faces = true;
		}
		else if (line_type == "f") {
			oldVertex v1 = create_vertex_from_indicies(tokens.at(1), vecs, uvs);
			oldVertex v2 = create_vertex_from_indicies(tokens.at(2), vecs, uvs);
			oldVertex v3 = create_vertex_from_indicies(tokens.at(3), vecs, uvs);
			
			obj.tris.emplace_back(v1, v2, v3);
		}
	}

	// place the last object into the list of objects
	objs->push_back(obj);
	return 1;
}

