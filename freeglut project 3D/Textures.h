//Pedro Morgado Alarcón
//Samuel Méndez Galán

#pragma once
#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <GL/freeglut.h>
#include <GL/glut.h>

class Textures {
private:
	GLuint texturas[2];

public:
	Textures();
	~Textures(void){};

	GLuint getTexture(int i){return texturas[i];};
	void bindTexture(int i);
};