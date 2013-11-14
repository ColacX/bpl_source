#pragma once

namespace water_effect
{
	GLuint height0_texture, height1_texture;
	GLuint ripple_wave_update, ripple_wave_rendering, ripple_wave_mouse, ripple_wave_player;
	GLuint ripple_frame_buffer;
	GLuint background_texture, player_texture, reflection_texture;
	int water_width = 1280;
	int water_height = 720;
	bool ping_pong = false;

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
		height0_texture = construct_color_texture();
		height1_texture = construct_color_texture();

		ripple_wave_rendering = CreateShaderProgram("bpl_source/water_effect_rendering.vert", "bpl_source/water_effect_rendering.frag");
		glUseProgram(ripple_wave_rendering);
		glUniform1i(glGetUniformLocation(ripple_wave_rendering, "background_sampler"), 0);
		glUniform1i(glGetUniformLocation(ripple_wave_rendering, "height_sampler"), 1);
		glUseProgram(0);

		ripple_wave_mouse = CreateShaderProgram("bpl_source/water_effect_mouse.vert", "bpl_source/water_effect_mouse.frag");

		ripple_wave_update = CreateShaderProgram("bpl_source/water_effect_update.vert", "bpl_source/water_effect_update.frag");
		glUseProgram(ripple_wave_update);
		glUniform1i(glGetUniformLocation(ripple_wave_update, "height0_sampler"), 0);
		glUniform1i(glGetUniformLocation(ripple_wave_update, "height1_sampler"), 1);
		glUseProgram(0);
		
		background_texture = LoadImageToTexture("bpl_binary/Colorful-Sky-Desktop-Background.jpg");
		//background_texture = LoadImageToTexture("bpl_binary/Colorful-Sky-Desktop-Background.png");

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
				height0_texture, //texture id
				0 //mip map level to attach
			);

			glFramebufferTexture2D(
				GL_FRAMEBUFFER, //GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER
				GL_COLOR_ATTACHMENT1, //GL_COLOR_ATTACHMENTi, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT
				GL_TEXTURE_2D, //specifies what type of texture is attached
				height1_texture, //texture id
				0 //mip map level to attach
			);
			
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw "error glCheckFramebufferStatus";

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			ripple_frame_buffer = generated_frame_buffer;
		}
	}

	void draw()
	{
		glUseProgram(ripple_wave_rendering);
		glEnable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, background_texture);

		glActiveTexture(GL_TEXTURE1);
		if(ping_pong)
			glBindTexture(GL_TEXTURE_2D, height1_texture);
		else
			glBindTexture(GL_TEXTURE_2D, height0_texture);

		glBegin(GL_TRIANGLE_STRIP);
		//texcoord; position;
        glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, -1); //top left
        glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, -1); //top right
        glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, +1); //bottom left
        glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, +1); //bottom right
        glEnd();

		glDisable(GL_TEXTURE_2D);
		glUseProgram(0);

		if(0)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, ripple_frame_buffer);
			
			if(ping_pong)
				glReadBuffer(GL_COLOR_ATTACHMENT1);
			else
				glReadBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, water_height, water_width, 0, 0, 0, water_width, water_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void mouse_interaction(int mx, int my)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ripple_frame_buffer);
		glUseProgram(ripple_wave_mouse);

		if(ping_pong)
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		else
			glDrawBuffer(GL_COLOR_ATTACHMENT1);
		
		float s = (float)mx/(float)water_width*2.0f - 1.0f;
		float t = (float)my/(float)water_height*2.0f - 1.0f;

		//glBegin(GL_POINTS);
		//glVertexAttrib4f(1, 1, 0, 0, 1);
		//glVertexAttrib2f(0, s, t);
		//glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		//color; position;
        glVertexAttrib4f(1, 1, 0, 0, 0); glVertexAttrib2f(0, s, t); //bottom left
        glVertexAttrib4f(1, 1, 0, 0, 0); glVertexAttrib2f(0, s+0.1, t); //bottom right
        glVertexAttrib4f(1, 1, 0, 0, 0); glVertexAttrib2f(0, s, t+0.1); //top left
        glVertexAttrib4f(1, 1, 0, 0, 0); glVertexAttrib2f(0, s+0.1, t+0.1); //top right
        glEnd();
		
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void update()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ripple_frame_buffer);
		glUseProgram(ripple_wave_update);
		glEnable(GL_TEXTURE_2D);

		if(ping_pong)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT1);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, height1_texture);
		
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, height0_texture);
		}
		else
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, height0_texture);
			
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, height1_texture);
		}

		glBegin(GL_TRIANGLE_STRIP);
		//texcoord; position; //upside down texcoords???
        glVertexAttrib2f(1, 0, 0); glVertexAttrib2f(0, -1, -1); //top left
        glVertexAttrib2f(1, 1, 0); glVertexAttrib2f(0, +1, -1); //top right
        glVertexAttrib2f(1, 0, 1); glVertexAttrib2f(0, -1, +1); //bottom left
        glVertexAttrib2f(1, 1, 1); glVertexAttrib2f(0, +1, +1); //bottom right
        glEnd();

		glDisable(GL_TEXTURE_2D);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ping_pong = !ping_pong;
	}
};
