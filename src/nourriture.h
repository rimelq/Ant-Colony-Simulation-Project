
// Nom de fichier : nourriture.h
// Auteurs : EL QABLI Rim (45%), FUMEAUX Max (45%), SCHWARTZ Maxime (10%)
// Scipers : 340997            , 326003           , 345352

#ifndef NOURRITURE_H		 
#define NOURRITURE_H

#include <cstdlib> 
#include <string>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
#include "squarecell.h"
#include "message.h"

class Nourriture 
{
public :
	Nourriture(unsigned x, unsigned y);

	unsigned get_posx() const;
	unsigned get_posy() const;

	void draw_nourriture(unsigned posx, unsigned posy);
	
private :
	Point pos;
};

Nourriture lecture_nourriture(std::ifstream & fichier, bool &error);
Nourriture new_food();
void save_food(std::fstream& outfile);
void clear_elem_N();

#endif
