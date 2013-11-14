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
		glewInit();
		//IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	}

	{
		water_effect::construct();
	}

	{
		//VerticalSync(false);
	}

	printf("entering main loop\n");
	glutMainLoop();
}
