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

MTL::MTL(MTL const& copy) : diffuse(copy.diffuse), specular(copy.specular)
{
	this->ambient = copy.ambient;
	this->diff = copy.diff;
	this->spec = copy.spec;
	this->shininess = copy.shininess;
	this->transparency = copy.transparency;
	this->isTextured = copy.isTextured;
}

void MTL::setDiffuseTex(Texture diffuse)
{
	this->diffuse = diffuse;
}

void MTL::setSpecTex(Texture specular)
{
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

