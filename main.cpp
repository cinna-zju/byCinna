#include "stdafx.h"
#include "windows.h"
#include "Leap.h"
#include "GL/glut.h"
#include <string>
#include "draw.h"
#include "control.h"

#define FOV 160
#define FNEAR 200
#define FFAR 1200

using namespace Leap;

float x = 0, y = 0, z = 0;//palm center 
float model_x=0, model_y=0, model_z=-100;//temp position of model
Controller controller;
float joints[20][3] = { 0 };
float strength;

GLuint texID[10];//texture id

void Render()
{
    RenderScene();	
	if (strength < 0.5) {
		model_x = x;
		model_y = y;
		model_z = z;
		MoveModel(model_x, model_y, model_z);
	}
	//else {
	//	drawModel(model_x, model_y, model_z);
	//	RenderScene();
	//}
	drawHand();
	glutSwapBuffers();
}

void LeapListener()
{
	strength = 0; x = 0, y = 0, z = 0; 
	for (int i = 24; i > 0; i--) {
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
		strength += righthand.grabStrength();		
	}
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 3; j++) {
			joints[i][j] /= 24;
		}
	x /= 24; y /= 24; z /= 24; strength /= 24; 
	//std::cout << x << ' ' << y << ' ' << z << '\n';
	if (strength > 0.5) {
		glLoadIdentity();
		gluLookAt(3*x, 3*y, -3*z, 0, 80, -200, 0, 1, 0);
	}
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

	glFrustum(-FOV, FOV, -FOV, FOV, FNEAR, FFAR);
	gluLookAt(0, 300, 300, 0, 50, 0, 0, 0.5, -0.5);
	
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Scene1");
	//define texture
	texID[1]=getTex("texture\\earth.bmp");//bug: index cant be 0
	texID[2]=getTex("texture\\sun.bmp");
	texID[3] = getTex("texture\\moon.bmp");
	glutDisplayFunc(Render);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(LeapListener);
	//set background policy
	bool backgroundModeAllowed = controller.config().getInt32("background_app_mode") == 2;
	if (!backgroundModeAllowed) {
			controller.config().setInt32("background_app_mode", 2);
			controller.config().save();
	}
	controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	glutMainLoop();
	return 0;
}
