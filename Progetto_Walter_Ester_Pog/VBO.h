#pragma once
#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GLAD/glad.h>

class VBO 
{
	public:
		unsigned int ID;
		VBO(float* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
};

#endif