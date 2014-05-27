//Pedro Morgado Alarc�n
//Samuel M�ndez Gal�n

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
using namespace std;

#include "Camara.h"
#include "PV3D.h"
#include "ObjetoCompuesto3D.h"
#include "MesaBillar.h"
#include "TextureLoader.h"
#include "Textures.h"

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

// Viewport size
int WIDTH= 500, HEIGHT= 500;

// Viewing frustum parameters
GLdouble xRight=10, xLeft=-xRight, yTop=10, yBot=-yTop, N=1, F=1000;

// Camera parameters
GLdouble eyeX=25.0, eyeY=25.0, eyeZ=25.0;
GLdouble lookX=0.0, lookY=0.0, lookZ=0.0;
GLdouble upX=0, upY=1, upZ=0;

//Our parameters
GLdouble angleX = 0.0;
GLdouble angleY = 0.0;
GLdouble angleZ = 0.0;
Camara camera;
GLdouble recorridoX = 0.0;
GLdouble recorridoY = 0.0;
GLdouble recorridoZ = 0.0;
GLdouble desplazaY = 0.0;

GLdouble angleGiraX = 0.0;
GLdouble angleGiraY = 0.0;
GLdouble angleGiraZ = 0.0;

float lampHeight = 1.0;

Textures t = Textures();

PV3D d = PV3D(0.1, 0.1, 1, 0); //Para proyecci�n oblicua

ObjetoCompuesto3D* escena;

bool globalOn = true;
bool lamparaOn = true;

bool luzRemotaOn = true;
bool nieblaOn = true;

float calculateLampAngle(float a, float b){
	//Con hipotenusa y cateto(a) halla angulo. sen(alfa)=a/h
	float h = sqrt(pow(a, 2) + pow(b, 2));
	float sen = a / h;
	float rad = asin(sen);

	float degrees = rad * 57.3;

	return degrees;
}

void buildScene() {
	//escena = new ObjetoCompuesto3D();
	escena = new MesaBillar();

	// Camera set up
	camera = Camara(PV3D(eyeX, eyeY, eyeZ, 1), PV3D(lookX, lookY, lookZ, 1), PV3D(upX, upY, upZ, 0));
}

