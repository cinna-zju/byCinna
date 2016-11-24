#include "stdafx.h"
#include "GL/glut.h"
#include "windows.h"

float w = 0;
float fEarthRot = 0, fMoonRot = 0;

extern float x, y, z, cube_x, cube_y, cube_z, strength;
extern float joints[20][3];
extern GLuint texID[3];

GLuint getTex(char *p)
{
	//get texture
	GLint width, height, pixelLength;
	GLubyte* pixelData=NULL;
	GLuint texid;
	FILE *pfile=NULL;
	if(fopen_s(&pfile, p, "rb+"))
		return -1;
	if (fseek(pfile, 18, SEEK_SET))
		return -1;// 移动到0x0012位置

	fread(&width, sizeof(width), 1, pfile); // 读取宽度
	//(pfile, 0x0016, SEEK_SET);          // 移动到0x0016位置
											 // 由于上一句执行后本就应该在0x0016位置
											 // 所以这一句可省略
	fread(&height, sizeof(height), 1, pfile); // 读取高度
	pixelLength = width * 3;
	while (pixelLength % 4 != 0) {
		pixelLength++;
	}
	pixelData = (GLubyte*)malloc(pixelLength*height);
	fseek(pfile, 54, SEEK_SET);
	fread(pixelData, pixelLength*height, 1, pfile);

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	//glMatrixMode(GL_TEXTURE);
	//glLoadIdentity();
	//glRotatef(30, 1, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelData);
	return texid;
}
void RenderScene()
{
	// 用当前的清除颜色清除窗口
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	//set up light source
	float lightPos[] = { 0,0,0,1 };
	float lightAmb[] = { 0.3f,0.3f,0.3f,1.0f };
	float lightDif[] = { 0.7f,0.7f,0.7f,1.0f };
	float lightSpe[] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpe);

	glEnable(GL_LIGHT0);

	// 绘制太阳

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(0.0f, 80.0f, -200.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, texID[2]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);
	gluSphere(qobj, 50, 60, 60);
	glDisable(GL_TEXTURE_2D);
	

	// 绘制地球
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	//glColor3ub(20, 20, 200);
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(200.0f, 0.0f, 0.0f);
	fEarthRot += 0.5f;
	if (fEarthRot > 360.0f) {
		fEarthRot = 0.0f;
	}
	//start  drawing a sphere
	qobj = gluNewQuadric();
	/////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	/////////////////////////////////////
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);
	gluSphere(qobj, 30, 30, 30);
	
	// 绘制月球
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(70.0f, 0.0f, 0.0f);
	// 设置月亮的旋转步进
	fMoonRot += 5.0f;
	if (fMoonRot > 360.0f) {
		fMoonRot = 0.0f;
	}
	qobj = gluNewQuadric();
	glBindTexture(GL_TEXTURE_2D, texID[3]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);

	gluSphere(qobj, 10, 10, 10);
	glDisable(GL_TEXTURE_2D);
	
	// 恢复矩阵状态（当前坐标系与视觉坐标重合）
	glPopMatrix();
	Sleep(20);

}

void drawHand()
{
	glColor3f(1, 1, 1);

	// 保存矩阵状态（模型视图矩阵）
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// 平移坐标系，注意是相对于视觉坐标的位置
	glTranslatef(x, y, z);
	glEnable(GL_DEPTH_TEST);
	glutSolidSphere(3, 10, 10);
	glPopMatrix();
	
	for (int j = 0; j < 20; j++) {
		float x = joints[j][0];
		float y = joints[j][1];
		float z = joints[j][2];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);
		glutSolidSphere(3, 10, 10);
		glPopMatrix();
	}
	//line between each joint
	for (int j = 0; j < 19; j++) {
		if (j == 3 || j == 7 || j == 11 || j == 15) {
			continue;
		}
		float x = joints[j][0];
		float y = joints[j][1];
		float z = joints[j][2];
		float a = joints[j + 1][0];
		float b = joints[j + 1][1];
		float c = joints[j + 1][2];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glColor3f(0, 0, 1);
		glLineWidth(3);
		glBegin(GL_LINES);
			glVertex3d(x, y, z);
			glVertex3d(a, b, c);
		glEnd();
		glPopMatrix();
		glColor3f(1, 1, 1);
	}
}