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
extern GLuint CreateShaderProgram(const char* vert_source = 0, const char* frag_source = 0, const char* geom_source = 0);

namespace blockTypes{
	
SDL_Color red   = {0xff, 0x00, 0x00, 0x00};
SDL_Color green = {0x00, 0xff, 0x00, 0x00};
SDL_Color blue  = {0x00, 0x00, 0xff, 0x00};
SDL_Color cyan  = {0x00, 0xff, 0xff, 0x00};


int text_width = 1280;
int text_height = 720;
GLuint text_buffer_texture;
GLuint text_frame_buffer;
GLuint block_type_shader;

void compile();


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

	SDL_Color c = green;

	if( str.find(" if(") != std::string::npos || str.find(" else") != std::string::npos || str.find(" while(") != std::string::npos || str.find(" for(") != std::string::npos )
		c = red;
	else if( str.size()==1 && str[0]=='|' ) // if cursor
		c = cyan;
	else if(str.size() && str[0]!=' ') // if no spaces
		c = blue;

	char t[128];
	for(size_t i=0; i<str.size(); ++i){
		// Interpret the text, find out if there are any keywords etc, and set the correct color
		t[0] = str[i];
		t[1] = '\0';

		if(t[0] != ' '){ // If we don't send spaces, we get a speed boost of ~31%
			bolls_draw_text(t, x + i*font_char_width, y, c);
		}
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

		float scrollingY = r * font_char_height;

		for(int r=0; r<int(strings.size()); ++r)
			strings[r].draw(x, y+r*font_char_height - scrollingY);

		int minC = min(c, int(strings[this->r].str.length()));
		TextLine('|').draw(x+minC*font_char_width-font_char_width/2, y+this->r*font_char_height - scrollingY);
	}
};



std::vector<TextArea> textArea;
TextArea* activeObject = NULL;

void update_draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, text_frame_buffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i<textArea.size(); ++i)
		textArea[i].draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//doesnt swap the buffers, thats why theres a freeze when pressing  down a key
}

void construct(){
	textArea.push_back( TextArea() );
	textArea.push_back( TextArea() );
	textArea.push_back( TextArea() );
	activeObject = &textArea[0];

	textArea[0].strings.push_back(TextLine("$RaytracerEngine"));
	textArea[0].strings.push_back(TextLine("#antialiasing = true"));
	textArea[0].strings.push_back(TextLine("#fullscreen = true"));
	textArea[0].x = 0;
	textArea[0].y = 200;
	textArea[0].c = 1;
	textArea[0].r = 1;

	textArea[1].strings.push_back(TextLine("$Constructor"));
	textArea[1].strings.push_back(TextLine(""));
	textArea[1].strings.push_back(TextLine("    printf(\"The raytracer is starting!\\n\");"));
	textArea[1].x = 0;
	textArea[1].y = 500;
	textArea[1].c = 1;
	textArea[1].r = 1;

	textArea[2].strings.push_back(TextLine("$Raytracer"));
	//textArea[2].strings.push_back(TextLine("void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);}"));
	//textArea[2].strings.push_back(TextLine(""));

	FILE* defaultFrag = fopen("bpl_binary\\CodeLibrary\\RaytracerEngine_defaultFrag", "rb");

	textArea[2].strings.push_back(TextLine(""));
	while( 1 ){
		char c;
		fread(&c, 1, 1, defaultFrag);
		if(feof(defaultFrag))
			break;
		if(c==13){
			fread(&c, 1, 1, defaultFrag);
			textArea[2].strings.push_back(TextLine(""));
		}
		else{
			if(c=='\t')
				for(size_t t=0; t<4; ++t)
					textArea[2].strings.back().str.push_back(' ');
			else
				textArea[2].strings.back().str.push_back(c);
		}
	}


	textArea[2].x = 400;
	textArea[2].y = 300;
	textArea[2].c = 1;
	textArea[2].r = 1;

	block_type_shader = CreateShaderProgram("bpl_source/block_type_shader.glsl", "bpl_source/block_type_shader.glsl");

	{
		GLuint generated_texture;
		glGenTextures(1, &generated_texture);
		glBindTexture(GL_TEXTURE_2D, generated_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		GLfloat border_color[4] = { 0, 0, 0, 0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

		glTexImage2D(
			GL_TEXTURE_2D, //target
			0, //mipmap-level
			GL_RGBA, //texture format
			text_width, //texture width
			text_height, //texture height
			0, //this value must be 0? lol.
			GL_RED, //input data format
			GL_FLOAT, //input data element type
			0 //input data
		);

		glBindTexture(GL_TEXTURE_2D, 0);
		text_buffer_texture = generated_texture;
	}

	{
		//http://www.songho.ca/opengl/gl_fbo.html
		GLuint generated_frame_buffer;
		glGenFramebuffers(1, &generated_frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, generated_frame_buffer);

		//Attaching a 2D texture image to FBO
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, //GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER
			GL_COLOR_ATTACHMENT0, //GL_COLOR_ATTACHMENTi, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT
			GL_TEXTURE_2D, //specifies what type of texture is attached
			text_buffer_texture, //texture id
			0 //mip map level to attach
		);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw "error glCheckFramebufferStatus";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		text_frame_buffer = generated_frame_buffer;
	}

	update_draw();
}

void draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(block_type_shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text_buffer_texture);

	glBegin(GL_TRIANGLE_STRIP);
	//texcoord; position;
	glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, -1); //bottom left
	glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, -1); //bottom right
	glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, +1); //top left
	glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, +1); //top right
	glEnd();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

	glDisable(GL_BLEND);
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
		std::string availableChars = " ()[]{}=+-*/%!\"#&|<>/?^~'.:,;\\";
		if( isalnum(key) || availableChars.find(key) != availableChars.npos ){
			ta.strings[ta.r].str.insert(ta.c, 1, key);
			ta.c++;

			update_draw();
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

				update_draw();
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

				update_draw();
			}
		}
		else if(key == GLUT_KEY_ENTER){
			ta.strings.insert(ta.strings.begin()+ta.r+1, TextLine());
			ta.strings[ta.r+1].str.append( ta.strings[ta.r].str.begin() + ta.c, ta.strings[ta.r].str.end() );
			ta.strings[ta.r].str.erase( ta.strings[ta.r].str.begin() + ta.c, ta.strings[ta.r].str.end() );

			ta.r++;
			ta.c = 0;
			update_draw();
		}
	}
}

