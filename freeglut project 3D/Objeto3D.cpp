//Pedro Morgado Alarc�n
//Samuel M�ndez Gal�n

#include "Objeto3D.h"

Objeto3D::Objeto3D(){
	this->tAfin = new TAfin();
}

Objeto3D::Objeto3D(TAfin* ta)
{
	this->tAfin = ta;
}