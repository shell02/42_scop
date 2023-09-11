#include "Model.hpp"

bool isFloat(const std::string& line) {
	bool dot = false;
	size_t i = line[0] == '-' ? 1 : 0;
	for (; i < line.size(); i++) {
		if (line[i] == '.' && dot == true)
			return false;
		if (line[i] == '.')
			dot = true;
		if (!isdigit(line[i]) && line[i] != '.') {
			return false;
        }
	}
	return true;
}

float Model::toFloat(std::string line) {
	float num = 0.0f;

	if (!isFloat(line)) {
		err = 1;
	}
	std::stringstream ss(line);
	ss >> num;
	return num;
}

bool isInt(const std::string& line) {
	size_t i = line[0] == '-' ? 1 : 0;
	for (; i < line.size(); i++) {
		if (!isdigit(line[i])) {
			return false;
        }
	}
	return true;
}

int Model::toInt(std::string line) {
	int num = 0;

	if (!isInt(line)) {
		err = 1;
	}
	std::stringstream ss(line);
	ss >> num;
	return num;
}

float max(float a, float b) {
	return a > b ? a : b;
}

float min(float a, float b) {
	return a < b ? a : b;
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
	bool Tex = true;

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector3> coords;
	std::map<std::string, MTL> texturesNames;
	unsigned int faces = 0;

	int idx = 0;

	objects.push_back(Mesh());

	file.open(filename);

	if (file.is_open()) {
		
		while (getline(file, line)) {
			idx++;
			if (line.find('#') != std::string::npos)
				line.erase(line.find('#'));

			if (line.size()) {
				opt = getOpt(line);
				if (opt.size() == 0) {
					err = 1;
					gl_log_err("**ERROR1**: Parsing error in file %s: line %d: %s\n", filename.c_str(), idx, line.c_str());
					break ;
				}

				if (opt == "o") {
					if (objects[objects.size() - 1].vertices.size() != 0) {
						
						objects[objects.size() - 1].indices.push_back(faces);
						objects[objects.size() - 1].setTex(Tex);
						objects.push_back(Mesh());
						
						faces = 0;
						Tex = true;
					}
				}
				else if (opt == "mtllib") {
					texturesNames = parseMTL(line);
				}
				else if (opt == "v") {
					Vector3 pos = parseVec3(line);
					positions.push_back(pos);

					maxX = max(maxX, positions[positions.size() - 1][0]);
					minX = min(minX, positions[positions.size() - 1][0]);
					maxY = max(maxY, positions[positions.size() - 1][1]);
					minY = min(minY, positions[positions.size() - 1][1]);
					maxZ = max(maxZ, positions[positions.size() - 1][2]);
					minZ = min(minZ, positions[positions.size() - 1][2]);
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
					if (faces != 0) {
						objects[objects.size() - 1].indices.push_back(faces);
						faces = 0;
					}
					line = getArg(line, 1);
					if (line.size() == 0) {
						gl_log_err("**ERROR** : No Material name provided\n");
						err = 1;
					}
					else {
						if (texturesNames.find(line) == texturesNames.end()) {
							gl_log_err("**ERROR** : Material name does not exist : line %s\n", line.c_str());
							err = 1;
						}
						else
							objects[objects.size() - 1].textures.push_back(texturesNames.at(line));
					}
				}
				else if (opt == "f") {
					bool tmp  = parseToIndice(line, objects[objects.size() - 1].vertices, &faces, positions, normals, coords);
					if (tmp == false)
						Tex = false;
				}
				else if (opt == "s" || opt == "g") {
					//pass
				}
				else {
					gl_log_err("**ERROR2**: Parsing error in file %s: line %d: %s\n", filename.c_str(), idx, line.c_str());
					err = 1;
					return ;
				}

				if (err == 1) {
					gl_log_err("**ERROR3**: Parsing error in file %s: line %d: %s\n", filename.c_str(), idx, line.c_str());
					return ;
				}
			}
		}
		file.close();
		objects[objects.size() - 1].indices.push_back(faces);

		if (objects[objects.size() - 1].textures.size() == 0)
			objects[objects.size() - 1].textures.push_back(MTL());

		// objects.push_back(Mesh(vertices, indices, textures));
		objects[objects.size() - 1].setTex(Tex);


		for (size_t i = 0; i < objects.size(); i++) {
			objects[i].normalizeV(maxX, maxY, maxZ, minX, minY, minZ);
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

void Model::clear()
{
	for(unsigned int i = 0; i < objects.size(); i++)
        objects[i].clear();
}

std::vector<Mesh> Model::getObjects() const
{
	return objects;
}

int Model::checkFile(std::string filename)
{
	std::string line = filename.substr(filename.find_last_of("."));

	if (line != ".obj") {
		gl_log_err("**ERROR**: Please provide a file ending in .obj\n");
		return 0;
	}

	size_t last = filename.find_last_of('/');
	if (last != std::string::npos) {
		dirPath = filename.substr(0, last + 1);
	}
	else
		dirPath = "./";
	return 1;
}

std::string Model::getArg(std::string line, int offset) {
	
	for (int i = 0; i < offset; i++) {
		if (!rmOpt(line)) {
			return "";
		}
	}
	size_t pos = line.find(" ");

	if (pos != std::string::npos)
		line.erase(pos);
	return line;
}

int Model::rmOpt(std::string &line)
{
	std::string opt = getOpt(line);
	if (opt.size() == 0)
		return 0;

	line.erase(0, line.find(opt) + opt.size());
	size_t pos = line.find_first_not_of(" ");

	if (pos != std::string::npos)
		line.erase(0, pos);
	else
		return 0;
	return 1;
}

std::string Model::getOpt(std::string line)
{
	size_t pos = line.find_first_not_of(" ");

	if (pos != std::string::npos)
		line.erase(0, pos);
	else
		return "";
	pos = line.find(" ");
	if (pos != std::string::npos)
		line.erase(pos);
	else
		return "";
	return line;
}

std::map<std::string, MTL> Model::parseMTL(std::string line)
{
	std::map<std::string, MTL> textures;
	
	line = getArg(line, 1);
	std::string filename = dirPath + line;
	std::fstream file;
	std::string opt;
	std::string material;
	bool diff = false;

	file.open(filename);
	if (file.is_open()) {
		
		while (getline(file, line)) {
			if (line.find('#') != std::string::npos)
				line.erase(line.find('#'));

			if (line.size()) {
				opt = getOpt(line);
				if (opt.size() == 0) {
					err = 1;
					gl_log_err("**ERROR4**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
					return textures ;
				}

				if (opt == "newmtl") {

					if (textures.size() != 0 && diff == false) {
						textures[material].setDiff(textures[material].getAmbient());
					}
					material = getArg(line, 1);
					textures.insert(std::make_pair(material, MTL()));

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
					textures[material].setShiny(toFloat(getArg(line, 1)));
				}
				else if (opt == "d") {
					textures[material].setTr(toFloat(getArg(line, 1)));
				}
				else if (opt == "tr") {
					textures[material].setTr(1.0f - toFloat(getArg(line, 1)));
				}
				else if (opt == "map_Kd" || opt == "map_Ka") {
					Texture tex(dirPath + getArg(line, 1));
					if (tex.getErr()) {
						err = 1;
						return textures;
					}
					textures[material].setDiffuseTex(tex);
				}
				else if (opt == "map_Ns" || opt == "map_Ks") {
					Texture tex(dirPath + getArg(line, 1));
					if (tex.getErr()) {
						err = 1;
						return textures;
					}
					textures[material].setSpecTex(tex);
				}
				else if (opt == "illum" || opt == "Ke" || opt == "Ni") {
					//pass
				}
				else {
					gl_log_err("**ERROR5**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
					err = 1;
					return textures;
				}

				if (err == 1) {
					gl_log_err("**ERROR6**: Parsing error in file %s: line: %s\n", filename.c_str(), line.c_str());
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
	std::string l1 = getArg(line, 1);
	std::string l2 = getArg(line, 2);
	std::string l3 = getArg(line, 3);
	
	if (l1.size() == 0 || l2.size() == 0 || l3.size() == 0) {
		err = 1;
	}

	v1 = toFloat(l1);
	v2 = toFloat(l2);
	v3 = toFloat(l3);

	return Vector3(v1, v2, v3);
}

Vector3 Model::parseTexC(std::string line)
{
	float v1;
	float v2;
	std::string l1 = getArg(line, 1);
	std::string l2 = getArg(line, 2);

	if (l1.size() == 0 || l2.size() == 0) {
		err = 1;
	}

	v1 = toFloat(l1);
	v2 = toFloat(l2);

	return Vector3(v1, v2, 0.0f);
}

bool checkfaces(std::string line) {
	if (!isdigit(line[0]))
		return false;
	size_t i = 0;
	while (isdigit(line[i]))
		i++;
	if (i == line.size())
		return true;
	if (line[i++] != '/')
		return false;
	while (isdigit(line[i]))
		i++;
	if (line[i] != '/' && i != line.size())
		return false;
	if (i == line.size())
		return true;
	i++;
	while (isdigit(line[i]))
		i++;
	if (i != line.size())
		return false;
	return true;
}

int toFaces(std::string line) {
	int num = 0;

	std::stringstream ss(line);
	ss >> num;
	return num;
}

Vector3 Model::getFace(std::string line) {
	if (!checkfaces(line)) {
		err = 1;
		return Vector3(-1.0f, -1.0f, -1.0f);
	}
	
	int v1;
	int v2;
	int v3;
	size_t pos = line.find("/");
	size_t last = line.find_last_of("/");

	if (pos == std::string::npos) {
		v1 = toInt(line);
		v2 = -1;
		v3 = -1;
	}
	else if (pos == last) {
		v1 = toFaces(line.substr(0, line.find('/')));
		v2 = toFaces(line.substr(line.find('/') + 1));
		v3 = -1;
	}
	else if (pos + 1 == last) {
		v1 = toFaces(line.substr(0, line.find('/')));
		v2 = -1;
		v3 = toFaces(line.substr(line.find_last_of('/') + 1));
	}
	else {
		v1 = toFaces(line.substr(0, line.find('/')));
		v2 = toFaces(line.substr(line.find('/') + 1, line.find_last_of('/')));
		v3 = toFaces(line.substr(line.find_last_of('/') + 1));
	}

	return Vector3(v1, v2, v3);
}

bool Model::parseToIndice(std::string line, std::vector<Vertex>& vertices, unsigned int* faces, std::vector<Vector3> &positions, std::vector<Vector3> &normals, std::vector<Vector3> &coords)
{
	std::string face1 = getArg(line, 1);
	std::string face2 = getArg(line, 2);
	std::string face3 = getArg(line, 3);
	if (face1.size() == 0 || face2.size() == 0 || face3.size() == 0) {
		err = 1;
		return false;
	}

	std::string face4;
	if (getArg(line, 4) != "") {
		face4 = getArg(line, 4);

		if (face4.size() == 0) {
			err = 1;
			return false;
		}
		if (getArg(line, 5) != "") {
			gl_log_err("**ERROR**: Faces with 5+ vertices not supported\n");
			err = 1;
			return false;
		}
	}

	Vector3 v1 = getFace(face1);
	Vector3 v2 = getFace(face2);
	Vector3 v3 = getFace(face3);
	Vector3 v4;
	if (face4.size())
		v4 = getFace(face4);

	bool Tex = (coords.size() ? true : false);


	if (v1[0] == 0 || v2[0] == 0 || v3[0] == 0 || (face4.size() && v4[0] == 0)) {
		gl_log_err("**ERROR**: Vertex Position can't be 0\n");
		err = 1;
		return Tex;
	}
	if (v1[0] <= -1 || v2[0] <= -1 || v3[0] <= -1 || (face4.size() && v4[0] <= -1)) {
		gl_log_err("**ERROR**: Negative Vertex Position not supported\n");
		err = 1;
		return Tex;
	}
	if (v1[0] > positions.size() ||
		v2[0] > positions.size() ||
		v3[0] > positions.size() ||
		(face4.size() && v4[0] > positions.size())) {
		gl_log_err("**ERROR**: Vertex Position over the number of vertices declared\n");
		err = 1;
		return Tex;
	}


	if (v1[1] == 0 || v2[1] == 0 || v3[1] == 0 || (face4.size() && v4[1] == 0)) {
		gl_log_err("**ERROR**: Vertex Texture can't be 0\n");
		err = 1;
		return Tex;
	}
	if (v1[1] == -1 || v2[1] == -1 || v3[1] == -1 || (face4.size() && v4[1] == -1)) {
		Tex = false;
	}
	if (coords.size() && (v1[1] > coords.size() ||
		v2[1] > coords.size() ||
		v3[1] > coords.size() ||
		(face4.size() && v4[1] > coords.size()))) {
		gl_log_err("**ERROR**: Vertex Texture over the number of vertices declared\n");
		err = 1;
		return Tex;
	}

	if (v1[2] == 0 || v2[2] == 0 || v3[2] == 0 || (face4.size() && v4[2] == 0)) {
		gl_log_err("**ERROR**: Vertex Normal can't be 0\n");
		err = 1;
		return Tex;
	}
	if (normals.size() && (v1[2] > normals.size() ||
		v2[2] > normals.size() ||
		v3[2] > normals.size() ||
		(face4.size() && v4[2] > normals.size()))) {
		gl_log_err("**ERROR**: Vertex Normal over the number of vertices declared\n");
		err = 1;
		return Tex;
	}

	Vector3 norm = positions[v2[0] - 1].subVector(positions[v1[0] - 1]).crossVector(positions[v3[0] - 1].subVector(positions[v1[0] - 1]));
	norm = norm.unit();
	
	Vertex tmp;
	
	tmp.Position = positions[v1[0] - 1];
	if (v1[1] == -1 || v1[1] > coords.size()) {
		tmp.TexCoords[0] = 0;
		tmp.TexCoords[1] = 0;
	} else {
		tmp.TexCoords[0] = coords[v1[1] - 1][0];
		tmp.TexCoords[1] = coords[v1[1] - 1][1];
	}
	if (v1[2] == -1 || v1[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v1[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;

	tmp.Position = positions[v2[0] - 1];
	if (v2[1] == -1 || v2[1] > coords.size()) {
		tmp.TexCoords[0] = 0;
		tmp.TexCoords[1] = 0;
	} else {
		tmp.TexCoords[0] = coords[v2[1] - 1][0];
		tmp.TexCoords[1] = coords[v2[1] - 1][1];
	}
	if (v2[2] == -1 || v2[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v2[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;
	
	tmp.Position = positions[v3[0] - 1];
	if (v3[1] == -1 || v3[1] > coords.size()) {
		tmp.TexCoords[0] = 0;
		tmp.TexCoords[1] = 0;
	} else {
		tmp.TexCoords[0] = coords[v3[1] - 1][0];
		tmp.TexCoords[1] = coords[v3[1] - 1][1];
	}
	if (v3[2] == -1 || v3[2] > normals.size()) {
		tmp.Normal = norm;
	} else {
		tmp.Normal = normals[v3[2] - 1];
	}
	vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << tmp.TexCoords[0] << ", " << tmp.TexCoords[1] << std::endl;

	if (face4.size()) {
		
		tmp.Position = positions[v3[0] - 1];
		if (v3[1] == -1 || v3[1] > coords.size()) {
			tmp.TexCoords[0] = 0;
			tmp.TexCoords[1] = 0;
		} else {
			tmp.TexCoords[0] = coords[v3[1] - 1][0];
			tmp.TexCoords[1] = coords[v3[1] - 1][1];
		}
		if (v3[2] == -1 || v3[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v3[2] - 1];
		}
		vertices.push_back(tmp);

		tmp.Position = positions[v4[0] - 1];
		if (v4[1] == -1 || v4[1] > coords.size()) {
			tmp.TexCoords[0] = 0;
			tmp.TexCoords[1] = 0;
		} else {
			tmp.TexCoords[0] = coords[v4[1] - 1][0];
			tmp.TexCoords[1] = coords[v4[1] - 1][1];
		}
		if (v4[2] == -1 || v4[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v4[2] - 1];
		}
		vertices.push_back(tmp);

		tmp.Position = positions[v1[0] - 1];
		if (v1[1] == -1 || v1[1] > coords.size()) {
			tmp.TexCoords[0] = 0;
			tmp.TexCoords[1] = 0;
		} else {
			tmp.TexCoords[0] = coords[v1[1] - 1][0];
			tmp.TexCoords[1] = coords[v1[1] - 1][1];
		}
		if (v1[2] == -1 || v1[2] > normals.size()) {
			tmp.Normal = norm;
		} else {
			tmp.Normal = normals[v1[2] - 1];
		}
		vertices.push_back(tmp);
	// std::cout << tmp.Position[0] << ", " << tmp.Position[1] << ", " << tmp.Position[2] << ", " << tmp.Normal[0] << ", " << tmp.Normal[1] << ", " << tmp.Normal[2] << ", " << (tmp.TexCoords[0] == 0.0f) << ", " << tmp.TexCoords[1] << std::endl;
	}

	*faces += 3;
	if (face4.size()) {
		*faces += 3;
	}
	return Tex;
}