void Keyboard_Special(int key, int x, int y){
	if(textureAreaKeyInput){
		TextArea& ta = *activeObject;
		switch(key){
		case GLUT_KEY_RIGHT:
			if (ta.c > int(ta.strings[ta.r].str.size()))
			{
				ta.c = ta.strings[ta.r].str.size();
				update_draw();
			}

			if(ta.r+1 != ta.strings.size() || ta.c != ta.strings[ta.r].str.size()){
				ta.c++;
				if(ta.c > int(ta.strings[ta.r].str.length())){
					ta.c = 0;
					ta.r++;
				}
				update_draw();
			}
			break;
		case GLUT_KEY_LEFT:
			if (ta.c > int(ta.strings[ta.r].str.size()))
			{
				ta.c = ta.strings[ta.r].str.size();
				update_draw();
			}

			if(ta.r != 0 || ta.c != 0){
				ta.c--;
				if(ta.c == -1){
					ta.r--;
					ta.c = ta.strings[ta.r].str.length();
				}
				update_draw();
			}
			break;
		case GLUT_KEY_UP:
			if (ta.r > 0)
			{
				ta.r--;
				update_draw();
			}
			break;
		case GLUT_KEY_DOWN:
			if (ta.r < int(ta.strings.size()) - 1)
			{
				ta.r++;
				update_draw();
			}
			break;
		case GLUT_KEY_HOME:
			ta.c = 0;
			update_draw();
			break;
		case GLUT_KEY_END:
			ta.c = ta.strings[ta.r].str.size();
			update_draw();
			break;
		}
	}

	// Always compile if F7 is pressed
	if(key == GLUT_KEY_F7){
		compile();
	}
}

void mouseFunction(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		for(size_t i=0; i<textArea.size(); ++i){
			TextArea& ta = textArea[i];
			if(x >= ta.x && x < ta.x + ta.width() && y >= ta.y && y < ta.y + ta.height()){
				activeObject = &ta;
				break;
			}
		}
	}
}


