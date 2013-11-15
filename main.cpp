//linker settings and standard librarys-----------------------------------------
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#include <GL\glew.h>
#include <gl\glut.h>

#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#undef main

#include <SDL_image.h>
#pragma comment(lib, "SDL2_image.lib")

#include <SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")

//global switch and macros-------------------------------------------------------
#define trigger_breakpoint __debugbreak()

//global variables---------------------------------------------------------------
//temporary buffer, use carefully
#define t_size 1920*720*4*4
static union
{
	unsigned char c[t_size/sizeof(unsigned char)];
	int i[t_size/sizeof(int)];
	float f[t_size/sizeof(float)];
	double d[t_size/sizeof(double)];
	long l[t_size/sizeof(long)];
} t;

int window_width = 1280;
int window_height = 720;
GLuint shader_program_text;

//custom header files------------------------------------------------------------
#include "library.h"
#include "text_area.h"
#include "water_effect.h"
#include "program_run.h"
#include "program_callbacks.h"
#include "program_construct.h"

void main(int argc, char** argv)
{
	program_construct(argc, argv);
}
