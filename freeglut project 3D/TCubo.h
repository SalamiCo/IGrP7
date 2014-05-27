//Pedro Morgado Alarc�n
//Samuel M�ndez Gal�n

#pragma once
#include "objetocuadrico.h"

#include "TColor.h"
#include "Textures.h"

class TCubo : public ObjetoCuadrico
{
private:
	GLfloat lado;
	GLfloat xT, yT, zT;
	GLfloat xE, yE, zE;
	TColor color;
	int type; //0: tiza, 1:borde

public:
	TCubo(void);
	TCubo(GLfloat l, GLfloat xT, GLfloat yT, GLfloat zT, GLfloat xE, GLfloat yE, GLfloat zE, TColor c, int t);
	~TCubo(void);

	virtual void dibuja();
};

