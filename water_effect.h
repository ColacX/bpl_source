#pragma once

namespace water_effect
{
	GLuint height0_texture, height1_texture;
	GLuint ripple_wave_update, ripple_wave_rendering, ripple_wave_mouse, ripple_wave_player;
	GLuint ripple_frame_buffer;
	GLuint textures[2];
	GLuint background_texture, player_texture, reflection_texture;
	int water_width = 1280;
	int water_height = 720;

	GLuint construct_color_texture()
	{
		float* texture_data = t.f;
		GLuint generated_texture;
		glGenTextures(1, &generated_texture);
		glBindTexture(GL_TEXTURE_2D, generated_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST, GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		
		GLfloat border_color[4] = {0,0,0,0};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		
		glTexImage2D(
			GL_TEXTURE_2D, //target
			0, //mipmap-level
			GL_R32F, //texture format
			water_width, //texture width
			water_height, //texture height
			0, //this value must be 0? lol.
			GL_RED, //input data format
			GL_FLOAT, //input data element type
			texture_data //input data
		); 
		glBindTexture(GL_TEXTURE_2D, 0);
		return generated_texture;
	}

	void construct()
	{
		ripple_wave_rendering = CreateShaderProgram("bpl_source/water_effect_rendering.vert", "bpl_source/water_effect_rendering.frag");
		glUniform1i(glGetUniformLocation(ripple_wave_rendering, "background_sampler"), 0);

		background_texture = LoadImageToTexture("bpl_binary/transparent.png");
	}

	void draw()
	{
		glUseProgram(ripple_wave_rendering);

		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, background_texture);

		glBegin(GL_TRIANGLE_STRIP);
        glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, -1); //bottom left
        glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, -1); //bottom right
        glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, +1); //top left
        glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, +1); //top right
        glEnd();

		glDisable(GL_TEXTURE_2D);

		glUseProgram(0);
	}
};
