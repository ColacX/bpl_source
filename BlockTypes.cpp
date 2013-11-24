#define WIN32_LEAN_AND_MEAN
#include "BlockTypes.h"
#include <Windows.h>
#include <gl\glew.h>
#include <gl\glut.h>
#include <gl\gl.h>

#define GLUT_KEY_BACKSPACE  8
#define GLUT_KEY_ENTER     13
#define GLUT_KEY_ESC       27
#define GLUT_KEY_SPACE     32
#define GLUT_KEY_COMMA     44
#define GLUT_KEY_HYPHEN    45
#define GLUT_KEY_DOT       46
#define GLUT_KEY_DELETE   127

void bolls_draw_text(const std::string& text, float x, float y, const SDL_Color& color);
extern float font_char_width;
extern float font_char_height;



namespace blockTypes{
	
SDL_Color red   = {0xff, 0x00, 0x00, 0x00};
SDL_Color green = {0x00, 0xff, 0x00, 0x00};
SDL_Color blue  = {0x00, 0x00, 0xff, 0x00};
SDL_Color cyan  = {0x00, 0xff, 0xff, 0x00};



TextLine::TextLine(){
}
TextLine::TextLine(const std::string& str):str(str){
}
TextLine::TextLine(int i){
	char temp[16];
	sprintf_s(temp,16,"%i",i);
	str.assign(temp);
}
TextLine::TextLine(char c){
	str.push_back(c);
}

void TextLine::draw(float x, float y, const SDL_Color& color){
	//bolls_draw_text(str, x,y, color);

	SDL_Color c = color;

	char t[128];
	for(size_t i=0; i<str.size(); ++i){
		// Interpret the text, find out if there are any keywords etc, and set the correct color
		t[0] = str[i];
		t[1] = '\0';

		if( (t[0]>='a'&&t[0]<='z') || (t[0]>='A'&&t[0]<='Z'))
			c = red;
		else
			c = blue;

		bolls_draw_text(t, x + i*font_char_width, y, c);
	}
}
void TextLine::saveAsText(std::string& str){
	str.append(this->str);
}


struct TextArea{
	std::vector< TextLine > strings;

	float x;
	float y;

	int c, r;

	float width(){
		struct{ bool operator()(const TextLine& a, const TextLine& b){ return a.str.size() < b.str.size(); } } comparator;
		float maximum = std::max_element(strings.begin(), strings.end(), comparator)->str.length() * font_char_width;
		return max(maximum, font_char_width); // minimum textarea width is one char
	}
	float height(){
		return strings.size() * font_char_height;
	}

