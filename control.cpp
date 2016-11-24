#include "stdafx.h"
#include "GL/glut.h"
#include <iostream>

extern void RenderScene();
extern float x, y, z;

void MoveModel(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	RenderScene();
	glPopMatrix();
}
/*
void rotCube(float v)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(v, 0, 1, 0);
	glTranslatef(x, y, z);
	glPopMatrix();

}
*/
//void drawModel(float x, float y, float z)
//{
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glTranslatef(x, y, z);
//	RenderScene();
//	glPopMatrix();
//}