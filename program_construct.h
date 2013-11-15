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

	//initialize standard libraries
	{
		if(glewInit() != 0)
			throw "glewInit";
		if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF) != 7)
			throw "IMG_Init";
		if(TTF_Init() == -1)
			throw "TTF_Init";
	}

	//initialize custom program objects
	{
		water_effect::construct();
		text_area::construct();
	}

	//special settings
	{
		//VerticalSync(false);
		srand(1337);
	}

	printf("entering main loop\n");
	glutMainLoop();
}
