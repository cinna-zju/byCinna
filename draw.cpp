#include "stdafx.h"
#include "GL/glut.h"
#include "windows.h"


float fEarthRot = 0, fMoonRot = 0;
extern float x, y, z, cube_x, cube_y, cube_z, strength;
extern float joints[20][3];

void RenderScene()
{
	// �õ�ǰ�������ɫ�������
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, 80.0f, -200.0f);

	float lightPos[] = { 0,0,0,1 };
	float lightAmb[] = { 0.2,0.2,0.2,1 };
	float lightDif[] = { 0.7,0.7,0.7,1 };
	float lightSpe[] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpe);

	glEnable(GL_LIGHT0);



	// ����̫��
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3ub(200, 200, 100);
	glutSolidSphere(50.0f, 50, 50);
	glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);

	// ���Ƶ���
	//get texture
	/*
	GLint width, height, imageWidth, pixelData;
	FILE *pfile=fopen("...\earth.bmp", "rb");
	fseek(pfile, 0x0012, SEEK_SET);          // �ƶ���0x0012λ��
	fread(&width, sizeof(width), 1, pfile); // ��ȡ���
	fseek(pfile, 0x0016, SEEK_SET);          // �ƶ���0x0016λ��
											 // ������һ��ִ�к󱾾�Ӧ����0x0016λ��
											 // ������һ���ʡ��
	fread(&height, sizeof(height), 1, pfile); // ��ȡ�߶�

	*/


	glEnable(GL_LIGHTING|GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glColor3ub(20, 20, 200);
	glTranslatef(200.0f, 0.0f, 0.0f);
	fEarthRot += 1.0f;
	if (fEarthRot > 360.0f) {
		fEarthRot = 0.0f;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glutSolidSphere(30.0f, 30, 30);

	// ��������
	glColor3ub(100, 100, 100);
	// ��ת����ϵ���ۼ�Ч��
	glRotatef(fMoonRot, 0.0f, 1.0f, 0.0f);
	// ƽ������ϵ���ۼ�Ч��
	glTranslatef(70.0f, 0.0f, 0.0f);
	// ������������ת����
	fMoonRot += 10.0f;
	if (fMoonRot > 360.0f) {
		fMoonRot = 0.0f;
	}
	glutSolidSphere(10.0f, 15, 15);

	// �ָ�����״̬����ǰ����ϵ���Ӿ������غϣ�
	glPopMatrix();
	Sleep(20);

}




void drawHand()
{
	glColor3f(1, 1, 1);
	if (strength > 0.8)
		glColor3f(0, 1, 0);

	// �������״̬��ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// ƽ������ϵ��ע����������Ӿ������λ��
	glTranslatef(x, y, z);
	glEnable(GL_DEPTH_TEST);
	glutSolidSphere(5, 15, 15);
	glPopMatrix();
	/*
	glColor3f(0, 0, 1);
	for (int j = 0; j < 20; j++) {
		float x = joints[j][0];
		float y = joints[j][1];
		float z = joints[j][2];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);
		glutSolidSphere(5, 15, 15);
		glPopMatrix();
	}
	*/

}