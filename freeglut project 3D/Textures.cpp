//Pedro Morgado Alarcón
//Samuel Méndez Galán

#include "Textures.h"

Textures::Textures(){
	glGenTextures(2, texturas);
}

void Textures::bindTexture(int i){
	glBindTexture(GL_TEXTURE_2D, texturas[i]);
}