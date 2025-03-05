// rendu1 public mais ne PAS diffuser

// Nom de fichier : fourmiliere.h
// Auteurs : EL QABLI Rim (30%), FUMEAUX Max(40%), SCHWARTZ Maxime (30%) 
// Scipers : 340997            , 326003          , 345352

#ifndef FOURMILIERE_H
#define FOURMILIERE_H

#include <cstdlib> 
#include <vector>
#include <iostream> 
#include <fstream>
#include <sstream>
#include "squarecell.h"
#include "fourmi.h"

class Fourmiliere 
{
public :
	void add_attributs(unsigned x, unsigned y, 
					   unsigned size, unsigned gx, unsigned gy, unsigned food, 
					   unsigned nbC, unsigned nbD, unsigned nbP);
	void add_fourmi(Fourmi* fourmi);
	unsigned get_posx() const;
	unsigned get_posy() const;
	unsigned get_posGx() const;
	unsigned get_posGy() const;
	unsigned get_idF() const;
	double get_nb_food() const;
	unsigned get_nbC() const;
	unsigned get_nbD() const;
	unsigned get_nbP() const;
	unsigned get_color() const;
	unsigned get_size_fourmiliere() const;
	unsigned get_nombre_fourmis() const;
	bool get_end_of_klan() const;
	bool get_state() const;

	Fourmi* get_fourmi(size_t i);

	unsigned get_fourmi_posx(unsigned j) const; 
	unsigned get_fourmi_posy(unsigned j) const;

	unsigned get_fourmi_age(unsigned j) const;

	unsigned get_fourmi_side(unsigned j) const;
	unsigned get_fourmi_idF(unsigned j) const;
	Categorie get_categorie(unsigned j) const;
	void end_of_life_true(unsigned j);	
	
	void set_posx(unsigned posx);
	void set_posy(unsigned posy);
	void set_posGx(unsigned posGx);
	void set_posGy(unsigned posGy);
	void set_fourmiliere_idF(unsigned idf);
	void set_idF_all_fourmis(unsigned idf);
	void set_size_fourmiliere(unsigned size);
	void set_nbC(unsigned nbc);
	void set_nbD(unsigned nbd); 
	void set_nbP(unsigned nbp);
	void set_nb_food(double food);
	void set_end_of_klan(bool life);
	void set_state(bool freedom);
	void set_color(unsigned couleur);
	
	void naissance_fourmis();
	void naissance_fourmis_free(size_t i, double ratioC, double ratioD, 
								unsigned posx, unsigned posy, unsigned size);
	void naissance_fourmis_constrained(size_t i, double ratioC, double ratioD, 
							    unsigned posx, unsigned posy, unsigned size);									  
	void check_age_fourmis();
	void erase_dead_fourmis(std::vector<Point>& new_food);
	Neighbors get_not_empty_cells_1(unsigned j);
	Neighbors get_not_empty_cells_2(unsigned j);

	void draw_fourmiliere(unsigned size_fourmiliere, unsigned posx, unsigned posy);
	void fonc_sizeF(unsigned i, unsigned &taille, unsigned &posx, unsigned &posy,
					unsigned nbC, unsigned nbD, unsigned nbP, bool &state);
	void sizeF_pos(unsigned &posx, unsigned &posy,bool &state, unsigned &taille, 
				   unsigned sizeF, int diff);
	void chemin_fourmiliere(size_t i, unsigned size_fourmiliere,unsigned posx, 
							unsigned posy,unsigned nb_nourritures, Point &food_eaten);
	
private :
	Point pos;
	unsigned taille;
	Point posG;
	unsigned idF;
	double nb_food;
	unsigned nbC;
	unsigned nbD;
	unsigned nbP;
	bool end_of_klan;
	bool state; //FREE(true) CONSTRAINED(false)
	std::vector<Fourmi*> fourmis; 
	unsigned color;
};

static std::vector<Point> nourritureVec;
static std::vector<Fourmi*> fpredatorVec;

void add_nourriture(Point pos);
void add_fpredator(Fourmi *fourmi/*, unsigned nb_fpredator*/);

void check_fourmiliere(unsigned const& x, unsigned const& y,
					   unsigned const& size, unsigned& h,std::vector<Square>& elem_F);

void check_G(unsigned gx, unsigned gy, 
			 unsigned fx, unsigned fy, unsigned size, unsigned h, bool &error);  

Fourmiliere lecture_fourmiliere(std::ifstream & fichier, bool &error);

void infos_access(int i, int& nbC, int& nbD, int& nbP, double& nbF);

void save_fourmiliere(std::fstream& outfile);

void clear_elem_F();

#endif
