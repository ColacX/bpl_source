#pragma once

void program_run()
{
	//Timer(T, printf("time: %lfs , fps: %lf\n", T/1000000.0, 1000000.0/T))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		water_effect::update();
		water_effect::draw();
		//text_area::draw();
		blockTypes::draw();
		particle_system::update();
		particle_system::draw();
		glutSwapBuffers();
	}
}
