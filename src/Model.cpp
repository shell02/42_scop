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


Model::Model(std::string filename) : err(0), minX(0.0f), minY(0.0f), maxX(0.0f), maxY(0.0f), maxZ(0.0f), minZ(0.0f)
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

	std::map<int, unsigned int> indices;
	std::vector<Vertex> vertices;
	std::vector<MTL> textures;

	file.open(filename);

	if (file.is_open()) {
		
		while (getline(file, line)) {
			if (line.find('#') != std::string::npos)
				line.erase(line.find('#'));

			if (line.size()) {
				opt = getOpt(line);

				if (opt == "o") {
					if (vertices.size() != 0) {
						
						indices.insert(std::make_pair(idx++, faces.size()));
						objects.push_back(Mesh(vertices, indices, textures));
						
						faces.clear();
						vertices.clear();
						indices.clear();
						textures.clear();
					}
				}
				else if (opt == "mtllib") {
					texturesNames = parseMTL(line);
				}
				else if (opt == "v") {
					Vector3 pos = parseVec3(line);
					positions.push_back(pos);

					if (maxX < positions[positions.size() - 1][0])
						maxX = positions[positions.size() - 1][0];
					if (minX > positions[positions.size() - 1][0])
						minX = positions[positions.size() - 1][0];
					if (maxY < positions[positions.size() - 1][1])
						maxY = positions[positions.size() - 1][1];
					if (minY > positions[positions.size() - 1][1])
						minY = positions[positions.size() - 1][1];
					if (maxZ < positions[positions.size() - 1][2])
						maxZ = positions[positions.size() - 1][2];
					if (minZ > positions[positions.size() - 1][2])
						minZ = positions[positions.size() - 1][2];
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
					if (faces.size() != 0) {
						indices.insert(std::make_pair(idx++, faces.size()));
						faces.clear();
					}
					line = getArg(line);
					if (line.size() == 0) {
						gl_log_err("**ERROR** : No Material name provided\n");
						err = 1;
					}
					else
						textures.push_back(texturesNames.at(line));
				}
				else if (opt == "f") {
					parseToIndice(line, vertices, faces, positions, normals, coords);
				}
				else if (opt == "s" || opt == "g") {
					//pass
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
		indices.insert(std::make_pair(idx, faces.size()));

		if (textures.size() == 0)
			textures.push_back(MTL());

		objects.push_back(Mesh(vertices, indices, textures));

		float centerX = 0.0f;
		float centerY = 0.0f;
		float centerZ = 0.0f;
		
		centerX = (maxX + minX) / 2;
		centerY = (maxY + minY) / 2;
		centerZ = (maxZ + minZ) / 2;

		for (size_t i = 0; i < objects.size(); i++) {
			objects[i].normalizeV(maxX, maxY, maxZ, minX, minY, minZ);
			objects[i].center(-centerX, -centerY, -centerZ);
		}
	}

}

Model::~Model()
{
	objects.clear();
}

void Model::draw(Shader const &program)
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].draw(program);
}

void Model::setTexture(bool isTextured)
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].setIsTextured(isTextured);
}

void Model::setRotation(int axis)
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].setRotation(axis);
}

