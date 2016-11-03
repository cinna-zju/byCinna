#include "stdafx.h"
#include "windows.h"
#include "Leap.h"
#include <cstring>
#include "GL/glut.h"
#include "draw.h"

#define fov 160
#define fnear 200
#define ffar 800

using namespace Leap;


float x = 0, y = 0, z = 0;
float cube_x=0, cube_y=0, cube_z=-100;
Controller controller;
float joints[20][3] = { 0 };
float strength;
float gestureSwipe[2];

extern void RenderScene();
extern float fEarthRot, fMoonRot;




void drawCube(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	RenderScene();
	glPopMatrix();
}
void moveCube(float x, float y, float z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	RenderScene();
	glPopMatrix();

}
void rotCube(float v)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(v, 0, 1, 0);
	glTranslatef(x, y, z);
	glColor3f(1, 1, 0);
	glutWireCube(100);
	glPopMatrix();

}

void moveCamera()
{
	glMatrixMode(GL_PROJECTION);
	//();
	glTranslatef(x, y, 0);
	glPopMatrix();
	//gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
}



void Render()
{

	glClear(GL_COLOR_BUFFER_BIT);

    RenderScene();

	
	if (strength > 0.8) {
		//(x, y, z);
		cube_x = x;
		cube_y = y;
		cube_z = z;
		moveCube(cube_x, cube_y, cube_z);
		//moveCamera();
	}
	else {
		drawCube(cube_x, cube_y, cube_z);
		RenderScene();

	}
	//if (gestureSwipe[2] > 50) {
		//for (int i = 0; i <= 20; i++) {
			//glMatrixMode(GL_PROJECTION);
			//gluLookAt(0, 10*i, 300 - 25*i, 0, 0, 300-25*i, 0, 0, -1);
			//RenderScene();
			//Sleep(10
			//gluLookAt(0, 200, -300, 0, 0, -300, 0, 0, -1);
		//}
			
	//}
	//if (gestureSwipe[2] < -50) {
		//gluLookAt(0, 0, 300, 0, 0, 0, 0, 1, 0);
		
	//}
	/*
	/////////////////////////////////
	if (abs(gestureSwipe) > 0.5)
	{
		//rotCube(gestureSwipe);
	}
	//////////////////////////////////


	*/
	drawHand();
	glutSwapBuffers();
}


void move()
{
	strength = 0; x = 0, y = 0, z = 0; 
	for (int i = 24; i > 0; i--) {
		gestureSwipe[2] = 0;
		Frame frame = controller.frame(i);
		Hand righthand = frame.hands().rightmost();
		Hand lefthand = frame.hands().leftmost();
		Vector HandCenter = righthand.palmPosition();
		Vector RightHandV = righthand.direction();
		FingerList allTheFingers = frame.fingers();
		int j = 0;
		for (FingerList::const_iterator fl = allTheFingers.begin(); fl != allTheFingers.end(); fl++) {
			for (int b = 0; b < 4; b++)
			{
				Bone::Type boneType = static_cast<Leap::Bone::Type>(b);
				Bone bone = (*fl).bone(boneType);
				Vector middle = bone.center();
				joints[j][0] += middle.toFloatPointer()[0];
				joints[j][1] += middle.toFloatPointer()[1];
				joints[j][2] += middle.toFloatPointer()[2];
				j++;

			}
		}
		x += HandCenter.x;
		y += HandCenter.y;
		z += HandCenter.z;
		gestureSwipe[2] += righthand.palmVelocity()[1];
		strength += righthand.grabStrength();
		
	}
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 3; j++) {
			joints[i][j] /= 24;
		}
	x /= 24; y /= 24; z /= 24; strength /= 24; 
	gestureSwipe[2] /= 24;
	//y -= 20;
	//std::cout << x << ' ' << y << ' ' << z << '\n';
	glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	// 窗口的纵横比
	GLfloat fAspect;
	// 防止被0除
	if (0 == h) {
		h = 1;
	}
	// 将视口设置为窗口的大小
	glViewport(0, 0, w, h);
	// 计算窗口的纵横比
	fAspect = (GLfloat)w / (GLfloat)h;
	// 设置当前操作的矩阵为投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-fov, fov, -fov, fov, fnear, ffar);
	gluLookAt(0, 300, 300, 0, 50, 0, 0, 0.5, -0.5);
	
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);

	
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Scene1");
	
	glutDisplayFunc(Render);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(move);

	bool backgroundModeAllowed = controller.config().getInt32("background_app_mode") == 2;
	if (!backgroundModeAllowed) {
			controller.config().setInt32("background_app_mode", 2);
			controller.config().save();
	}
	controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	glutMainLoop();
	return 0;
}
