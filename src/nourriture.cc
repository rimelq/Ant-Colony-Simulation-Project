// rendu1 public mais ne PAS diffuser

// Nom de fichier : nourriture.cc
// Auteurs : EL QABLI Rim (45%), FUMEAUX Max (45%), SCHWARTZ Maxime (10%)
// Scipers : 340997            , 326003           , 345352

#include <cstdlib> 
#include <string>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "squarecell.h"
#include "message.h"
#include "nourriture.h"
#include "constantes.h"

using namespace std; 
using namespace squarecell;
using namespace message;

Nourriture::Nourriture(unsigned x, unsigned y) 
		: pos({x, y}) {}

unsigned Nourriture::get_posx() const { return pos.x; }
unsigned Nourriture::get_posy() const { return pos.y; }

Nourriture lecture_nourriture(std::ifstream& fichier, bool& error) 
{
    string line;
    unsigned x;
    unsigned y;
    Nourriture null(0,0);

	while(getline(fichier>>ws, line))
	{
		if(line[0]=='#') continue;
		istringstream iss(line);
		iss >> x >> y;
		Square f_square{1,1, x, y};
		if (valid_square(f_square)) {
			if (overlapping_squares(f_square)) {
				cout<<food_overlap(x, y);
				error=false;
				return null;
			} else {
				declare_square(f_square);
			}
		}
		if (valid_square(f_square)==false) {
			error=false;
			return null;
		}
		Nourriture food(x, y);
		return food;	
	}
	return null;	
}	
Nourriture new_food()
{
	Nourriture miam(0,0);
	bool booboo(false);
    int prob=trunc(1/food_rate);
	
    if(rand() % prob+1==1){
       booboo=true;
	}
    if(booboo){
		int i(0);
        while(i<max_food_trial)
        {
            unsigned int posx = rand() % 127+1;
            unsigned int posy = rand() % 127+1;
            Square f_square{1,1, posx, posy};
			if (overlapping_squares(f_square)){
				i++;
			}
			else{
				Nourriture food(posx, posy);
				return food;
			}
        }
    }
	return miam;
}

void Nourriture::draw_nourriture(unsigned posx, unsigned posy) 
{
	Square nourriture {true, 1, posx, posy};
	draw_square(nourriture, LOSANGE, BLANC);
}
