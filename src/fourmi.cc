// rendu1 public mais ne PAS diffuser

// Nom de fichier : fourmi.cc
// Auteurs : EL QABLI Rim (30%), FUMEAUX Max (20%), SCHWARTZ Maxime (50%)
// Scipers : 340997            , 326003           , 345352

#include <cstdlib> 
#include <string>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include "constantes.h"
#include "squarecell.h"
#include "message.h"
#include "fourmi.h"

using namespace std;

namespace 
{
	vector<string> vec_collectors;
	vector<string> vec_defensors;
	vector<string> vec_predators;
}

bool to_bool(string s) 
{
	return (s == "true");
}

Fourmi::Fourmi(std::istringstream& iss, Categorie c) 
{
	lecture_fourmi_1(iss, c, error);
}

Fourmi::Fourmi(unsigned x, unsigned y, unsigned age, unsigned idF, unsigned side, 
			   Categorie categorie, bool end_of_life)
: pos({x, y}), age(age), idF(idF), side(side), categorie(categorie), 
  end_of_life(end_of_life) {}

Collector::Collector(unsigned x, unsigned y, unsigned age, bool dispo, unsigned idF, 
					 unsigned side, Categorie categorie, bool end_of_life)
: Fourmi(x, y, age, idF, side, categorie, end_of_life), dispo(dispo) {}

Defensor::Defensor(unsigned x, unsigned y, unsigned age, unsigned idF, 
				   unsigned side, Categorie categorie, bool end_of_life) 
: Fourmi(x, y, age, idF, side, categorie, end_of_life) {}

Predator::Predator(unsigned x, unsigned y, unsigned age, unsigned idF, 
				   unsigned side, Categorie categorie, bool end_of_life)
: Fourmi(x, y, age, idF, side, categorie, end_of_life) {}

void Fourmi::increment_age() { ++age; }
unsigned Fourmi::get_posx() const { return pos.x; }
unsigned Fourmi::get_posy() const { return pos.y; }
unsigned Fourmi::get_side() const { return side; }
unsigned Fourmi::get_age() const { return age; }
Categorie Fourmi::get_categorie_fourmi() const { return categorie; }
void Fourmi::end_of_life_true() { end_of_life = true; }	
bool Fourmi::get_end_of_life() const { return end_of_life; }
unsigned Fourmi::get_idF() const { return idF; }
void Fourmi::set_idF(unsigned idf) { idF=idf; } 
bool Collector::get_fourmi_dispo() const { return dispo; }
	
Fourmi* lecture_fourmi_2(std::istringstream& iss, Categorie c, 
						 Square fourmiliere, unsigned h,bool &error) {
	unsigned posx(0), posy(0), age(0);
	Fourmi* ptr_par_defaut(nullptr);
	string loaded; 
    switch(c)
    {
        case COLLECTOR: {
            iss >> posx >> posy >> age >> loaded; 
            Square c_square{true, sizeC, posx, posy};
            if(squarecell::valid_square(c_square)) {
				unsigned x,y;
				if(squarecell::overlapping_squares(c_square,x,y)) {
					cout<< message::collector_overlap(posx,posy,x,y);
					error=false;
					return 0;
				} else {
					squarecell::declare_square(c_square);
					vec_collectors.push_back(to_string(posx)+" "+to_string(posy)
											 +" "+to_string(age)+" "+"true");
					return new Collector(posx, posy, age, to_bool(loaded), h, sizeC, 
										 COLLECTOR, false);
				}
			}
			if(squarecell::valid_square(c_square)==false) {
				error=false;
				return 0;
			}
            break;
		}
        case DEFENSOR: {
            iss >> posx >> posy >> age;
            Square d_square{true, sizeD, posx, posy};
            if(squarecell::valid_square(d_square)) {
				unsigned x,y;
				if(squarecell::overlapping_squares(d_square,x,y)) {
					cout<< message::defensor_overlap(posx,posy,x,y);
					error=false;
					return 0;
				} else {
					if (!(squarecell::full_overlapping(d_square,fourmiliere,true))) {
						cout<< message::defensor_not_within_home(posx,posy,h);
						error=false;
						return 0;
					} else {
						squarecell::declare_square(d_square);
						vec_defensors.push_back({to_string(posx)+" "
												+to_string(posy)+" "+to_string(age)});
						return new Defensor(posx, posy, age, h, sizeD, DEFENSOR, false);
					}
				}
			}
			if(squarecell::valid_square(d_square)==false) {
				error=false;
				return 0;
			}
            break;
		}
        case PREDATOR: {
            iss >> posx >> posy >> age;
            Square p_square{true, sizeP, posx, posy};
            if(squarecell::valid_square(p_square)) {
				if(squarecell::overlapping_squares(p_square)) {
					cout<< message::predator_overlap(posx,posy);
					error=false;
					return 0;
				} else {
					squarecell::declare_square(p_square);
					vec_predators.push_back(to_string(posx)+" "+to_string(posy)
														+" "+to_string(age));
					return new Predator(posx, posy, age, h, sizeP, PREDATOR, false);
				}
			}
			if(squarecell::valid_square(p_square)==false) {
				error=false;
				return 0;
			}
            break;
		}
    }
    return ptr_par_defaut;
}

