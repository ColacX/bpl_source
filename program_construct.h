#pragma once

void program_construct(int argc, char** argv)
{
	//initalizing window
	{
		printf("intializing window\n");
		glutInit(&argc, argv);
		glutInitWindowSize(window_width, window_height);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
		glutCreateWindow("BPL");
		glewInit();
	}

	//register callbacks
	{
		printf("registering callbacks\n");
		glutDisplayFunc(window_display_function);
		glutReshapeFunc(window_reshape_function);
		glutKeyboardFunc(keyboard_function);
		glutSpecialFunc(keyboard_special_function);
		glutMouseFunc(mouse_function);
		glutMotionFunc(mouse_motion_function);
		glutIdleFunc(idle_function);
	}

	{
		water_effect::construct();
	}

	printf("entering main loop\n");
	glutMainLoop();
}
