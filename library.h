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
		trigger_breakpoint;
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

GLenum CreateShaderProgram(const char* vert_source = 0, const char* frag_source = 0, const char* geom_source = 0)
{
	GLenum program_object = glCreateProgram();

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