Fourmi* lecture_fourmi_1(std::istringstream& iss, Categorie c, bool &error)
{
	Square square{true, 0, 0, 0};
	unsigned h=0;
	return lecture_fourmi_2(iss, c, square, h, error);
}

//Drawing ants functions
void Collector::draw_fourmi(Couleur couleur) 
{
	Square fourmi {true, sizeC, pos.x, pos.y};
	if(get_fourmi_dispo() == true) {
		squarecell::draw_square(fourmi, DIAGONALE_LOSANGE, couleur);
	} else {
		squarecell::draw_square(fourmi, DIAGONALE, couleur);
	}
}

void Defensor::draw_fourmi(Couleur couleur) 
{
	Square fourmi {true, sizeD, pos.x, pos.y};
	squarecell::draw_square(fourmi, GRILLE, couleur);
}

void Predator::draw_fourmi(Couleur couleur) 
{
	Square fourmi {true, sizeP, pos.x, pos.y};
	squarecell::draw_square(fourmi, UNIFORME, couleur);
}

//Naissance des fourmis
Fourmi* nouvelle_collector(Point pos_vide, unsigned idF)
{
	Square sq_collector {true, sizeC, pos_vide.x, pos_vide.y};
	squarecell::declare_square(sq_collector);

	return new Collector(pos_vide.x, pos_vide.y, 1, false, idF, sizeC, COLLECTOR, 
						 false);
}

Fourmi* nouvelle_defensor(Point pos_vide, unsigned idF)
{
	Square sq_defensor {true, sizeD, pos_vide.x, pos_vide.y};
	squarecell::declare_square(sq_defensor);

	return new Defensor(pos_vide.x, pos_vide.y, 1, idF, sizeD, DEFENSOR, false);
}

Fourmi* nouvelle_predator(Point pos_vide, unsigned idF)
{
	Square sq_predator {true, sizeP, pos_vide.x, pos_vide.y};
	squarecell::declare_square(sq_predator);

	return new Predator(pos_vide.x, pos_vide.y, 1, idF, sizeP, PREDATOR, false);
}

bool Collector::verif_id_predator() { return true; }
bool Defensor::verif_id_predator() { return false; }
bool Predator::verif_id_predator() { return true; }

void add_nourritureVector(Point pos, unsigned nb_nourritures)
{
	if (nourritureVector.size() != nb_nourritures) {
		nourritureVector.push_back(pos);
	}
}

void add_fpredatorVector(Fourmi *fourmi)
{
	if (fourmi->verif_id_predator()) {
		fpredatorVector.push_back(fourmi);
	}
}