void Model::setMove(int axis, float move)
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].setMove(axis, move);
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
	bool diff = false;

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

					if (textures.size() != 0 && diff == false) {
						textures[material].setDiff(textures[material].getAmbient());
					}
					material = getArg(line);
					textures.insert(std::make_pair(material, tmp));
					// textures[material].setIsTextured(true);


				}
				else if (opt == "Ka") {
					textures[material].setAmbient(parseVec3(line));
				}
				else if (opt == "Kd") {
					textures[material].setDiff(parseVec3(line));
					diff = true;
				}
				else if (opt == "Ks") {
					textures[material].setSpec(parseVec3(line));
				}
				else if (opt == "Ns") {
					textures[material].setShiny(toFloat(rmOpt(line)));
				}
				else if (opt == "d" || opt == "tr") {
					textures[material].setTr(toFloat(rmOpt(line)));
				}
				else if (opt == "map_Kd" || opt == "map_Ka") {
					textures[material].setDiffuseTex(Texture(dirPath + rmOpt(line)));
					// textures[material].setIsTextured(true);
				}
				else if (opt == "map_Ns" || opt == "map_Ks") {
					textures[material].setSpecTex(Texture(dirPath + rmOpt(line)));
				}
				else if (opt == "illum" || opt == "Ke" || opt == "Ni") {
					//pass
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

void Model::parseToIndice(std::string line, std::vector<Vertex>& vertices, std::vector<unsigned int>& faces, std::vector<Vector3> &positions, std::vector<Vector3> &normals, std::vector<Vector3> &coords)
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
		err = 1;
		return ;
	}

	float min_x = positions[v1[0] - 1][0];
	float min_y = positions[v1[0] - 1][1];
	float min_z = positions[v1[0] - 1][2];
	float max_x = positions[v1[0] - 1][0];
	float max_y = positions[v1[0] - 1][1];
	float max_z = positions[v1[0] - 1][2];

	if (min_x > positions[v2[0] - 1][0])
		min_x = positions[v2[0] - 1][0];
	if (max_x < positions[v2[0] - 1][0])
		max_x = positions[v2[0] - 1][0];
	if (min_y > positions[v2[0] - 1][1])
		min_y = positions[v2[0] - 1][1];
	if (max_y < positions[v2[0] - 1][1])
		max_y = positions[v2[0] - 1][1];
	if (min_z > positions[v2[0] - 1][2])
		min_z = positions[v2[0] - 1][2];
	if (max_z < positions[v2[0] - 1][2])
		max_z = positions[v2[0] - 1][2];

	if (min_x > positions[v3[0] - 1][0])
		min_x = positions[v3[0] - 1][0];
	if (max_x < positions[v3[0] - 1][0])
		max_x = positions[v3[0] - 1][0];
	if (min_y > positions[v3[0] - 1][1])
		min_y = positions[v3[0] - 1][1];
	if (max_y < positions[v3[0] - 1][1])
		max_y = positions[v3[0] - 1][1];
	if (min_z > positions[v3[0] - 1][2])
		min_z = positions[v3[0] - 1][2];
	if (max_z < positions[v3[0] - 1][2])
		max_z = positions[v3[0] - 1][2];

	if (face4.size()) {
		if (min_x > positions[v4[0] - 1][0])
			min_x = positions[v4[0] - 1][0];
		if (max_x < positions[v4[0] - 1][0])
			max_x = positions[v4[0] - 1][0];
		if (min_y > positions[v4[0] - 1][1])
			min_y = positions[v4[0] - 1][1];
		if (max_y < positions[v4[0] - 1][1])
			max_y = positions[v4[0] - 1][1];
		if (min_z > positions[v4[0] - 1][2])
			min_z = positions[v4[0] - 1][2];
		if (max_z < positions[v4[0] - 1][2])
			max_z = positions[v4[0] - 1][2];
	}

	// std::cout << "Max_x: " << max_x << " Min_x: " << min_x << " Max_y: " << max_y << " Min_y: " << min_y << std::endl;

	Vector3 norm = positions[v2[0] - 1].subVector(positions[v1[0] - 1]).crossVector(positions[v3[0] - 1].subVector(positions[v1[0] - 1]));
	norm = norm.unit();
	
	Vertex tmp;
	
	tmp.Position = positions[v1[0] - 1];
	if (v1[1] == 0 || v1[1] > coords.size()) {
		float tmpX;
		if (max_x - min_x != 0)
			tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
		else
			tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
		float tmpY;
		if (max_y - min_y != 0)
			tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
		else
			tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

		// if (!tmpX)
		// 	tmpX = 0.0f;
		// if (!tmpY)
		// 	tmpY = 0.0f;
		Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
		tmp.TexCoords[0] = tex[1];
		tmp.TexCoords[1] = tex[0];
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
		float tmpX;
		if (max_x - min_x != 0)
			tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
		else
			tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
		float tmpY;
		if (max_y - min_y != 0)
			tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
		else
			tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

		// if (!tmpX)
		// 	tmpX = 0.0f;
		// if (!tmpY)
		// 	tmpY = 0.0f;
		Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
		tmp.TexCoords[0] = tex[1];
		tmp.TexCoords[1] = tex[0];
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
		float tmpX;
		if (max_x - min_x != 0)
			tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
		else
			tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
		float tmpY;
		if (max_y - min_y != 0)
			tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
		else
			tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

		// if (!tmpX)
		// 	tmpX = 0.0f;
		// if (!tmpY)
		// 	tmpY = 0.0f;
		Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
		tmp.TexCoords[0] = tex[1];
		tmp.TexCoords[1] = tex[0];
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
			float tmpX;
			if (max_x - min_x != 0)
				tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
			else
				tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
			float tmpY;
			if (max_y - min_y != 0)
				tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
			else
				tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

			if (!tmpX)
				tmpX = 0.0f;
			if (!tmpY)
				tmpY = 0.0f;
			Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
			tmp.TexCoords[0] = tex[1];
			tmp.TexCoords[1] = tex[0];
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
			float tmpX;
			if (max_x - min_x != 0)
				tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
			else
				tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
			float tmpY;
			if (max_y - min_y != 0)
				tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
			else
				tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

			if (!tmpX)
				tmpX = 0.0f;
			if (!tmpY)
				tmpY = 0.0f;
			Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
			tmp.TexCoords[0] = tex[1];
			tmp.TexCoords[1] = tex[0];
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
			float tmpX;
			if (max_x - min_x != 0)
				tmpX = (tmp.Position[0] - min_x) / (max_x - min_x);
			else
				tmpX = (tmp.Position[2] - min_z) / (max_z - min_z);
			float tmpY;
			if (max_y - min_y != 0)
				tmpY = (tmp.Position[1] - min_y) / (max_y - min_y);
			else
				tmpY = (tmp.Position[2] - min_z) / (max_z - min_z);

			if (!tmpX)
				tmpX = 0.0f;
			if (!tmpY)
				tmpY = 0.0f;
			Vector3 tex = Vector3(tmpX, tmpY, 0.0f);
			tmp.TexCoords[0] = tex[1];
			tmp.TexCoords[1] = tex[0];
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
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << (tmp.TexCoords[0] == 0.0f) << ", " << tmp.TexCoords[1] << std::endl;
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
