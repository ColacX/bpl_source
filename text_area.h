//draw text using SDL2.0, OpenGL
//supports TTF and OTF fonts
//http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html
//http://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_5.html#SEC5

#pragma once

namespace text_area //todo change name to text_view
{
	uint16_t* text_data;
	TTF_Font* text_font;
	float rectangle_u, rectangle_v, rectangle_w, rectangle_h;
	SDL_Color rgba_color = {0xff, 0x00, 0x00, 0x00};

	void construct()
	{
		uint16_t default_text[] = {
			'H', 'e', 'l', 'l', 0xF6, 'Ö', '\n', 'W', 'o', 'r', 'l', 'd', ' ', '1', '2', '3', '4', 'X', 'Y', 'Z', 'x', 'y', 'z', 0x00}; //unicode number
		text_data = new uint16_t[sizeof(default_text)/sizeof(uint16_t)];
		for(int ia=0; ia<sizeof(default_text)/sizeof(uint16_t); ia++)
			text_data[ia] = default_text[ia];

		rectangle_u = 100;
		rectangle_v = 0;
		rectangle_w = 1000;
		rectangle_h = 1000;

		text_font = TTF_OpenFont("bpl_binary/BrushScriptStd.otf", 72);
	}

	void destruct()
	{
		TTF_CloseFont(text_font);
	}

	//draw TTF and OTF supported text in natural window coordinate space
	//figures out how to word wraps the text
	//draws only text that will be visible within the viewing rectangle
	//since this call is expensive a tip is to draw it to a FBO, only when text is modified, generate a texture and use it instead
	void draw(uint16_t* text_data, TTF_Font* text_font, float rectangle_u, float rectangle_v, float rectangle_w, float rectangle_h)
	{
		uint16_t text_draw_data[1024];
		int word_start = 0;
		int word_width = 0;
		int draw_index = 0;

		//printf("--------------------------\n");
		for(int ia=0; ia < sizeof(text_draw_data)/sizeof(uint16_t); ia++)
		{
			//printf("d:%d: x:%x c:%c\n", text_data[ia], text_data[ia], text_data[ia]);
			int min_x, max_x, min_y, max_y, advance_x;
			TTF_GlyphMetrics(text_font, text_data[ia], &min_x, &max_x, &min_y, &max_y, &advance_x);
			word_width += advance_x;

			if(text_data[ia] == ' ' || text_data[ia] == '\n')
			{
				text_draw_data[draw_index++] = text_data[ia];
				word_width = 0;
			}
			else if(text_data[ia] == 0x00)
			{
				text_draw_data[draw_index++] = 0;
				break;
			}
			else if(word_width >= rectangle_w)
			{
				//the word is too wide, split it into multiple rows
				text_draw_data[draw_index++] = '\n';
				text_draw_data[draw_index++] = text_data[ia];
				word_width = 0;
				continue;
			}
			else
			{
				text_draw_data[draw_index++] = text_data[ia];
			}
		}

		SDL_Surface* sdl_surface = TTF_RenderUNICODE_Blended_Wrapped(
			text_font, //TTF or OTF text font
			text_draw_data, //unicode text data utf16
			rgba_color, //text rgba_color
			rectangle_w //width in pixels in window space coordinates, if text drawn exceedes the width it will wrap around weirdly
		);

		//add borders
		for(int v=0; v < sdl_surface->h && v < rectangle_h; v++)
		{
			for(int u=0; u < sdl_surface->w && u < rectangle_w; u++)
			{
				//if(v == 0 || u == 0 || v == min(rectangle_h, sdl_surface->h) - 1 || u == min(rectangle_w, sdl_surface->w) - 1)
				if(v == 0 || u == 0 || v == rectangle_h - 1 || u == rectangle_w - 1)
				{
					((uint32_t*)sdl_surface->pixels)[v*sdl_surface->w + u] = 0xffffffff;
				}
			}
		}

		//ATTENTION remember to unbind texture2d and disable it before, otherwize the text will not be visible or might be drawn with strange colors
		//glDisable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glWindowPos2f(rectangle_u, window_height - rectangle_v);
		glPixelZoom(1.0f, -1.0f);
		glDrawPixels(min(rectangle_w, sdl_surface->w), min(rectangle_h, sdl_surface->h), GL_BGRA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
		glDisable(GL_BLEND);

		SDL_FreeSurface(sdl_surface);
	}

	void draw()
	{
		draw(text_data, text_font, rectangle_u, rectangle_v, rectangle_w, rectangle_h);
	}

	void keyboard_function()
	{

	}
};