bool verif_chemin_defensor(unsigned x1, unsigned y1, unsigned x2, unsigned y2, 
						   unsigned x3, unsigned y3) {
	bool verif_def(false);
	verif_def = squarecell::detect_full_cell(x1,y1);
	if (verif_def == false) {
		verif_def = squarecell::detect_full_cell(x2,y2);
		if (verif_def == false) {
			verif_def = squarecell::detect_full_cell(x3,y3);
		}
	}
	return verif_def;
}

bool verif_chemin_collector(int x1, int y1, int x2, int y2, int x3, int y3,
							int x4, int y4, int x5, int y5)
{
	bool verif_col(false);
	verif_col = squarecell::detect_full_cell(x1,y1);
	if (verif_col == false) {
		verif_col = squarecell::detect_full_cell(x2,y2);
		if (verif_col == false) {
			verif_col = squarecell::detect_full_cell(x3,y3);
			if (verif_col == false) {
				verif_col = squarecell::detect_full_cell(x4,y4);
				if (verif_col == false) {
					verif_col = squarecell::detect_full_cell(x5,y5);
				}
			}
		}
	}
	return verif_col;
}

bool verif_chemin_collector1(int x1, int y1, int x2, int y2, int x3, int y3,
							 int x4, int y4) 
{	
	bool verif_col(false);
	verif_col = squarecell::detect_full_cell(x1,y1);
	if (verif_col == false) {
		verif_col = squarecell::detect_full_cell(x2,y2);
		if (verif_col == false) {
			verif_col = squarecell::detect_full_cell(x3,y3);
			if (verif_col == false) {
				verif_col = squarecell::detect_full_cell(x4,y4);
			}
		}
	}
	return verif_col;
}

int choice_chemin (unsigned x_n, unsigned y_n, Square square, int dir_x, int dir_y) 
{
	cout << "Dans choice chemin" << endl;
	
	int compteur(0), xn(x_n), yn(y_n),sx(square.origin.x), sy(square.origin.y);
	int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
	if ( (dir_x == -1) and (dir_y == 1) ) {
		x1 = 0; y1 = 2;	x2 = -1;  y2 = 2;
		x3 = -2; y3 = 2; x4 = -2; y4 = 1;
		x5 = -2; y5 = 0;
	} 
	if ( (dir_x == 1) and (dir_y == 1) ) {
		x1 = 0; y1 = 2;	x2 = 1; y2 = 2;
		x3 = 2; y3 = 2; x4 = 2; y4 = 1;
		x5 = 2; y5 = 0;
	}
	if ( (dir_x == 1) and (dir_y == -1) ) {
		x1 = 2; y1 = 0; x2 = 2; y2 = -1;
		x3 = 2; y3 = -2; x4 = 1; y4 = -2;
		x5 = 0; y5 = -2;
	}
	if ( (dir_x == -1) and (dir_y == -1) ) {
		x1 = -2; y1 = 0; x2 = -2; y2 = -1;
		x3 = -2; y3 = -2; x4 = -1; y4 = -2;
		x5 = 0; y5 = -2;
	}
	while ( (abs(yn - sy) != abs(xn - sx)) ) {
		if((sy == 1) or (sy == 125) or (sx == 1) or (sx == 125)) return compteur;

		if (verif_chemin_collector(sx+x1, sy+y1, sx+x2, sy+y2, sx+x3, sy+y3,
					 sx+x4, sy+y4, sx+x5, sy+y5)) {
			++compteur;
		}
		sy += dir_y;
		sx += dir_x;
		if((sy == 1) or (sy == 125) or (sx == 1) or (sx == 125)) return compteur;
	} return compteur;
}

void Fourmi::set_pos(unsigned x1, unsigned y1)
{
	Square ancien {true, 3, pos.x, pos.y};
	squarecell::delete_square(ancien);
	
	pos.x += x1;
	pos.y += y1;
	
	Square nouveau {true, 3, pos.x, pos.y};
	squarecell::declare_square(nouveau);
}


