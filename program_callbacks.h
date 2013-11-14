#pragma once

void window_display_function()
{
}

void window_reshape_function(int width, int height)
{
	window_width = width;
	window_height = height;
}

void keyboard_function(unsigned char key, int x, int y)
{
}

void keyboard_special_function(int key, int x, int y)
{
}

void mouse_function(int button, int state, int x, int y)
{
	water_effect::mouse_interaction(x, y);
}

void mouse_motion_function(int x, int y)
{
}

void idle_function()
{
	program_run();
}