	void draw(){
		float rx = width();
		float ry = height();

		for(int r=0; r<int(strings.size()); ++r)
			strings[r].draw(x, y+r*font_char_height);

		int minC = min(c, int(strings[this->r].str.length()));
		TextLine('|').draw(x+minC*font_char_width-font_char_width/2, y+this->r*font_char_height, cyan);
	}
};


std::vector<TextArea> textArea;
TextArea* activeObject = NULL;

void construct(){
	textArea.push_back( TextArea() );
	textArea.push_back( TextArea() );
	activeObject = &textArea[0];

	textArea[0].strings.push_back(TextLine("#include <stdlib.h>"));
	textArea[0].strings.push_back(TextLine("#include <stdio.h>"));
	textArea[0].strings.push_back(TextLine("int square(int i);"));
	textArea[0].strings.push_back(TextLine("int main(int argc, char* argv[]){"));
	textArea[0].strings.push_back(TextLine("    printf(\"Hello world! %d\\n\", square(5));"));
	textArea[0].strings.push_back(TextLine("    system(\"PAUSE\");"));
	textArea[0].strings.push_back(TextLine("    return EXIT_SUCCESS;"));
	textArea[0].strings.push_back(TextLine("}"));
	textArea[0].x = 500;
	textArea[0].y = 200;
	textArea[0].c = 1;
	textArea[0].r = 1;

	textArea[1].strings.push_back(TextLine("int square(int i){"));
	textArea[1].strings.push_back(TextLine("    return i*i;"));
	textArea[1].strings.push_back(TextLine("}"));
	textArea[1].x = 700;
	textArea[1].y = 500;
	textArea[1].c = 1;
	textArea[1].r = 1;
}

void draw(){
	for(size_t i=0; i<textArea.size(); ++i)
		textArea[i].draw();
}




bool textureAreaKeyInput = true;

void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case GLUT_KEY_ESC:
		exit(0);
		break;
	}

	if(textureAreaKeyInput && activeObject != NULL){
		TextArea& ta = *activeObject;
		ta.c = min(ta.c, int(ta.strings[ta.r].str.length()));
		std::string availableChars = " ()[]{} +-*/%!\"#&|<>/?^~'.:,;\\";
		if( isalnum(key) || availableChars.find(key) != availableChars.npos ){
			ta.strings[ta.r].str.insert(ta.c, 1, key);
			ta.c++;
		}
		else if(key == GLUT_KEY_BACKSPACE){
			if(ta.c != 0 || ta.r != 0){
				if(ta.c==0){
					size_t oldRowsLength = ta.strings[ta.r-1].str.length();
					ta.strings[ta.r-1].str.append( ta.strings[ta.r].str );
					ta.strings.erase(ta.strings.begin()+ta.r);
					ta.c = oldRowsLength;
					ta.r--;
				}
				else{
					ta.strings[ta.r].str.erase(ta.c-1, 1);
					ta.c--;
				}
			}
		}
		else if(key == GLUT_KEY_DELETE){
			if(ta.r+1 != ta.strings.size() || ta.c != ta.strings[ta.r].str.length()){
				if(ta.c==ta.strings[ta.r].str.size()){
					ta.strings[ta.r].str.append( ta.strings[ta.r+1].str );
					ta.strings.erase(ta.strings.begin()+ta.r+1);
				}
				else
					ta.strings[ta.r].str.erase(ta.c, 1);
			}
		}
		else if(key == GLUT_KEY_ENTER){
			ta.strings.insert(ta.strings.begin()+ta.r+1, TextLine());
			ta.strings[ta.r+1].str.append( ta.strings[ta.r].str.begin() + ta.c, ta.strings[ta.r].str.end() );
			ta.strings[ta.r].str.erase( ta.strings[ta.r].str.begin() + ta.c, ta.strings[ta.r].str.end() );

			ta.r++;
			ta.c = 0;
		}
	}
}

void Keyboard_Special(int key, int x, int y){
	if(textureAreaKeyInput){
		TextArea& ta = *activeObject;
		switch(key){
		case GLUT_KEY_RIGHT:
			if(ta.c > int(ta.strings[ta.r].str.size()))
				ta.c = ta.strings[ta.r].str.size();
			if(ta.r+1 != ta.strings.size() || ta.c != ta.strings[ta.r].str.size()){
				ta.c++;
				if(ta.c > int(ta.strings[ta.r].str.length())){
					ta.c = 0;
					ta.r++;
				}
			}
			break;
		case GLUT_KEY_LEFT:
			if(ta.c > int(ta.strings[ta.r].str.size()))
				ta.c = ta.strings[ta.r].str.size();
			if(ta.r != 0 || ta.c != 0){
				ta.c--;
				if(ta.c == -1){
					ta.r--;
					ta.c = ta.strings[ta.r].str.length();
				}
			}
			break;
		case GLUT_KEY_UP:
			if(ta.r > 0)
				ta.r--;
			break;
		case GLUT_KEY_DOWN:
			if(ta.r < int(ta.strings.size())-1)
				ta.r++;
			break;
		case GLUT_KEY_HOME:
			ta.c = 0;
			break;
		case GLUT_KEY_END:
			ta.c = ta.strings[ta.r].str.size();
			break;
		}
	}

	// Always compile if F7 is pressed
	if(key == GLUT_KEY_F7){
		char fileName[32];
		strcpy(fileName, "Blocks/fileXXX.cpp");
		for(size_t i=0; i<textArea.size(); ++i){
			sprintf(fileName+11, "%03d.cpp", i);
			FILE* f = fopen(fileName, "wb");
			for(size_t r=0; r<textArea[i].strings.size(); ++r){
				// If you get a crash here: create a folder namned "Blocks" in project folder, (the same folder where you have the folder "bpl_source")
				fwrite(textArea[i].strings[r].str.c_str(), 1, textArea[i].strings[r].str.length(), f);
				fwrite("\r\n", 1, 2, f);
			}
			fclose(f);
		}

		system("gcc -Os -s -o Blocks/test.exe Blocks/*.cpp\n");
		system("del /S /F /Q Blocks\\*.cpp > NUL");
		system("Blocks\\test.exe");
	}
}

}