void Defensor::deplacement_defensor (int x1, int y1, int x2, int y2, int x3, int y3, int dir_x, int dir_y)
{
	bool verif_def(false);
	verif_def = verif_chemin_defensor(pos.x+x1, pos.y+y1, pos.x+x2, pos.y+y2, 
												  pos.x+x3, pos.y+y3);
	if (verif_def) { set_pos(0,0); }
	else { set_pos(dir_x,dir_y); }
}



void Defensor::chemin_fourmi(unsigned size_fourmiliere, unsigned posx, unsigned posy, 
							 unsigned posGx, unsigned posGy, bool& gagner, Point& food_eaten)
{
	Square D_square({1, sizeD, pos.x, pos.y});
	Square F_square({0, size_fourmiliere, posx, posy});
	if(squarecell::full_overlapping(D_square, F_square,true)==false) {
		this->end_of_life_true();
	}
	if ((posy + size_fourmiliere - pos.y) == 3) {
	} else if ((pos.y - posy) == 2) {
	} else if ((pos.x - posx) == 2) {
	} else if ((posx + size_fourmiliere - pos.x) == 3) {
	} else if (posy + size_fourmiliere - pos.y <= pos.y - posy) {
		if (pos.x - posx <= posx + size_fourmiliere - pos.x) {
			if (posy + size_fourmiliere - pos.y <= pos.x - posx) {
				deplacement_defensor(-1,2,0,2,1,2,0,1);
			} else {
				deplacement_defensor(-2,1,-2,0,-2,-1,-1,0);
			}
		} else {
			if (posy + size_fourmiliere - pos.y <= posx + size_fourmiliere - pos.x) {
				deplacement_defensor(-1,2,0,2,1,2,0,1);
			} else {
				deplacement_defensor(2,1,2,0,2,-1,1,0);
			}
		}
	} else {
		if (pos.x - posx <= posx + size_fourmiliere - pos.x) {
			if (pos.y - posy <= pos.x - posx) {
				deplacement_defensor(-1,-2,0,-2,1,-2,0,-1);
			} else {
				deplacement_defensor(-2,1,-2,0,-2,-1,-1,0);
			}
		} else {
			if (pos.y - posy <= posx + size_fourmiliere - pos.x) {
				deplacement_defensor(-1,-2,0,-2,1,-2,0,-1);
			} else {
				deplacement_defensor(2,1,2,0,2,-1,1,0);
			}
		}
	}
}

void Collector::step_collector (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int dir_x, int dir_y)
{
	bool verif_def(false);
	verif_def = verif_chemin_collector(pos.x+x1, pos.y+y1, pos.x+x2, pos.y+y2, 
										pos.x+x3, pos.y+y3, pos.x+x4, pos.y+y4, pos.x+x5, pos.y+y5);
	if (verif_def) { set_pos(0,0); }
	else { set_pos(dir_x,dir_y); }
}

