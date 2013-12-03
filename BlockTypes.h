#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include <SDL_pixels.h>
#include <glm\glm.hpp>

namespace blockTypes{
	
extern SDL_Color red;
extern SDL_Color green;
extern SDL_Color blue;
extern SDL_Color cyan;

extern glm::vec3 color1;
extern glm::vec3 color2;
extern glm::vec3 color3;
extern glm::vec3 color4;
extern glm::vec3 color5;

extern void need_update();

// Forward declarations of draw and construct (these are used in program_run.h and program_construct.h).
void draw();
void construct();

struct TextLine{
	// A placeholder for the characters in the text line.
	std::string str;
	
	// Constructors
	TextLine();
	TextLine(const std::string& str);
	TextLine(int i);
	TextLine(char c);

	// Draw one line of text on coordinate x,y. I is the linenumber and number of lines in the current textarea. Color is the color is currently not used.
	void draw(float x, float y, size_t i, size_t max, const SDL_Color& color = red);
	
	// Appending the text line to the input string.
	void saveAsText(std::string& str);
};

// Forward declared functions for keyboard and mouse inputs.
void Keyboard(unsigned char key, int x, int y);
void Keyboard_Special(int key, int x, int y);
void mouseFunction(int button, int state, int x, int y);
void mouseMovement(int x, int y);
}