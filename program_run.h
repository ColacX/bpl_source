#pragma once

void program_run()
{
	double start_time = windows_timer_get_time();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	water_effect::update();
	water_effect::draw();
	text_area::draw();
	blockTypes::draw();
	glutSwapBuffers();

	double end_time = windows_timer_get_time();
	printf("fps: %f\n", 1000000.0 / (end_time - start_time));
}