void Collector::deplacement_collector(int cgy, int cgx, int fy, int fx) 
{
	if (cgy < -cgx + fy + fx) {
		if (cgy > cgx + fy - fx -1) {
			int comptA(0);
			int comptD(0);
			if (comptA <= comptD) {
				if ( pos.x == 3 ) { step_collector(0,2,1,2,2,2,2,1,2,0,1,1);
				} else if (pos.y == 124) { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);
				} else { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,1); }
			} else {
				if (pos.y == 3) { step_collector(0,2,-1,2,-2,2,-2,1,-2,0,-1,1);
				} else if ( pos.x == 3) { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1);
				} else { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1); }
			}
		} else {
			int comptD(0);
			int comptC(0);
			Square squareD {true, 3, pos.x -1, pos.y -1};
			Square squareC {true, 3, pos.x +1, pos.y -1};
			if ( (pos.x != 1) and (pos.x != 125) and (pos.y != 1) and (pos.y != 125) ) {
				comptD = choice_chemin(cgy, cgx, squareD, -1, -1);
				comptC = choice_chemin(cgy, cgx, squareC, 1, -1);
			}
			if (comptD <= comptC) {
				if (pos.x == 3) { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1);
				} else if (pos.y == 3) { step_collector(0,2,-1,2,-2,2,-2,1,-2,0,-1,1);
				} else { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1); }
			} else {
				if (pos.y == 3) { step_collector(0,2,1,2,2,2,2,1,2,0,1,1);
				} else if (pos.x == 124) { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);
				} else { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1); }
			}
		}
	} else if (cgy > -cgx + fy + fx -1) {
		if (cgy > cgx + fy - fx -1) {
			int comptA(0);
			int comptB(0);
			Square squareA {true, 3, pos.x -1, pos.y +1};
			Square squareB {true, 3, pos.x +1, pos.y +1};
			if ( (pos.x != 1) and (pos.x != 125) and (pos.y != 1) and (pos.y != 125) ) {
				comptA = choice_chemin(cgy, cgx, squareA, -1, 1);
				comptB = choice_chemin(cgy, cgx, squareB, +1, +1);
			}
			if (comptA <= comptB) { 
				if ( pos.x == 3 ) { step_collector(0,2,1,2,2,2,2,1,2,0,1,1);
				} else if (pos.y == 124) { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);
				} else { step_collector(0,2,-1,2,-2,2,-2,1,-2,0,-1,1); }
			} else { 
				if (pos.x == 124) { step_collector(0,2,-1,2,-2,2,-2,1,-2,0,-1,1);
				} else if (pos.y == 124) { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1);
				} else { step_collector(0,2,1,2,2,2,2,1,2,0,1,1); }
			}
		} else {
			int comptB(0);
			int comptC(0);
			Square squareB {true, 3, pos.x +1, pos.y +1};
			Square squareC {true, 3, pos.x +1, pos.y -1};
			if ( (pos.x != 1) and (pos.x != 126) and (pos.y != 1) and (pos.y != 126) ) {
				comptB = choice_chemin(cgy, cgx, squareB, +1, +1);
				comptC = choice_chemin(cgy, cgx, squareC, 1, -1);
			}
			if (comptC <= comptB) {
				if (pos.y == 3) { step_collector(0,2,1,2,2,2,2,1,2,0,1,1);
				} else if (pos.x == 124) { step_collector(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);
				} else { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1); }
			} else {
				if (pos.x == 124) { step_collector(0,2,-1,2,-2,2,-2,1,-2,0,-1,1);
				} else if (pos.y == 124) { step_collector(2,0,2,-1,2,-2,1,-2,0,-2,1,-1);
				} else { step_collector(0,2,1,2,2,2,2,1,2,0,1,1); } 
			}
		}
	}
}

