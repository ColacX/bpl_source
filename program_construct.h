#pragma once

void program_construct(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(window_width, window_height);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("BPL");
	glewInit();

	glutDisplayFunc(window_display_function);
	glutReshapeFunc(window_reshape_function);
	glutKeyboardFunc(keyboard_function);
	glutSpecialFunc(keyboard_special_function);
	glutMouseFunc(mouse_function);
	glutMotionFunc(mouse_motion_function);
	glutIdleFunc(idle_function);
	glutMainLoop();
}
