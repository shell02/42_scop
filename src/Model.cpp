#include "Model.hpp"

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

	Mesh obj;
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
					// if (obj.vertices.size() != 0) {
					// 	indices.insert(std::make_pair(faces.size(), faces));
					// 	faces.clear();
					// 	for (std::map<int, std::vector<unsigned int>>::iterator it = indices.begin(); it != indices.end(); it++) {
					// 		std::vector<Vertex> tmp = fillVertices(positions, normals, coords, it->second);
					// 		vertices.insert(vertices.end(), tmp.begin(), tmp.end());
					// 	}
					// 	objects.push_back(Mesh(vertices, indices, textures));
					// }
				}
				else if (opt == "mtllib") {
					texturesNames = parseMTL(line);
					std::cout << "Opening mtllib file" << std::endl;
				}
				else if (opt == "v") {
					Vector3 pos = parsePos(line);
					positions.push_back(pos);
				}
				else if (opt == "vn") {
					Vector3 norm = parseNorm(line);
					normals.push_back(norm);
				}
				else if (opt == "vt") {
					Vector3 tex = parseTexC(line);
					coords.push_back(tex);
				}
				else if (opt == "usemtl") {
					faces.clear();
					// if (faces.size() != 0) {
					// 	indices.insert(std::make_pair(faces.size(), faces));
					// 	faces.clear();
					// }
					// line.erase(0, line.find("usemtl ") + 7);
					// if (line.size() == 0) {
					// 	err = 1;
					// 	break ;
					// }
					// textures.push_back(texturesNames.at(line));
				}
				else if (opt == "f") {
					parseToIndice(line, faces);
				}
				else if (opt == "s" || opt == "g") {
					gl_log_err("**INFO**: option %s not supported\n", opt.c_str());
				}
			}
		}
		file.close();
		std::cout << "End of File: Number of positions: " << positions.size() << std::endl;
		std::cout << "End of File: Number of normals: " << normals.size() << std::endl;
		std::cout << "End of File: Number of coords: " << coords.size() << std::endl;
		std::cout << "End of File: Number of faces: " << faces.size() << std::endl;
		
		// indices.insert(std::make_pair(faces.size(), faces));
		faces.clear();
		// for (std::map<int, std::vector<unsigned int>>::iterator it = indices.begin(); it != indices.end(); it++) {
		// 	std::vector<Vertex> tmp = fillVertices(positions, normals, coords, it->second);
		// 	vertices.insert(vertices.end(), tmp.begin(), tmp.end());
		// }
		// objects.push_back(Mesh(vertices, indices, textures));
	}

	


	for (int i = 0; i < 36; i++) {
		Vertex tmp;

		tmp.Position = Vector3(example[i * 8], example[i * 8 + 1], example[i * 8 + 2]);
		// tmp.Position[0] = example[i * 8];
		// tmp.Position[1] = example[i * 8 + 1];
		// tmp.Position[2] = example[i * 8 + 2];
		tmp.Normal = Vector3(example[i * 8 + 3], example[i * 8 + 4], example[i * 8 + 5]);
		// tmp.Normal[0] = example[i * 8 + 3];
		// tmp.Normal[1] = example[i * 8 + 4];
		// tmp.Normal[2] = example[i * 8 + 5];
		tmp.TexCoords[0] = example[i * 8 + 6];
		tmp.TexCoords[1] = example[i * 8 + 7];
		vertices.push_back(Vertex(tmp));
	}


	// for (int i = 0; i != 0; i++) {
	// 	unsigned int indices;
	// }

	MTL tex;
	tex.setDiffuseTex(Texture("./assets/container2.png"));
	tex.setSpecTex(Texture("./assets/container2_specular.png"));
	tex.setIsTextured(true);
	textures.push_back(tex);

	objects.push_back(Mesh(vertices, indices, textures));
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

	file.open(filename);
	if (file.is_open()) {
		size_t last = filename.find_last_of('/') + 1;

		if (last != std::string::npos) {
			dirPath = filename.substr(0, last);
		}
		else
			dirPath = "./";
		file.close();
		return 1;
	}
	else {
		gl_log_err("**ERROR** : Could not open %s\n", filename);
		return 0;
	}
}

std::string Model::getOpt(std::string line)
{
	size_t pos = line.find_first_not_of(" ");

	if (pos != std::string::npos)
		line.erase(0, pos);
	pos = line.find(" ");
	if (pos != std::string::npos)
		line.erase(pos);
	return line;
}

std::map<std::string, MTL> Model::parseMTL(std::string line)
{
	std::map<std::string, MTL> textures;
	
	line.erase(0, line.find(" ") + 1);
	std::string filename = dirPath + line;
	std::fstream file;

	file.open(filename);
	if (file.is_open()) {
		file.close();
	}
	else {
		gl_log_err("**ERROR** : Could not open Material File %s\n", filename.c_str());
		err = 1;
	}
	return textures;
}

Vector3 Model::parsePos(std::string line)
{
	(void)line;
	Vector3 pos;
	return pos;

}

Vector3 Model::parseNorm(std::string line)
{
	(void)line;
	Vector3 norm;
	return norm;
}

Vector3 Model::parseTexC(std::string line)
{
	(void)line;
	Vector3 tex;
	return tex;
}

void Model::parseToIndice(std::string line, std::vector<unsigned int>& faces)
{
	(void)line;
	faces.push_back(1);
}

std::vector<Vertex> Model::fillVertices(std::vector<Vector3> positions, std::vector<Vector3> normals, std::vector<float[2]> coords, std::vector<unsigned int> faces)
{
	std::vector<Vertex> vertices;
		(void)positions;
		(void)normals;
		(void)coords;
		(void)faces;
	return vertices;
}
