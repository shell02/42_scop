#include "Model.hpp"

float toFloat(std::string line) {
	float num;

	std::stringstream ss(line);
	ss >> num;
	return num;
}

int toInt(std::string line) {
	int num = 0;

	std::stringstream ss(line);
	ss >> num;
	return num;
}

float example[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

Model::Model(std::string filename) : err(0)
{
	if (!checkFile(filename)) {
		err = 1;
		return ;
	}

	std::fstream file;
	std::string line;
	std::string opt;
	int idx = 0;

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector3> coords;
	std::map<std::string, MTL> texturesNames;
	std::vector<unsigned int> faces;

	std::map<int, std::vector<unsigned int>> indices;
	std::vector<Vertex> vertices;
	std::vector<MTL> textures;

	file.open(filename);

	if (file.is_open()) {
		
		while (getline(file, line)) {
			// std::cout << line << std::endl;
			if (line.find('#') != std::string::npos)
				line.erase(line.find('#'));

			if (line.size()) {
				opt = getOpt(line);

				if (opt == "o") {
					// std::cout << "New Object" << std::endl;
					if (vertices.size() != 0) {
						indices.insert(std::make_pair(idx++, faces));
						objects.push_back(Mesh(vertices, indices, textures));
						faces.clear();
						positions.clear();
						coords.clear();
						normals.clear();
						vertices.clear();
						indices.clear();
						textures.clear();
					}
				}
				else if (opt == "mtllib") {
					texturesNames = parseMTL(line);
					for (std::map<std::string, MTL>::iterator it = texturesNames.begin(); it != texturesNames.end(); it++) {
						// std::cout << it->first << ":\n";
						// std::cout << "Ambient color: " << it->second.getAmbient()[0] << " " << it->second.getAmbient()[1] << " " << it->second.getAmbient()[2] << std::endl;
						// std::cout << "Diffuse color: " << it->second.getDiff()[0] << " " << it->second.getDiff()[1] << " " << it->second.getDiff()[2] << std::endl;
						// std::cout << "Spec color: " << it->second.getSpec()[0] << " " << it->second.getSpec()[1] << " " << it->second.getSpec()[2] << std::endl;
						// std::cout << "Shininess: " << it->second.getShiny() << std::endl;
						// std::cout << "Transparency: " << it->second.getTr() << std::endl;
						// std::cout << "IsTextured: " << it->second.getIsTextured() << std::endl;
						// std::cout << "Tex Diff: " << it->second.getDiffuse().getID() << std::endl;
						// std::cout << "Tex Spec: " << it->second.getSpecular().getID() << std::endl;
					}
				}
				else if (opt == "v") {
					Vector3 pos = parseVec3(line);
					positions.push_back(pos);
				}
				else if (opt == "vn") {
					Vector3 norm = parseVec3(line);
					normals.push_back(norm);
				}
				else if (opt == "vt") {
					Vector3 tex = parseTexC(line);
					coords.push_back(tex);
				}
				else if (opt == "usemtl") {
					// std::cout << "Texture: " << getArg(line) << std::endl;
					if (texturesNames.find(line) != texturesNames.end())
						std::cout << "Found\n";
					// faces.clear();
					if (faces.size() != 0) {
						indices.insert(std::make_pair(idx++, faces));
						faces.clear();
					}
					line = getArg(line);
					if (line.size() == 0) {
						err = 1;
						break ;
					}
					textures.push_back(texturesNames.at(line));
				}
				else if (opt == "f") {
					parseToIndice(line, vertices, faces, positions, normals, coords);
				}
				else if (opt == "s" || opt == "g") {
					gl_log_err("**INFO**: option %s not supported\n", opt.c_str());
				}
				else {
					gl_log_err("**ERROR**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
					err = 1;
					return ;
				}

				if (err == 1) {
					gl_log_err("**ERROR**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
					return ;
				}
			}
		}
		file.close();
		std::cout << "End of File: Number of vertices: " << vertices.size() << std::endl;
		// std::cout << "End of File: Number of normals: " << indices.size() << std::endl;
		// std::cout << "End of File: Number of coords: " << textures.size() << std::endl;
		// std::cout << "End of File: Number of faces: " << faces.size() << std::endl;
		indices.insert(std::make_pair(idx, faces));
		objects.push_back(Mesh(vertices, indices, textures));
		vertices.clear();
	}

	


	// for (int i = 0; i < 36; i++) {
	// 	Vertex tmp;

	// 	tmp.Position = Vector3(example[i * 8], example[i * 8 + 1], example[i * 8 + 2]);
	// 	// tmp.Position[0] = example[i * 8];
	// 	// tmp.Position[1] = example[i * 8 + 1];
	// 	// tmp.Position[2] = example[i * 8 + 2];
	// 	tmp.Normal = Vector3(example[i * 8 + 3], example[i * 8 + 4], example[i * 8 + 5]);
	// 	// tmp.Normal[0] = example[i * 8 + 3];
	// 	// tmp.Normal[1] = example[i * 8 + 4];
	// 	// tmp.Normal[2] = example[i * 8 + 5];
	// 	tmp.TexCoords[0] = example[i * 8 + 6];
	// 	tmp.TexCoords[1] = example[i * 8 + 7];
	// 	vertices.push_back(Vertex(tmp));
	// }


	// // for (int i = 0; i != 0; i++) {
	// // 	unsigned int indices;
	// // }

	// MTL tex;
	// tex.setDiffuseTex(Texture("./assets/container2.png"));
	// tex.setSpecTex(Texture("./assets/container2_specular.png"));
	// tex.setIsTextured(true);
	// textures.push_back(tex);

	// objects.push_back(Mesh(vertices, indices, textures));
}

void Model::draw(Shader const &program)
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].draw(program);
}

int Model::getErr() const
{
	return err;
}

std::vector<Mesh> Model::getObjects() const
{
	return objects;
}

int Model::checkFile(std::string filename)
{
	std::fstream file;
	std::string line = filename.substr(filename.find_last_of("."));

	if (line != ".obj") {
		gl_log_err("**ERROR**: Please provide a file ending in .obj\n");
		return 0;
	}

	file.open(filename);
	if (file.is_open()) {
		size_t last = filename.find_last_of('/');

		if (last != std::string::npos) {
			dirPath = filename.substr(0, last + 1);
		}
		else
			dirPath = "./";
		file.close();
		return 1;
	}
	else {
		gl_log_err("**ERROR** : Could not open %s\n", filename.c_str());
		return 0;
	}
}

std::string Model::getArg(std::string line) {
	line = rmOpt(line);
	size_t pos = line.find(" ");

	if (pos != std::string::npos)
		line.erase(pos);

	if (line.size() == 0)
		err = 1;
	return line;
}

std::string Model::rmOpt(std::string line)
{
	size_t pos = line.find_first_not_of(" ");

	if (pos != std::string::npos)
		line.erase(0, pos);
	pos = line.find(" ");
	if (pos != std::string::npos)
		line.erase(0, pos);
	pos = line.find_first_not_of(" ");
	if (pos != std::string::npos)
		line.erase(0, pos);
	return line;
}

std::string Model::getOpt(std::string line)
{
	size_t pos = line.find_first_not_of(" ");

	if (pos != std::string::npos)
		line.erase(0, pos);
	pos = line.find(" ");
	if (pos != std::string::npos)
		line.erase(pos);
	else
		return 0;
	return line;
}

std::map<std::string, MTL> Model::parseMTL(std::string line)
{
	std::map<std::string, MTL> textures;
	
	line = getArg(line);
	std::string filename = dirPath + line;
	std::fstream file;
	std::string opt;
	std::string material;

	file.open(filename);
	if (file.is_open()) {
		
		while (getline(file, line)) {
			// std::cout << line << std::endl;
			if (line.find('#') != std::string::npos)
				line.erase(line.find('#'));

			if (line.size()) {
				opt = getOpt(line);

				if (opt == "newmtl") {
					MTL tmp;

					material = getArg(line);
					textures.insert(std::make_pair(material, tmp));
					std::cout << "New material: " << material << std::endl;
					textures[material].setIsTextured(true);

				}
				else if (opt == "Ka") {
					textures[material].setAmbient(parseVec3(rmOpt(line)));
				}
				else if (opt == "Kd") {
					textures[material].setDiff(parseVec3(rmOpt(line)));
				}
				else if (opt == "Ks") {
					textures[material].setSpec(parseVec3(rmOpt(line)));
				}
				else if (opt == "Ns") {
					textures[material].setShiny(toFloat(rmOpt(line)));
				}
				else if (opt == "d" || opt == "tr") {
					textures[material].setTr(toFloat(rmOpt(line)));
				}
				else if (opt == "map_Kd" || opt == "map_Ka") {
					textures[material].setDiffuseTex(Texture(rmOpt(line)));
					textures[material].setIsTextured(true);
				}
				else if (opt == "map_Ns") {
					textures[material].setSpecTex(Texture(rmOpt(line)));
				}
				else if (opt == "illum" || opt == "Ke" || opt == "Ni") {
					gl_log_err("**INFO**: option %s not supported\n", opt.c_str());
				}
				else {
					gl_log_err("**ERROR**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
					err = 1;
					return textures;
				}
			}
		}
		file.close();
	}
	else {
		gl_log_err("**ERROR** : Could not open Material File %s\n", filename.c_str());
		err = 1;
	}
	return textures;
}

Vector3 Model::parseVec3(std::string line)
{
	float v1;
	float v2;
	float v3;

	v1 = toFloat(getArg(line));
	line = rmOpt(line);
	v2 = toFloat(getArg(line));
	line = rmOpt(line);
	v3 = toFloat(getArg(line));

	return Vector3(v1, v2, v3);

}

Vector3 Model::parseTexC(std::string line)
{
	float v1;
	float v2;

	v1 = toFloat(getArg(line));
	line = rmOpt(line);
	v2 = toFloat(getArg(line));

	return Vector3(v1, v2, 0.0f);
}

Vector3 Model::getFace(std::string line) {
	if (line.size() == 0)
		return Vector3(0.0f, 0.0f, 0.0f);
	
	int v1;
	int v2;
	int v3;
	size_t pos = line.find("/");
	size_t last = line.find_last_of("/");

	if (pos == std::string::npos) {
		v1 = toInt(line);
		v2 = 0;
		v3 = 0;
	}
	else if (pos == last) {
		v1 = toInt(line.substr(0, line.find('/')));
		v2 = toInt(line.substr(line.find('/') + 1));
		v3 = 0;
	}
	else {
		v1 = toInt(line.substr(0, line.find('/')));
		v2 = toInt(line.substr(line.find('/') + 1, line.find_last_of('/')));
		v3 = toInt(line.substr(line.find_last_of('/') + 1));
	}

	return Vector3(v1, v2, v3);
}

void Model::parseToIndice(std::string line, std::vector<Vertex>& vertices, std::vector<unsigned int>& faces, std::vector<Vector3> positions, std::vector<Vector3> normals, std::vector<Vector3> coords)
{
	std::string face1 = getArg(line);
	line = rmOpt(line);
	std::string face2 = getArg(line);
	line = rmOpt(line);
	std::string face3 = getArg(line);
	line = rmOpt(line);
	std::string face4;
	if (line.find(" ") != std::string::npos && line.find_first_not_of(" ", line.find(" ")) != std::string::npos) {
		face4 = getArg(line);
		line = rmOpt(line);
		if (line.find(" ") != std::string::npos && line.find_first_not_of(" ", line.find(" ")) != std::string::npos) {
			gl_log_err("**ERROR**: Faces with 5 vertices not supported\n");
			err = 1;
			return ;
		}
	}

	// std::cout << "Faces : " << face1 << " " << face2 << " " << face3 << " " << face4 << std::endl;

	Vector3 v1 = getFace(face1);
	Vector3 v2 = getFace(face2);
	Vector3 v3 = getFace(face3);
	Vector3 v4 = getFace(face4);

	if (v1[0] == 0 || v2[0] == 0 || v3[0] == 0 || (face4.size() && v4[0] == 0)) {
		gl_log_err("**ERROR**: Vertex Position can't be 0\n");
		err = 1;
		return ;
	}

	if (v1[0] > positions.size() ||
		v2[0] > positions.size() ||
		v3[0] > positions.size() ||
		(face4.size() && v4[0] > positions.size())) {
		gl_log_err("**ERROR**: Vertex position over the number of vertices declared\n");
		std::cout << positions.max_size() << std::endl;
		err = 1;
		return ;
	}
	Vector3 norm = v2.subVector(v1).crossVector(v3.subVector(v1));
	norm = norm.unit();
	
	Vertex tmp;
	
	tmp.Position = positions[v1[0] - 1];
	if (v1[1] == 0 || v1[1] > coords.size()) {
		Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
		tmp.TexCoords[0] = tex[0];
		tmp.TexCoords[1] = tex[1];
	} else {
		tmp.TexCoords[0] = coords[v1[1] - 1][0];
		tmp.TexCoords[1] = coords[v1[1] - 1][1];
	}
	if (v1[2] == 0 || v1[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v1[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;

	tmp.Position = positions[v2[0] - 1];
	if (v2[1] == 0 || v2[1] > coords.size()) {
		Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
		tmp.TexCoords[0] = tex[0];
		tmp.TexCoords[1] = tex[1];
	} else {
		tmp.TexCoords[0] = coords[v2[1] - 1][0];
		tmp.TexCoords[1] = coords[v2[1] - 1][1];
	}
	if (v2[2] == 0 || v2[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v2[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;
	
	tmp.Position = positions[v3[0] - 1];
	if (v3[1] == 0 || v3[1] > coords.size()) {
		Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
		tmp.TexCoords[0] = tex[0];
		tmp.TexCoords[1] = tex[1];
	} else {
		tmp.TexCoords[0] = coords[v3[1] - 1][0];
		tmp.TexCoords[1] = coords[v3[1] - 1][1];
	}
	if (v3[2] == 0 || v3[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v3[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;

	if (face4.size()) {
		
		tmp.Position = positions[v3[0] - 1];
		if (v3[1] == 0 || v3[1] > coords.size()) {
			Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
			tmp.TexCoords[0] = tex[0];
			tmp.TexCoords[1] = tex[1];
		} else {
			tmp.TexCoords[0] = coords[v3[1] - 1][0];
			tmp.TexCoords[1] = coords[v3[1] - 1][1];
		}
		if (v3[2] == 0 || v3[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v3[2] - 1];
		}
		vertices.push_back(tmp);

		tmp.Position = positions[v4[0] - 1];
		if (v4[1] == 0 || v4[1] > coords.size()) {
			Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
			tmp.TexCoords[0] = tex[0];
			tmp.TexCoords[1] = tex[1];
		} else {
			tmp.TexCoords[0] = coords[v4[1] - 1][0];
			tmp.TexCoords[1] = coords[v4[1] - 1][1];
		}
		if (v4[2] == 0 || v4[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v4[2] - 1];
		}
		vertices.push_back(tmp);

		tmp.Position = positions[v1[0] - 1];
		if (v1[1] == 0 || v1[1] > coords.size()) {
			Vector3 tex = Vector3(tmp.Position[0], tmp.Position[1], 0.0f).unit();
			tmp.TexCoords[0] = tex[0];
			tmp.TexCoords[1] = tex[1];
		} else {
			tmp.TexCoords[0] = coords[v1[1] - 1][0];
			tmp.TexCoords[1] = coords[v1[1] - 1][1];
		}
		if (v1[2] == 0 || v1[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v1[2] - 1];
		}
		vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;
	}

	faces.push_back(toInt(face1));
	faces.push_back(toInt(face2));
	faces.push_back(toInt(face3));
	if (face4.size()) {
		faces.push_back(toInt(face3));
		faces.push_back(toInt(face4));
		faces.push_back(toInt(face1));
	}
}
