#pragma once

//DEFINIZIONI
#define _USE_MATH_DEFINES

//AVVIO
#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>


//NAMESPACE
using namespace std;


//VETTORI
#include "Vec3F.h"
#include "Vec4F.h"


//MATRICI
#include "Mat4F.h"


//QUATERNIONI
#include "Quaternion.h"

//LIBRERIE

#include <KHR/khrplatform.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <RAPIDXML/rapidxml.hpp>
#include <RAPIDXML/rapidxml_iterators.hpp>
#include <RAPIDXML/rapidxml_print.hpp>
#include <RAPIDXML/rapidxml_utils.hpp>

#include "stb_image.h"

//CLASSI AGGIUNTIVE
#include "Shader_Class.cpp"
#include "Texture_Class.cpp"
#include "Camera_Class.cpp"