void initGL() {	 	
	buildScene();

	glClearColor(0.0f,0.0f,0.0f,1.0);
    glEnable(GL_LIGHTING);
	globalOn = true;

	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// Viewport set up
    glViewport(0, 0, WIDTH, HEIGHT);  

	 // Light0
    glEnable(GL_LIGHT0);
    GLfloat d[]={1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    GLfloat a[]={0.3f,0.3f,0.3f,1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[]={25.0, 25.0, 0.0, 1.0};	 
	glLightfv(GL_LIGHT0, GL_POSITION, p);

	//Light2
	glEnable(GL_LIGHT2);
	GLfloat p2[]={1.0, 1.0, 0.0, 0.0};
	glLightfv(GL_LIGHT2, GL_POSITION, p2);
	GLfloat d2[]={1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, d2);
    GLfloat a2[]={0.3f,0.3f,0.3f,1.0};
    glLightfv(GL_LIGHT2, GL_AMBIENT, a2);

	//Niebla
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat color[]={0.0f,0.0f,1.0f,0.0f};
	//glFogfv(GL_FOG_COLOR, color);
	//glFogf(GL_FOG_DENSITY, 0.5f);
	glFogi(GL_FOG_START, 20);
	glFogi(GL_FOG_END, 50);

	//Texturas
	unsigned int width;
	unsigned int height;
	unsigned char * data = loadBMPRaw("./bmp/madera.bmp", width, height);
	unsigned char * data2 = loadBMPRaw("./bmp/grass.bmp", width, height);
	cout<< width << endl;
	cout<< height << endl;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Textura mesa
	glBindTexture(GL_TEXTURE_2D, t.getTexture(0));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	//Textura tapete
	glBindTexture(GL_TEXTURE_2D, t.getTexture(1));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
 }

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	
	// Drawing axes
	glBegin( GL_LINES );
		glColor3f(1.0,0.0,0.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(20, 0, 0);	     
	 
		glColor3f(0.0,1.0,0.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(0, 20, 0);	 
	 
		glColor3f(0.0,0.0,1.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 20);

		glColor3f(1.0, 0.0, 0.0);
	glEnd();

	//Our code
	escena->dibuja();

	// Light1 - Lampara
	if(lamparaOn == true){
		glEnable(GL_LIGHT1);
		GLfloat d1[]={1.0,1.0,1.0,1.0};
		glLightfv(GL_LIGHT1, GL_DIFFUSE, d1);
		GLfloat a1[]={0.3f,0.3f,0.3f,1.0};
		glLightfv(GL_LIGHT1, GL_AMBIENT, a1);
		GLfloat pos[] = {5.0, 12.0, 3.0, 1.0};
		glLightfv(GL_LIGHT1, GL_POSITION, pos);
	
		float angleLamp = calculateLampAngle(0.5, lampHeight);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleLamp);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);
		GLfloat dir[] = {0.0,-1.0,0.0};
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
		//lamparaOn = true;
	}
	
	glFlush();
	glutSwapBuffers();
}


void resize(int newWidth, int newHeight) {
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport (0, 0, WIDTH, HEIGHT) ;
   
	GLdouble SVAWidth= xRight-xLeft;
	GLdouble SVAHeight= yTop-yBot;
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {		 
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( yBot+yTop )/2.0;
		yTop= yMiddle + newHeight/2.0;
		yBot= yMiddle - newHeight/2.0;
    }
	else {      
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( xLeft+xRight )/2.0;
		xRight= xMiddle + newWidth/2.0;
		xLeft=  xMiddle - newWidth/2.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();   
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}

void key(unsigned char key, int x, int y){
	float angleLamp;

	bool need_redisplay = true;
	switch (key) {
		case 27:  /* Escape key */
			//continue_in_main_loop = false; // (**)
			//Freeglut's sentence for stopping glut's main loop (*)
			glutLeaveMainLoop (); 
			break;	

		/*case 'a': //Eje X
			angleX += 1.0;
			break;
		case 'z': //Eje X
			angleX -= 1.0;
			break;

		case 's': //Eje Y
			angleY += 1.0;
			break;
		case 'x': //Eje Y
			angleY -= 1.0;
			break;

		case 'd': //Eje Z
			angleZ += 1.0;
			break;
		case 'c': //Eje Z
			angleZ -= 1.0;
			break;*/
		case 'u': //roll
			camera.roll(0.09); //0.09 radianes = 5 grados
			break;
		case 'i':
			camera.roll(-0.09);
			break;

		case 'j': //yaw
			camera.yaw(0.01);
			break;
		case 'k':
			camera.yaw(-0.01);
			break;

		case 'n': //pitch
			camera.pitch(0.01);
			break;
		case 'm':
			camera.pitch(-0.01);
			break;

		case 'e': //Along axis X
			recorridoX = 10.0;
			recorridoY = 0.0;
			recorridoZ = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;
		case 'r':
			recorridoX = -10.0;
			recorridoY = 0.0;
			recorridoZ = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;

		case 'd': //Along axis Y
			recorridoY = 10.0;
			recorridoX = 0.0;
			recorridoZ = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;
		case 'f':
			recorridoY = -10.0;
			recorridoX = 0.0;
			recorridoZ = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;

		case 'c': //Along axis Z
			recorridoZ = 10.0;
			recorridoX = 0.0;
			recorridoY = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;
		case 'v':
			recorridoZ = -10.0;
			recorridoX = 0.0;
			recorridoY = 0.0;
			camera.recorridoEje(recorridoX, recorridoY, recorridoZ);
			break;

		case 's': //Desplazar
			desplazaY = 2.0; 
			camera.desplazar(0.0, desplazaY, 0.0);
			break;
		case 'x':
			desplazaY = -2.0;
			camera.desplazar(0.0, desplazaY, 0.0);
			break;

		case '1': //Gira X
			//angleGiraX += 0.1;
			camera.giraX(0.1);
			break;

		case '2': //Gira Y
			//angleGiraY += 0.1;
			camera.giraY(0.1);
			break;

		case '3': //Gira Z
			//angleGiraZ += 0.1;
			camera.giraZ(0.1);
			break;

		case '4': //Lateral
			camera.lateral();
			break;

		case '5': //Frontal
			camera.frontal();
			break;

		case '6': //Cenital
			camera.cenital();
			break;

		case '7': //Esquina
			camera.esquina();
			break;

		case 'o': //Ortogonal
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(xLeft, xRight, yBot, yTop, N, F);
			break;
		case 'p': //Perspectiva
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45, 1, N, F);
			break;
		case 'l': //Oblicua
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			
			d.normaliza();

			if(d.getZ() != 0.0 && (d.getX() != 0 || d.getY() != 0 || d.getZ() != 1)){
				GLfloat m[16] = {	
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					};
				m[8] = -(d.getX()) / d.getZ();
				m[9] = -(d.getY()) / d.getZ();
				m[12] = -N * (d.getX() / d.getZ());
				m[13] = -N * (d.getY() / d.getZ());

				glOrtho(xLeft, xRight, yBot, yTop, N, F);
				glMultMatrixf(m);
			}
			break;

		case 'y': //Trasladar escena
			escena->traslacionEscena(1, 0, 0);
			break;
		case 'h':
			escena->traslacionEscena(-1, 0, 0);
			break;

		case 'g': //Escalacion escena
			escena->escalacionEscena(1, 2, 1);
			break;
		case 'b':
			escena->escalacionEscena(1, 0.5, 1);
			break;

		case '8': //Rotacion escena
			escena->rotacionEscena(2,0,1,0);
			break;
		case '9':
			escena->rotacionEscena(-2,0,1,0);
			break;

		case 't': //Encender/apagar luz ambiente global
			if(globalOn){
				globalOn = false;
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHT2);
				GLfloat amb[] = {0.0,0.0,0.0,1.0};
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
			} else {
				globalOn = true;
				glEnable(GL_LIGHT0);
				glEnable(GL_LIGHT1);
				glEnable(GL_LIGHT2);
				GLfloat amb[] = {0.2,0.2,0.2,1.0};
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
			}
			break;

		case 'a': //Escalar l�mpara
			escena->getHijo(15)->getTAfin()->escalacion(1, 1.1, 1);
			escena->getHijo(15)->getTAfin()->traslacion(0, -1, 0);
			lampHeight *= 1.1;
			angleLamp = calculateLampAngle(0.5, lampHeight);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleLamp);
			break;
		case 'z':
			escena->getHijo(15)->getTAfin()->escalacion(1, 0.9, 1);
			escena->getHijo(15)->getTAfin()->traslacion(0, 1, 0);
			lampHeight *= 0.9;
			angleLamp = calculateLampAngle(0.5, lampHeight);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, angleLamp);
			break;

		case 'q': //Encender/apagar l�mpara
			if(lamparaOn){
				lamparaOn = false;
				glDisable(GL_LIGHT1);
			} else {
				lamparaOn = true;
				glEnable(GL_LIGHT1);
			}
			break;

		case 'w': // Activar/desactivar luz remota del este
			if(luzRemotaOn){
				luzRemotaOn = false;
				glDisable(GL_LIGHT2);
			} else {
				luzRemotaOn = true;
				glEnable(GL_LIGHT2);
			}
			break;
		
		case '0': // Activar/desactivar niebla
			if(nieblaOn){
				nieblaOn = false;
				glDisable(GL_FOG);
			} else {
				nieblaOn = true;
				glEnable(GL_FOG);
			}
			break;

		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}

int main(int argc, char *argv[]){
	cout<< "Starting console..." << endl;

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition (140, 140);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Freeglut 3D-project");
    
	// Callback registration
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutDisplayFunc(display);

	// OpenGL basic setting
	initGL();

	// Freeglut's main loop can be stopped executing (**)
	// while (continue_in_main_loop) glutMainLoopEvent();

	// Classic glut's main loop can be stopped after X-closing the window,
	// using the following freeglut's setting (*)
	glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION) ;
    
	// Classic glut's main loop can be stopped in freeglut using (*)
	glutMainLoop(); 
  
	// We would never reach this point using classic glut
	//system("PAUSE"); 
   
	return 0;
}
