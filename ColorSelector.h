
#pragma once

#include <glm/glm.hpp>

namespace color_selector
{
	GLuint progID;
	float winFraction = (float)window_width / window_height;
	
	// global position and dimension
	float x = -0.98;
	float y = 0.98;
	float width = 0.3;
	
	float spaceSizeW = 0.05*width;
	float spaceSizeH = spaceSizeW * winFraction;

	float wSel = (width - 2 * spaceSizeW) / 6;
	float hSel = 2 * wSel * winFraction;
	float xSel = x + spaceSizeW;
	float ySel = y - spaceSizeH;

	float colorSizeW = (width - 6 * spaceSizeW) / 5;
	float colorSizeH = colorSizeW * winFraction;

	float height = hSel + colorSizeH + 3 * spaceSizeH;

	// current color selected to be modified
	int currentColor = 0;


	

	float selectorVert[] = { 	0*wSel+xSel, ySel,		1*wSel+xSel, ySel-hSel,		0*wSel+xSel, ySel-hSel,
								0*wSel+xSel, ySel,		1*wSel+xSel, ySel,		1*wSel+xSel, ySel-hSel,
								
								1*wSel+xSel, ySel,		2*wSel+xSel, ySel-hSel,		1*wSel+xSel, ySel-hSel,
								1*wSel+xSel, ySel,		2*wSel+xSel, ySel,		2*wSel+xSel, ySel-hSel,
								
								2*wSel+xSel, ySel,		3*wSel+xSel, ySel-hSel,		2*wSel+xSel, ySel-hSel,
								2*wSel+xSel, ySel,		3*wSel+xSel, ySel,		3*wSel+xSel, ySel-hSel,
								
								3*wSel+xSel, ySel,		4*wSel+xSel, ySel-hSel,		3*wSel+xSel, ySel-hSel,
								3*wSel+xSel, ySel,		4*wSel+xSel, ySel,		4*wSel+xSel, ySel-hSel,
								
								4 * wSel + xSel, ySel, 5 * wSel + xSel, ySel - hSel, 4 * wSel + xSel, ySel-hSel,
								4*wSel+xSel, ySel,		5*wSel+xSel, ySel,		5*wSel+xSel, ySel-hSel,
								
								5*wSel+xSel, ySel,		6*wSel+xSel, ySel-hSel,		5*wSel+xSel, ySel-hSel,
								5*wSel+xSel, ySel,		6*wSel+xSel, ySel,		6*wSel+xSel, ySel-hSel };
								
	float selectorFrag[] = {	1, 0, 0,	1, 1, 0,	1, 0, 0,
								1, 0, 0,	1, 1, 0,	1, 1, 0,
								
								1, 1, 0,	0, 1, 0,	1, 1, 0,
								1, 1, 0,	0, 1, 0,	0, 1, 0,
								
								0, 1, 0,	0, 1, 1,	0, 1, 0,
								0, 1, 0,	0, 1, 1,	0, 1, 1,
								
								0, 1, 1,	0, 0, 1,	0, 1, 1,
								0, 1, 1,	0, 0, 1,	0, 0, 1,
								
								0, 0, 1,	1, 0, 1,	0, 0, 1,
								0, 0, 1,	1, 0, 1,	1, 0, 1,
								
								1, 0, 1,	1, 0, 0,	1, 0, 1,
								1, 0, 1,	1, 0, 0, 	1, 0, 0 };
								
								
	float whiteBoxFrag[] = { 1, 1, 1,	1, 1, 1, 	1, 1, 1,	1, 1, 1};
	float blackBoxFrag[] = { 0, 0, 0,	0, 0, 0,	0, 0, 0,	0, 0, 0};

	float globalBoxVert[] = { x, y,		x, y-height,		x+width, y-height,		x+width, y};
	
