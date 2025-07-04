#include <stdlib.h>

#include <GL/glut.h>

#include <stb_image_write.h>

int ww = 256;
int wh = 256;
int once = 1;

double rot = 0;

GLfloat lightpos[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightwht[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightgry[] = {0.6f, 0.6f, 0.6f, 1.0f};
GLfloat lightdim[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat lightblk[] = {0.0f, 0.0f, 0.0f, 1.0f};

void display(void){
	const float csz = 1.5;
	const float sp = 0.5;
	const float cm = 0.75;
	const float csp = 0.001;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(rot, 0, 1, 0);
	glColor3f(cm, 0, 0);
	glPushMatrix();
	glTranslatef(-csp / 2, sp - csp / 2, -csp / 2);
	glutSolidCube(csz + csp);
	glPopMatrix();

	glColor3f(0, cm, 0);
	glPushMatrix();
	glTranslatef(0, 0, sp);
	glutSolidCube(csz);
	glPopMatrix();

	glColor3f(0, 0, cm);
	glPushMatrix();
	glTranslatef(sp, 0, 0);
	glutSolidCube(csz);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();

	if(once){
		int y;
		unsigned char* px = malloc(ww * wh * 4);
		glReadPixels(0, 0, ww, wh, GL_RGBA, GL_UNSIGNED_BYTE, px);

		for(y = 0; y < wh / 2; y++){
			int iy = wh - y - 1;
			int x;
			for(x = 0; x < ww * 4; x++){
				unsigned char old = px[(iy * ww * 4) + x];
				px[(iy * ww * 4) + x] = px[(y * ww * 4) + x];
				px[(y * ww * 4) + x] = old;
			}
		}

		stbi_write_png("output.png", ww, wh, 4, px, 0);
		free(px);

		once = 0;
	}
}

void resize(int w, int h){
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / h, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4, 4, 4, 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	ww = w;
	wh = h;
}

void keyboard(unsigned char key, int x, int y){
	if(once) return;
	switch (key){
		case 'q':
		case 'Q':
		case '\033':
			exit(0);
		default:
			break;
	}
}

void special(int key, int x, int y){
	if(once) return;
	switch(key){
		case GLUT_KEY_LEFT:
			rot -= 5;
			break;
		case GLUT_KEY_RIGHT:
			rot += 5;
			break;
	}
}

void idle(void){
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInitWindowSize(ww, wh);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightgry);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightwht);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightwht);

	glutMainLoop();
}