void Predator::chemin_fourmi(unsigned size_fourmiliere, unsigned posx, unsigned posy,
							 unsigned posGx, unsigned posGy, bool& gagner, Point& food_eaten)
{
	static vector<double> fpredator_distance;
	static vector<double> futures_distances;
	static vector<double> futures_distances_tri;
	static vector<Point> futurs_deplacements;
	
	cout << "fpredatorVector.size() : " << fpredatorVector.size() << endl;
	if ( fpredatorVector.size() != 0) {
		for (unsigned r(0) ; r < fpredatorVector.size() ; ++r) {
			unsigned fp_x (fpredatorVector[r]->get_posx());
			unsigned fp_y (fpredatorVector[r]->get_posy());
			double distance(0);
			distance = sqrt((pos.x-fp_x)*(pos.x-fp_x)+(pos.y-fp_y)*(pos.y-fp_y));
			fpredator_distance.push_back(distance);
		}
		int p(0);
		
		for (unsigned o(0) ; o < fpredator_distance.size() ; o++) {
			if (o == 0) { p = 0; }
			else if (fpredator_distance[o] < fpredator_distance[p]) {
				p = o;
			}
		}
		unsigned f_x (fpredatorVector[p]->get_posx());
		unsigned f_y (fpredatorVector[p]->get_posy());
		
		futures_distances.push_back( sqrt(((pos.x +1) - f_x)*((pos.x +1) - f_x) + ((pos.y +2) - f_y)*((pos.y +2) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x +2) - f_x)*((pos.x +2) - f_x) + ((pos.y +1) - f_y)*((pos.y +1) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x +2) - f_x)*((pos.x +2) - f_x) + ((pos.y -1) - f_y)*((pos.y -1) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x +1) - f_x)*((pos.x +1) - f_x) + ((pos.y -2) - f_y)*((pos.y -2) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x -1) - f_x)*((pos.x -1) - f_x) + ((pos.y -2) - f_y)*((pos.y -2) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x -2) - f_x)*((pos.x -2) - f_x) + ((pos.y -1) - f_y)*((pos.y -1) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x -2) - f_x)*((pos.x -2) - f_x) + ((pos.y +1) - f_y)*((pos.y +1) - f_y) ) );
		futures_distances.push_back( sqrt(((pos.x -1) - f_x)*((pos.x -1) - f_x) + ((pos.y +2) - f_y)*((pos.y +2) - f_y) ) );
		
		for (unsigned m(0); m < futures_distances.size(); m++) {
			cout << futures_distances[m] << endl;
		}
		
		int pf(0);
		for (unsigned o(0) ; o < futures_distances.size() ; o++) {
			if (o == 0) { pf = 0; }
			else if (futures_distances[o] < futures_distances[p]) {
				pf = o;
			}
		}
		if (pf == 0) {
			if ( (pos.x+1 <= 126) and (pos.x +1 > 0) ) {
				if ( (pos.y+2 <= 126) and (pos.y+2 > 0) ) {
					set_pos(1,2);
				}
			}
		}
		if (pf == 1) {
			if ( (pos.x+2 <= 126) and (pos.x +2 > 0) ) {
				if ( (pos.y+1 <= 126) and (pos.y+1 > 0) ) {
					set_pos(2,1);
				}
			}
		}
		if (pf == 2) { 
			if ( (pos.x+2 <= 126) and (pos.x +2 > 0) ) {
				if ( (pos.y-1 <= 126) and (pos.y-1 > 0) ) {
					set_pos(2,-1);
				}
			}
		}
		if (pf == 3) { 
			if ( (pos.x+1 <= 126) and (pos.x +1 > 0) ) {
				if ( (pos.y-2 <= 126) and (pos.y-2 > 0) ) {
					set_pos(1,-2);
				}
			}
		}
		if (pf == 4) { 
			if ( (pos.x-1 <= 126) and (pos.x -1 > 0) ) {
				if ( (pos.y-2 <= 126) and (pos.y-2 > 0) ) {
					set_pos(-1,-2);
				}
			}
		}
		if (pf == 5) { 
			if ( (pos.x-2 <= 126) and (pos.x -2 > 0) ) {
				if ( (pos.y-1 <= 126) and (pos.y-1 > 0) ) {
					set_pos(-2,-1);
				}
			}
		}
		if (pf == 6) { 
			if ( (pos.x-2 <= 126) and (pos.x -2 > 0) ) {
				if ( (pos.y+1 <= 126) and (pos.y+1 > 0) ) {
					set_pos(-2,1);
				}
			}
			
		}
		if (pf == 7) { 
			
			if ( (pos.x-1 <= 126) and (pos.x -1 > 0) ) {
				if ( (pos.y+2 <= 126) and (pos.y+2 > 0) ) {
					set_pos(-1,2);
				}
			}
		}
		futurs_deplacements.clear();
		futures_distances_tri.clear();
		futures_distances.clear();
		fpredator_distance.clear();
		fpredatorVector.clear();
	}
}