	float colorBoxVert[5][8];
	float colorVert[5][8];
	float colorFrag[5][12] = { { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1 },
	{ 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	
	void construct()
	{
		for(int i(0); i<5; i++)
		{
			colorBoxVert[i][0] = x + spaceSizeW / 2 + i*(colorSizeW + spaceSizeW);
			colorBoxVert[i][1] = y-hSel-1.5*spaceSizeH;
			colorBoxVert[i][2] = x + spaceSizeW / 2 + i*(colorSizeW + spaceSizeW);
			colorBoxVert[i][3] = y-hSel-2.5*spaceSizeH-colorSizeH;
			colorBoxVert[i][4] = x + 1.5*spaceSizeW + colorSizeW + i*(colorSizeW + spaceSizeW);
			colorBoxVert[i][5] = y-hSel-2.5*spaceSizeH-colorSizeH;
			colorBoxVert[i][6] = x + 1.5*spaceSizeW + colorSizeW + i*(colorSizeW + spaceSizeW);
			colorBoxVert[i][7] = y-hSel-1.5*spaceSizeH;
		
			colorVert[i][0] = x + spaceSizeW + i*(colorSizeW + spaceSizeW);
			colorVert[i][1] = y-hSel-2*spaceSizeH;
			colorVert[i][2] = x + spaceSizeW + i*(colorSizeW + spaceSizeW);
			colorVert[i][3] = y-hSel-2*spaceSizeH-colorSizeH;
			colorVert[i][4] = x + spaceSizeW + colorSizeW + i*(colorSizeW + spaceSizeW);
			colorVert[i][5] = y-hSel-2*spaceSizeH-colorSizeH;
			colorVert[i][6] = x + spaceSizeW + colorSizeW + i*(colorSizeW + spaceSizeW);
			colorVert[i][7] = y-hSel-2*spaceSizeH;
		}
	
		
		progID = CreateShaderProgram("bpl_source/color2D.vert", "bpl_source/color2D.frag");
	}
	
	void draw()
	{
		glUseProgram(progID);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			
			// GLOBAL BOX
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, globalBoxVert);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, whiteBoxFrag);
			glDrawArrays(GL_QUADS, 0, 4);
			
			// COLOR SELECTOR
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, selectorVert);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, selectorFrag);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			// COLOR BOXES
			for(int i(0); i<5; i++)
			{
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, colorBoxVert[i]);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, currentColor == i ? blackBoxFrag : whiteBoxFrag);
				glDrawArrays(GL_QUADS, 0, 4);
				
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, colorVert[i]);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colorFrag[i]);
				glDrawArrays(GL_QUADS, 0, 4);
			}
			
		glUseProgram(0);
	}
	
	glm::vec3 getColorFromAngle(float angle)
	{
		// RGB COLOR
		float rVal;
		float gVal;
		float bVal;
		if (angle >= 0 && angle < 60){
			rVal = 1.0;
			gVal = angle / 60;
			bVal = 0;
		}
		else if (angle >= 60 && angle < 120){
			rVal = (120 - angle) / 60;
			gVal = 1.0;
			bVal = 0;
		}
		else if (angle >= 120 && angle < 180){
			rVal = 0;
			gVal = 1.0;
			bVal = (angle - 120) / 60;
		}
		else if (angle >= 180 && angle < 240){
			rVal = 0;
			gVal = (240 - angle) / 60;
			bVal = 1.0;
		}
		else if (angle >= 240 && angle < 300){
			rVal = (angle - 240) / 60;
			gVal = 0;
			bVal = 1.0;
		}
		else{
			rVal = 1.0;
			gVal = 0;
			bVal = (360 - angle) / 60;
		}

		glm::vec3 color(rVal, gVal, bVal);
		return color;
	}

	void updateCurrentColor(glm::vec3 color)
	{
		// update box color
		for (int i(0); i < 12; i += 3)
		{
			colorFrag[currentColor][i] = color.x;
			colorFrag[currentColor][i + 1] = color.y;
			colorFrag[currentColor][i + 2] = color.z;
		}
		// and block color
		switch (currentColor)
		{
		case 0:
			blockTypes::color1 = color;
			blockTypes::need_update();
			break;
		case 1:
			blockTypes::color2 = color;
			blockTypes::need_update();
			break;
		case 4:
			blockTypes::color5 = color;
			blockTypes::need_update();
			break;
		}
		
	}

	void mouseInteraction(int mx, int my)
	{
		float s = (float)mx/window_width*2-1;
		float t = 1 - (float)my/window_height*2;

		// check if cursor is in selector area
		if(s < x || s > x + width || t > y || t < y - height) { return; }

		// select new current color (to be modified) ?
		if(s >= x+spaceSizeW && s <= x+5*spaceSizeW+5*colorSizeW && t <= y-hSel-2*spaceSizeH && t >= y-hSel-2*spaceSizeH-colorSizeH)
		{
			float relS = fmod((s - (x + spaceSizeW)), (colorSizeW + spaceSizeW));
			if (relS <= colorSizeW){
				currentColor = 5*(s - (x + spaceSizeW))/(6*wSel);
			}
		}
		// modify the current selected color ?
		else if(s >= xSel && s <= xSel + 6*wSel && t <= ySel && t >= ySel - hSel)
		{
			// determine angle
			float angle = (float)(s - xSel)/(6*wSel) * 360;
			
			// get color from Angle
			glm::vec3 color = getColorFromAngle(angle);
			
			// update block type color + color boxes
			updateCurrentColor(color);
		}
	}

	
	
	

}