#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Texture.hpp"
#include "Vector.hpp"

class MTL {
	public:
		MTL();
		MTL(MTL const& copy);

		void setDiffuseTex(Texture diffuse);
		void setSpecTex(Texture specular);

		void setAmbient(Vector3 a);
		void setDiff(Vector3 d);
		void setSpec(Vector3 spec);
		void setShiny(float s);
		void setTr(float tr);
		void setIsTextured(bool b);

		void bindDiff() const;
		void bindSpec() const;

		Texture getDiffuse() const;
		Texture getSpecular() const;

		Vector3 getAmbient() const;
		Vector3 getDiff() const;
		Vector3 getSpec() const;
		float getShiny() const;
		float getTr() const;
		bool getIsTextured() const;

	private:
		Texture diffuse;
		Texture specular;

		Vector3 ambient;
		Vector3 diff;
		Vector3 spec;
		float shininess;
		float transparency;

		bool isTextured;
};