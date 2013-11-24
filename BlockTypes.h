#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include <SDL_pixels.h>

namespace blockTypes{
	
extern SDL_Color red;
extern SDL_Color green;
extern SDL_Color blue;
extern SDL_Color cyan;

void draw();
void construct();

struct TextLine{
	std::string str;
	
	TextLine();
	TextLine(const std::string& str);
	TextLine(int i);
	TextLine(char c);

	void draw(float x, float y, const SDL_Color& color = red);

	void saveAsText(std::string& str);
};


void Keyboard(unsigned char key, int x, int y);
void Keyboard_Special(int key, int x, int y);
}