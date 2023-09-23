#include "MTL.hpp"


MTL::MTL() : diffuse(Texture("./assets/kittens.jpg", true)), specular(Texture("./assets/white_spec.png"))
{
	ambient = Vector3(0.8f, 0.8f, 0.8f);
	diff = Vector3(0.8f, 0.8f, 0.8f);
	spec = Vector3(0.5f, 0.5f, 0.5f);
	shininess = 32.0f;
	transparency = 1.0f;

	isTextured = false;
}

void MTL::setDiffuseTex(Texture diffuse)
{
	// unsigned int id = this->diffuse.getID();
	// glDeleteTextures(1, &id);
	this->diffuse.clear();
	this->diffuse = diffuse;
}

void MTL::setSpecTex(Texture specular)
{
	// unsigned int id = this->specular.getID();
	// glDeleteTextures(1, &id);
	this->specular.clear();
	this->specular = specular;
}

void MTL::setAmbient(Vector3 a)
{
	ambient = a;
}

void MTL::setDiff(Vector3 d)
{
	diff = d;
}

void MTL::setSpec(Vector3 spec)
{
	this->spec = spec;
}

void MTL::setShiny(float s)
{
	shininess = s;
}

void MTL::setTr(float tr)
{
	transparency = tr;
}

void MTL::setIsTextured(bool b)
{
	isTextured = b;
}

void MTL::bindDiff() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse.getID());
	// std::cout << "Diffuse id: " << diffuse.getID() << std::endl;
}

void MTL::bindSpec() const
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular.getID());
	// std::cout << "Diffuse id: " << specular.getID() << std::endl;
}

Texture MTL::getDiffuse() const
{
	return diffuse;
}

Texture MTL::getSpecular() const
{
	return specular;
}

Vector3 MTL::getAmbient() const
{
	return ambient;
}

Vector3 MTL::getDiff() const
{
	return diff;
}

Vector3 MTL::getSpec() const
{
	return spec;
}

float MTL::getShiny() const
{
	return shininess;
}

float MTL::getTr() const
{
	return transparency;
}

bool MTL::getIsTextured() const
{
	return isTextured;
}

void MTL::clear()
{
	this->diffuse.clear();
	this->specular.clear();
}

