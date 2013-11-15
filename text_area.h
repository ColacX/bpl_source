//draw text using SDL2.0, OpenGL
//supports TTF and OTF fonts
//http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_42.html
//http://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_5.html#SEC5

#pragma once

namespace text_area
{
	std::vector<uint16_t> text_data;
	TTF_Font* text_font;
	float rectangle_u, rectangle_v, rectangle_w, rectangle_h;
	SDL_Color rgba_color = {0xff, 0x00, 0x00, 0x00};

	void construct()
	{
		uint16_t default_text[] = {
			'H', 'e', 'l', 'l', 0xF6, 'Ö', '\n', 'W', 'o', 'r', 'l', 'd', ' ', '1', '2', '3', '4', 'X', 'Y', 'Z', 'x', 'y', 'z'}; //unicode number
		
		for(int ia=0; ia<sizeof(default_text)/sizeof(uint16_t); ia++)
			text_data.push_back(default_text[ia]);

		rectangle_u = 0;
		rectangle_v = 0;
		rectangle_w = 1000;
		rectangle_h = 1000;

		text_font = TTF_OpenFont("bpl_binary/arial.ttf", 72);
		TTF_SetFontStyle(text_font, TTF_STYLE_STRIKETHROUGH | TTF_STYLE_UNDERLINE | TTF_STYLE_ITALIC);
		//TTF_SetFontKerning(text_font, 1);
	}

	void destruct()
	{
		TTF_CloseFont(text_font);
	}

	//draw TTF and OTF supported text in natural window coordinate space
	//figures out how to word wraps the text
	//draws only text that will be visible within the viewing rectangle
	//since this call is expensive a tip is to draw it to a FBO, only when text is modified, generate a texture and use it instead
	void draw_wrapped(const std::vector<uint16_t>& text_data, TTF_Font* text_font, float rectangle_u, float rectangle_v, float rectangle_w, float rectangle_h)
	{
		std::vector<uint16_t> text_draw_data;
		int word_start = 0;
		int word_width = 0;
		int draw_index = 0;

		printf("--------------------------\n");
		for(int ia=0; ia < text_data.size(); ia++)
		{
			int min_x, max_x, min_y, max_y, advance_x;
			TTF_GlyphMetrics(text_font, text_data[ia], &min_x, &max_x, &min_y, &max_y, &advance_x);
			advance_x += 1;
			word_width += advance_x;
			printf("d:%d: x:%x c:%c a:%d w:%d\n", text_data[ia], text_data[ia], text_data[ia], advance_x, word_width);

			if(text_data[ia] == ' ' || text_data[ia] == '\n')
			{
				text_draw_data.push_back(text_data[ia]);
				word_width = 0;
			}
			else if(text_data[ia] == 0x00)
			{
				text_draw_data.push_back(0);
				break;
			}
			else if(word_width >= rectangle_w)
			{
				//the word is too wide, split it into multiple rows
				text_draw_data.push_back('\n');
				text_draw_data.push_back(text_data[ia]);
				word_width = advance_x;
				continue;
			}
			else
			{
				//copy all other letters
				text_draw_data.push_back(text_data[ia]);
			}
		}

		text_draw_data.push_back(0x00);

		SDL_Surface* sdl_surface = TTF_RenderUNICODE_Blended_Wrapped(
			text_font, //TTF or OTF text font
			(uint16_t*)(&text_draw_data[0]), //unicode text data utf16
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

	void draw_unwrapped(const std::vector<uint16_t>& text_data, TTF_Font* text_font, float rectangle_u, float rectangle_v, float rectangle_w, float rectangle_h)
	{
		//http://www.sdltutorials.com/sdl-ttf
		int line_start_index = 0;
		int line_v = 0;
		int font_line_skip = TTF_FontLineSkip(text_font);
		int font_glyph_width = 52;
		std::vector<uint16_t> text_draw_data;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for(int ia=0; ia<=text_data.size(); ia++)
		{
			//printf("%c %d %x %d\n", text_data[ia], text_data[ia], text_data[ia], line_v);

			if(!text_draw_data.empty() && (ia == text_data.size() || text_data[ia] == '\n'))
			{
				int letter_start_u = 0;

				for(int ib=0; ib<text_draw_data.size(); ib++)
				{
					uint16_t glyph = text_draw_data[ib];

					if(TTF_GlyphIsProvided(text_font, glyph) == 0)
					{
						glyph = 0xffff;
						//continue;
					}

					int min_x, max_x, min_y, max_y, advance_x;
					TTF_GlyphMetrics(text_font, glyph, &min_x, &max_x, &min_y, &max_y, &advance_x);
					SDL_Surface* sdl_surface = TTF_RenderGlyph_Blended(text_font, glyph, rgba_color);

					//ATTENTION remember to unbind texture2d and disable it before, otherwize the text will not be visible or might be drawn with strange colors
					//glDisable(GL_TEXTURE_2D);
					//glActiveTexture(GL_TEXTURE0);
					//glBindTexture(GL_TEXTURE_2D, 0);
		
					glWindowPos2f(rectangle_u + letter_start_u, window_height - rectangle_v - line_v);
					glPixelZoom(1.0f, -1.0f);
					glDrawPixels(min(rectangle_w, sdl_surface->w), min(rectangle_h, sdl_surface->h), GL_BGRA, GL_UNSIGNED_BYTE, sdl_surface->pixels);

					SDL_FreeSurface(sdl_surface);
					//letter_start_u += advance_x;
					letter_start_u += font_glyph_width;
				}
				text_draw_data.clear();
				line_v += font_line_skip;
			}
			else if(ia != text_data.size() && text_data[ia] == '\n')
			{
				line_v += font_line_skip;
			}
			else if(ia < text_data.size())
			{
				text_draw_data.push_back(text_data[ia]);
			}
		}

		glDisable(GL_BLEND);
	}

	void draw()
	{
		if(text_data.empty())
			return;
		
		draw_unwrapped(text_data, text_font, rectangle_u, rectangle_v, rectangle_w, rectangle_h);
	}

	void keyboard_function(unsigned char key, int x, int y)
	{
		if(key == VK_BACK)
		{
			if(!text_data.empty())
				text_data.pop_back();
		}
		else if(key == VK_RETURN)
			text_data.push_back('\n');
		else
			text_data.push_back(key);
	}

	void keyboard_special_function(int key, int x, int y)
	{

	}
};
