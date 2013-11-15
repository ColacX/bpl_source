#pragma once

#ifdef WIN32
double windows_timer_get_time()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER performance_count;
	QueryPerformanceCounter(&performance_count); //get current tick
	return performance_count.QuadPart * (1000000.0 / frequency.QuadPart); //returns time in micro seconds
}

void test_windows_timer()
{
	double start_time = windows_timer_get_time();
	Sleep(500);
	double end_time = windows_timer_get_time();
	double elapsed_time = end_time - start_time;
	fprintf(stderr, "%lf(micro_seconds)\n", elapsed_time);
}
#endif

void ReadFile(const char* file_path, char*& buffer_pointer, unsigned int& buffer_capacity)
{
	std::ifstream file(file_path, std::ios::in | std::ios::binary | std::ios::ate);

	if(!file.is_open()){
		throw "is_open failed";
	}

	buffer_capacity = (unsigned int)file.tellg();
	buffer_pointer = new char[buffer_capacity+1];
	file.seekg(0, std::ios::beg);
	file.read(buffer_pointer, buffer_capacity);
	file.close();
	buffer_pointer[buffer_capacity] = 0;
}

void compile_shader(const char* file_path, std::vector<char*>& source_vector, GLuint shader_id)
{
	fprintf(stderr, "compiling %s ...\n", file_path);
	char* text_data;
	unsigned int text_dataSize;
	ReadFile(file_path, text_data, text_dataSize);
	source_vector.push_back(text_data);
	glShaderSource(shader_id, source_vector.size(), (const GLchar**)&source_vector[0], 0);
	glCompileShader(shader_id);
	int status = 0;
	glGetObjectParameterivARB(shader_id, GL_OBJECT_COMPILE_STATUS_ARB, &status);

	if(!status)
	{
		GLbyte info_log[1000];
		glGetInfoLogARB(shader_id, sizeof(info_log), 0, (GLcharARB*)info_log);
		fprintf(stderr, "info_log:\n%s\n", info_log);
		trigger_breakpoint;
		throw "glcompile_shader";
	}

	delete[] text_data;
}

GLuint CreateShaderProgram(const char* vert_source = 0, const char* frag_source = 0, const char* geom_source = 0)
{
	GLuint program_object = glCreateProgram();

	if(vert_source != 0)
	{
		GLuint shader_id = glCreateShader(GL_VERTEX_SHADER);
		std::vector<char*> source_vector;
		source_vector.push_back("#define __VERT__\n");
		compile_shader(vert_source, source_vector, shader_id);
		glAttachShader(program_object, shader_id);
	}

	if(geom_source != 0)
	{
		GLuint shader_id = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		std::vector<char*> source_vector;
		source_vector.push_back("#define __GEOM__\n");
		compile_shader(geom_source, source_vector, shader_id);
		glAttachShader(program_object, shader_id);
	}

	if(frag_source != 0)
	{
		GLuint shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		std::vector<char*> source_vector;
		source_vector.push_back("#define __FRAG__\n");
		compile_shader(frag_source, source_vector, shader_id);
		glAttachShader(program_object, shader_id);
	}

	glLinkProgram(program_object);
	return program_object;
}

GLuint LoadImageToTexture(const char* image_file_path)
{
	SDL_Surface* sdl_surface = IMG_Load(image_file_path); //can throw an odd error if something is wrong with the image

	if(sdl_surface == nullptr)
	{
		trigger_breakpoint;
		throw "IMG_Load";
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if(sdl_surface->format->BitsPerPixel == 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sdl_surface->w, sdl_surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, sdl_surface->pixels);
	else if(sdl_surface->format->BitsPerPixel == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdl_surface->w, sdl_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
	else
		throw "unsupported BitsPerPixel";

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(sdl_surface);
	return texture_id;
}

void VerticalSync(bool mode)
{
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(mode);
}

void test_ttf_sdl_opengl()
{
	//http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html
	//http://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_5.html#SEC5

	TTF_Font* text_font = TTF_OpenFont("bpl_binary/BrushScriptStd.otf", 72);
	SDL_Color color = {0xff, 0x00, 0x00, 0x00};
	uint16_t text_data[] = {'H', 'e', 'l', 'l', 0xF6, 'Ö', 0x0A, 'W', 'o', 'r', 'l', 'd', 0x22, 0x32, '+', 0x00}; //unicode number

	int w, h;
	if(TTF_SizeUNICODE(text_font, text_data, &w,& h))
		throw "TTF_SizeUNICODE";

	SDL_Surface* sdl_surface = TTF_RenderUNICODE_Blended_Wrapped(
		text_font, //TTF or OTF text font
		text_data, //unicode text data utf16
		color, //text color
		400 //inverse text width? tweak this parameter for diffrent font sizes
	);

	//GLuint texture_id;
	//glGenTextures(1, &texture_id);
	//glBindTexture(GL_TEXTURE_2D, texture_id);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdl_surface->w, sdl_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
	//glBindTexture(GL_TEXTURE_2D, 0);

	if(0)
	{
		FILE* file = fopen("output.txt", "w+");

		for(int h=0; h<sdl_surface->h; h++)
		{
		
			for(int w=0; w<sdl_surface->w; w++)
			{
				unsigned int xxx = ((unsigned int*)sdl_surface->pixels)[h*sdl_surface->w + w];
				/*if(xxx != 0)
					fprintf(file, "x", xxx);
				else
					fprintf(file, " ", xxx);*/
				fprintf(file, "%08x ", xxx);
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}

	//MULTIPLY WITH ALPHA TO ACTUALLY SEE SOMETHING

	glEnable(GL_BLEND);
	
	//{
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glUseProgram(shader_program_text);
	//	glEnable(GL_TEXTURE_2D);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texture_id);

	//	glBegin(GL_TRIANGLE_STRIP);
	//	//texcoord; position;
	//	glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, -1); //bottom left
	//	glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, -1); //bottom right
	//	glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, +1); //top left
	//	glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, +1); //top right
	//	glEnd();

	//	glDisable(GL_TEXTURE_2D);
	//	glUseProgram(0);
	//}

	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelZoom(1.0f, -1.0f);
		glRasterPos2f(-1.f, -0.5f);
		glDrawPixels(sdl_surface->w, sdl_surface->h, GL_BGRA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
		glRasterPos2f(-1.f, +0.5f);
		glDrawPixels(sdl_surface->w, sdl_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
	}

	glDisable(GL_BLEND);

	SDL_FreeSurface(sdl_surface);
	TTF_CloseFont(text_font);
}

void test_gl_draw_pixels()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	for(int ia=0; ia<1024*1024; ia++)
	{
		t.c[ia] = rand();
	}
	glDrawPixels(1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, t.c);
}