void Collector::step_collector_diagonale1 (int x1, int y1, int x2, int y2,
											 int x3, int y3, int x4, int y4,
											  int dir_x, int dir_y)
{
	bool verif_direction(false);
	verif_direction = verif_chemin_collector1(pos.x+x1, pos.y+y1, pos.x+x2,
											 pos.y+y2, pos.x+x3, pos.y+y3, 
											 pos.x+x4, pos.y+y4);
	if (verif_direction == false) {
		set_pos(dir_x,dir_y);
	} else {}
}

void Collector::step_collector_diagonale (int x1, int y1, int x2, int y2, int x3,
										 int y3, int x4, int y4, int x5,
										  int y5, int dir_x, int dir_y)
{
	bool verif_direction(false);
	verif_direction = verif_chemin_collector(pos.x+x1, pos.y+y1, pos.x+x2, pos.y+y2,
											 pos.x+x3, pos.y+y3, pos.x+x4, pos.y+y4, 
											 pos.x+x5, pos.y+y5);
	if (verif_direction == false) {
		set_pos(dir_x,dir_y);
	} else {}
}

void Collector::chemin_fourmi(unsigned size_fourmiliere, unsigned posx, unsigned posy, 
							unsigned posGx, unsigned posGy, bool& gagner, Point &food_eaten)
{
	int w(0);
	vector<Point> nourriture_accessible;
	vector<unsigned> nourr_vxvy;
	nourr_vxvy.clear();
	nourriture_accessible.clear();
	if(nourritureVector.size()!=0)
	{
		for (unsigned u(0); u < nourritureVector.size() ; ++u) {
			if (((pos.x - pos.y)%2) == ((nourritureVector[u].x - nourritureVector[u].y)%2) ) {
				nourriture_accessible.push_back(nourritureVector[u]);
			} else {}
		}
	}
	if(nourriture_accessible.size()!=0)
	{
		for (unsigned h(0) ; h < nourriture_accessible.size() ; ++h) {
			int vx(pos.x - nourriture_accessible[h].x);
			int vy(pos.y - nourriture_accessible[h].y);
			vx = abs(vx);
			vy = abs(vy);
			if (vx >= vy) {
				nourr_vxvy.push_back(vx);
			} else { nourr_vxvy.push_back(vy); }
		}
		for (unsigned o(0) ; o < nourr_vxvy.size() ; o++) {
			if (o == 0) { w = 0; }
			else if (nourr_vxvy[o] < nourr_vxvy[w]) {
				w = o;
			}
		}				//LA NOURRITURE LA PLUS PROCHE EST DONC LA NOURRITURE D'INDICE W DANS LE VECTOR NOURRITURE_ACCESSIBLE
		
		if ( (pos.x-1 == nourriture_accessible[w].x) and (pos.y+1 == nourriture_accessible[w].y) ) {
			food_eaten={nourriture_accessible[w].x,nourriture_accessible[w].y};
			dispo = true;
		}
		if ( (pos.x+1 == nourriture_accessible[w].x) and (pos.y+1 == nourriture_accessible[w].y) ) {
			food_eaten={nourriture_accessible[w].x,nourriture_accessible[w].y};
			dispo = true;
		}
		if ( (pos.x+1 == nourriture_accessible[w].x) and (pos.y-1 == nourriture_accessible[w].y) ) {
			food_eaten={nourriture_accessible[w].x,nourriture_accessible[w].y};
			dispo = true;
		}
		if ( (pos.x-1 == nourriture_accessible[w].x) and (pos.y-1 == nourriture_accessible[w].y) ) {
			food_eaten={nourriture_accessible[w].x,nourriture_accessible[w].y};
			dispo = true;
		}
		
		if ( ((pos.y - size_fourmiliere - posy) == 1) and (posx - 2 < pos.x) and (pos.x < (posx + size_fourmiliere) + 2) ) {
			dispo = false;
			gagner=true;
		} else if ( ((posx - pos.x) == 2) and (posy - 2 < pos.y) and (pos.y < (posy + size_fourmiliere) + 2) ) {
			dispo = false;
		} else if ( ((posy - pos.y) == 2) and (posx - 2 < pos.x) and (pos.y < (posx + size_fourmiliere) + 2) ) {
			dispo = false;
			gagner=true;
		} else if (((pos.x-posx-size_fourmiliere)==1)and(posy-2<pos.y)and(pos.y<(posy+size_fourmiliere)+2)){
			dispo = false;
			gagner=true;
		}
		if (dispo) {
			int cgx(0);
			int cgy(posGy);
			if ( (pos.x - pos.y)%2 == (posGx - posGy)%2 ) {
				cgx = posGx;
			} else {
				cgx = posGx + 1;
			}
			int fx (pos.x);
			int fy (pos.y);
			if ( abs(fy-cgy) == abs(fx-cgx) ) {
				if ( cgy > fy) {
					if ( cgx > fx ) {
						if ( (cgy == fy+2) and (cgx == fx+2) ) {
							step_collector_diagonale1(0,2,1,2,2,1,2,0,1,1);
						} else { step_collector_diagonale(0,2,1,2,2,2,2,1,2,0,1,1);}
					} else {
						if ( (cgy == fy+2) and (cgx == fx-2)) {
							step_collector_diagonale1(-2,0,-2,1,-1,2,0,2,-1,1);
						} else { step_collector_diagonale(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,1);}
					}
				}
				else {
					if (cgx > fx) {
						if ( (cgy == fy-2) and (cgx == fx+2) ) {
							step_collector_diagonale1(0,-2,1,-2,2,-1,2,0,1,-1);
						} else { step_collector_diagonale(2,0,2,-1,2,-2,1,-2,0,-2,1,-1);}
					} else {
						if ( (cgy == fy-2) and (cgx == fx-2) ) {
							step_collector_diagonale1(-2,0,-2,-1,-1,-2,0,-2,-1,-1);
						} else { step_collector_diagonale(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);}
					}
				}
			} else {deplacement_collector(cgy,cgx,fy,fx);}
		} else {
			int nx(nourriture_accessible[w].x);
			int ny(nourriture_accessible[w].y);
			int fy(pos.y);
			int fx(pos.x);
			if ( abs(fy-ny) == abs(fx-nx) ) {
				if ( nourriture_accessible[w].y > pos.y) {
					if ( nourriture_accessible[w].x > pos.x ) {
						if ( (ny == fy+2) and (nx == fx+2) ) {
							step_collector_diagonale1(0,2,1,2,2,1,2,0,1,1);
						} else { step_collector_diagonale(0,2,1,2,2,2,2,1,2,0,1,1); }
					} else {
						if ( (ny == fy+2) and (nx == fx-2) ) {
							step_collector_diagonale1(-2,0,-2,1,-1,2,0,2,-1,1);
						} else { step_collector_diagonale(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,1);}
					}
				}
				else {
					if (nourriture_accessible[w].x > pos.x) {
						if ( (nx == fx+2) and (ny == fy-2) ) {
							step_collector_diagonale1(0,-2,1,-2,2,-1,2,0,1,-1);
						} else { step_collector_diagonale(2,0,2,-1,2,-2,1,-2,0,-2,1,-1); }
					} else {
						if ( (nx == fx-2) and (ny == fy-2) ) {
							step_collector_diagonale1(-2,0,-2,-1,-1,-2,0,-2,-1,-1);
						} else { step_collector_diagonale(-2,0,-2,-1,-2,-2,-1,-2,0,-2,-1,-1);}
					}
				}
			} else { deplacement_collector(ny,nx,fy,fx);
			}
		}
	}
	
}	

void clear_fourmis()
{	
	vec_collectors.clear();
	vec_defensors.clear();
	vec_predators.clear();
}

