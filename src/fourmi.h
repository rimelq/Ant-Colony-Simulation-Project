// rendu1 public mais ne PAS diffuser

// Nom de fichier : fourmi.h
// Auteurs : EL QABLI Rim (45%), FUMEAUX Max (45%), SCHWARTZ Maxime (10%)
// Scipers : 340997            , 326003           , 345352

#ifndef FOURMI_H
#define FOURMI_H

#include <cstdlib> 
#include <string>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <memory>
#include "squarecell.h"
#include "constantes.h"

enum Categorie {COLLECTOR, DEFENSOR, PREDATOR};

class Fourmi 
{
public :
	Fourmi(std::istringstream& iss, Categorie c);
	Fourmi(unsigned x, unsigned y, unsigned age, unsigned idF, unsigned side, 
		   Categorie categorie, bool end_of_life=false);
	virtual ~Fourmi() {};

	bool error;
	
	virtual void draw_fourmi(Couleur couleur) = 0;
	virtual void chemin_fourmi(unsigned size_fourmiliere,unsigned posx, unsigned posy, 
							   unsigned posGx, unsigned posGy, bool& gagner, 
							   Point &food_eaten) = 0;
	
	void set_pos(unsigned x1, unsigned y1);
	virtual bool verif_id_predator(/*Fourmi *fourmi*/) = 0;


	void increment_age();
	unsigned get_posx() const;
	unsigned get_posy() const;
	unsigned get_side() const;
	unsigned get_age() const;
	unsigned get_idF() const;
	void set_idF(unsigned idf); 	
	Categorie get_categorie_fourmi() const;
	void end_of_life_true();
	bool get_end_of_life() const;
	virtual bool get_fourmi_dispo() const = 0;
	

	//virtual bool get_dispo() const = 0;

protected :
	Point pos;
	unsigned age;
	unsigned idF; //Numero de la fourmiliere a laquelle elle appartient
	unsigned side;
	Categorie categorie;
	bool end_of_life;
	
};

class Collector : public Fourmi 
{
public :
	Collector(unsigned x, unsigned y, unsigned age, bool dispo, unsigned idF, 
			  unsigned side=sizeC, Categorie categorie=COLLECTOR, 
			  bool end_of_life=false);
	~Collector() {};
	bool get_fourmi_dispo() const;
	
	void draw_fourmi(Couleur couleur);
	void chemin_fourmi(unsigned size_fourmiliere,
					   unsigned posx, unsigned posy, unsigned posGx, unsigned posGy, 
					   bool& gagner, Point &food_eaten);
	
	void deplacement_collector(int cgy, int cgx, int fy, int fx);
	void step_collector(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int dir_x, int dir_y);
	void step_collector_diagonale1 (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int dir_x, int dir_y);
	void step_collector_diagonale (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int dir_x, int dir_y);
	//bool verif_nourriture_collector(unsigned x, unsigned y);
	bool verif_id_predator();

private :
	bool dispo; 
};

class Defensor : public Fourmi 
{
public :
	Defensor(unsigned x, unsigned y, unsigned age, unsigned idF, unsigned side=sizeD, 
			 Categorie categorie=DEFENSOR, bool end_of_life=false);
	~Defensor() {};
	bool get_fourmi_dispo() const { return false; }

	void draw_fourmi(Couleur couleur);
	void chemin_fourmi(unsigned size_fourmiliere,
					   unsigned posx, unsigned posy, unsigned posGx, unsigned posGy,
					   bool& gagner, Point &food_eaten);
						
	bool verif_id_predator();
	void deplacement_defensor(int x1, int y1, int x2, int y2, int x3, int y3, 
							  int dir_x, int dir_y);
	
};

class Predator : public Fourmi 
{
public :
	Predator(unsigned x, unsigned y, unsigned age, unsigned idF, unsigned side=sizeP, 
			 Categorie categorie=PREDATOR, bool end_of_life=false);
	~Predator() {};
	bool get_fourmi_dispo() const { return false; }

	void draw_fourmi(Couleur couleur);
	void chemin_fourmi(unsigned size_fourmiliere,
					   unsigned posx, unsigned posy, unsigned posGx, unsigned posGy,
					   bool& gagner, Point &food_eaten);

	bool verif_id_predator();
};

bool to_bool(std::string s);

Fourmi* lecture_fourmi_1(std::istringstream& iss, Categorie c, bool &error);
Fourmi* lecture_fourmi_2(std::istringstream& iss, 
						Categorie c, Square fourmiliere, unsigned h, bool &error);

void save_defensors(std::fstream& outfile, int nbD_total, int nbD);
void save_collectors(std::fstream& outfile, int nbC_total, int nbC);
void save_predators(std::fstream& outfile, int nbP_total, int nbP);

Fourmi* nouvelle_collector(Point pos_vide, unsigned idF);
Fourmi* nouvelle_defensor(Point pos_vide, unsigned idF);
Fourmi* nouvelle_predator(Point pos_vide, unsigned idF);

static std::vector<Point> nourritureVector;
static std::vector<Fourmi*> fpredatorVector;

void add_nourritureVector(Point pos, unsigned nb_nourritures);
void add_fpredatorVector(Fourmi *fourmi);

void clear_fourmis();

#endif 