#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GLAD/glad.h>
#include "VBO.h"

class VAO
{
public:
	unsigned int ID;
	VAO();

	void LinkVBO(VBO VBO, unsigned int layout);
	void Bind();
	void Unbind();
	void Delete();
};

#endif