void compile(){
	if(strcmp(textArea[0].strings[0].str.c_str(), "$RaytracerEngine")==0){
		system("del /S /F /Q Blocks\\*.cpp > NUL");

		auto directoryExists = [](const char path[]) -> BOOL{
			DWORD dwAttrib = GetFileAttributesA(path);
			return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
		};

		if( ! directoryExists("Blocks") )
			CreateDirectoryA("Blocks", NULL);
		system("copy /b bpl_binary\\CodeLibrary\\RaytracerEngine_fragHelper Blocks\\main.cpp > NUL");

		FILE* maindst = fopen("Blocks\\main.cpp", "ab");

		std::vector<size_t> tabs(textArea[2].strings.size(), 0);
		for(size_t i=0; i<textArea[2].strings.size(); ++i){
			size_t s;
			for(s=0; s<textArea[2].strings[i].str.size(); s++)
				if( textArea[2].strings[i].str[s] != ' ' )
					break;

			// Important to check is if there are more characters after the tab
			if(s != textArea[2].strings[i].str.size()) // there exist more characters
				tabs[i] = s;
			else if(i!=0)				// if there isn't more characters after the tabs, then it's an empty line,
				tabs[i] = tabs[i-1];	// so we use the same tabs as the line above.
			else
				tabs[i] = 0;			// If it's the first line, we can't take the same as above, so we use 0
		}

		for(size_t i=1; i<textArea[2].strings.size(); ++i){
			// do some parsing...

			// add braces '{' and '}' around function bodies and flow control statements
			if(tabs[i] > tabs[i-1]){
				//fwrite(textArea[2].strings[i].str.c_str(), 1, tabs[i], maindst);	// add spaces
				fwrite("\"{\\n\"\r\n", 1, 7, maindst);
			}
			else if(tabs[i] < tabs[i-1]){ // if there is less tabs now...
				for(size_t t=0; t<tabs[i-1]-tabs[i]; t+=4){
					//fwrite(textArea[2].strings[i].str.c_str(), 1, tabs[i-1]-t, maindst);	// add spaces
					fwrite("\"}\\n\"\r\n", 1, 7, maindst);
				}
			}
				
			fwrite("\"", 1, 1, maindst);

			// fix color
			std::string s = textArea[2].strings[i].str;
			size_t index;
			size_t start = 0;
			while ((index = s.find("color", start)) != std::string::npos){
				if(index==0 || !((s[index-1]>='a' && s[index-1]<='z') || (s[index-1]>='A' && s[index-1]<='Z') || (s[index-1]>='0' && s[index-1]<='9')))
					s.replace(index, 5, "vec3");
				start += 3;
			}

			fwrite(s.c_str(), 1, s.size(), maindst);

			// add semicolon
			bool isFunction;
			if(i != textArea[2].strings.size() && tabs[i]==0 && tabs[i+1]>0) // not last line
				isFunction = true;
			else
				isFunction = false;

			bool isControlStatement;
			if( s.find(" if(") != std::string::npos || s.find(" else") != std::string::npos || s.find(" while(") != std::string::npos || s.find(" for(") != std::string::npos )
				isControlStatement = true;
			else
				isControlStatement = false;

			if(isFunction==false && isControlStatement==false)
				fwrite(";", 1, 1, maindst);

			// Lineending: first a "\" and a "n" in the string, so comments in the shader is "closed".
			// Then we have a " to close the string, and after that a windows newline for a new line in the output textfile
			fwrite("\\n\"\r\n", 1, 5, maindst);
		}
		fwrite("\"}\"\r\n", 1, 5, maindst); // end brace

		fwrite(";\n", 1, 2, maindst);

		fclose(maindst);

		system("del /S /F /Q Blocks\\*.exe > NUL");
		system("type bpl_binary\\CodeLibrary\\RaytracerEngine_main.cpp >> Blocks\\main.cpp");
		system("gcc -Os -s -o Blocks\\test.exe Blocks\\*.cpp -lopengl32 -lglu32 -lgdi32"); //  -mwindows
		system("del /S /F /Q Blocks\\*.cpp > NUL");
		system("Blocks\\test.exe");
	}


	if(0){ // old version
		char fileName[32];
		strcpy(fileName, "Blocks\\fileXXX.cpp");
		for(size_t i=0; i<textArea.size(); ++i){
			sprintf(fileName+11, "%03d.cpp", i);
			FILE* f = fopen(fileName, "wb");
			for(size_t r=0; r<textArea[i].strings.size(); ++r){
				fwrite(textArea[i].strings[r].str.c_str(), 1, textArea[i].strings[r].str.length(), f);
				fwrite("\r\n", 1, 2, f);
			}
			fclose(f);
		}

		//system("gcc -Os -s -o Blocks/test.exe Blocks/*.cpp");
		system("gcc -Os -s -o Blocks\\test.exe Blocks\\*.cpp -lopengl32 -lglu32 -lgdi32");
		system("del /S /F /Q Blocks\\*.cpp > NUL");
		system("Blocks\\test.exe");
	}
}

}