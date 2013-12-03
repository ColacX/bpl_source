#pragma once

void window_display_function()
{
}

void window_reshape_function(int width, int height)
{
	//dont allow resize of window
	glutReshapeWindow(window_width, window_height);
}

void keyboard_function(unsigned char key, int x, int y)
{
	//text_area::keyboard_function(key, x, y);
	blockTypes::Keyboard(key, x, y);
}

void keyboard_special_function(int key, int x, int y)
{
	//text_area::keyboard_special_function(key, x, y);
	blockTypes::Keyboard_Special(key, x, y);
}

void mouse_function(int button, int state, int x, int y)
{
	water_effect::mouse_interaction(x, y);
	color_selector::mouseInteraction(x, y);
	particle_system::addSource(x, y);
	blockTypes::mouseFunction(button, state, x, y);
}

void mouse_motion_function(int x, int y)
{
	blockTypes::mouseMovement(x, y);
}

void idle_function()
{
	program_run();
}
