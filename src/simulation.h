
// Nom de fichier : simulation.h
// Auteurs : EL QABLI Rim (33%), FUMEAUX Max (33%), SCHWARTZ Maxime (33%)
// Scipers : 340997            , 326003           , 345352

#ifndef SIMULATION_H		 
#define SIMULATION_H

#include <cstdlib> // string et vector
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "nourriture.h"
#include "fourmi.h"
#include "fourmiliere.h"

class Simulation 
{
public :
	static Simulation* get_simulation_adresse();
	Simulation ();
	~Simulation ();

	void add_new_food(Nourriture food);
	void add_fourmiliere(Fourmiliere fourmiliere); 

	void lecture(std::string nom_fichier, bool& error);

	size_t get_nombre_fourmilieres() const;	
	size_t get_nombre_nourritures() const;
	unsigned get_idF_fourmiliere(unsigned i) const; 
	void set_idF_fourmiliere(Fourmiliere& fourmiliere, unsigned idf);
	void set_idF_fourmis(Fourmiliere fourmiliere, unsigned idf);
	void get_size(int& taille) const;
	unsigned get_nbC_fourmiliere(int i) const;
	unsigned get_nbD_fourmiliere(int i) const;
	unsigned get_nbP_fourmiliere(int i) const;
	double get_nb_food_fourmiliere(int i) const;

	void fonction_food();
	void fonction_fourmiliere();
	void consommation_food();
	void death_row();
	void generator();
	void generator_x(unsigned centre_x, unsigned posGx, unsigned posGy, unsigned i);
	void generator_y(unsigned centre_y, unsigned posGx, unsigned posGy, unsigned i);
	void kill_food(Point food);
	
	Point get_id(unsigned x, unsigned y);
	bool check_if_predator(unsigned i, unsigned j);
	bool is_predator(unsigned idF, unsigned idf);
	bool check_if_defensor(unsigned i, unsigned j);
	bool is_defensor(unsigned idF, unsigned idf);
	void chemin_all_fourmilieres();
	
	bool same_coord_pred_pred(unsigned x, unsigned y, unsigned idF);
	Point get_pred_pred_id(unsigned x, unsigned y, unsigned idF);

	bool pred_within_coll(unsigned xP, unsigned yP, unsigned xC, unsigned yC);
	bool same_coord_pred_coll(unsigned x, unsigned y, unsigned idF);
	void contact_predator(unsigned i, unsigned j, Neighbors potential_predators);
	void superpo_predator(unsigned idF, unsigned idf);
	bool neighbor_defensor(unsigned x, unsigned y, unsigned idF);

	void check_touch_killers(unsigned i);
	void decimal_point(double& num);
	void simulation_algorithm();
	void save_me(std::string filename);
	void save_collectors(std::fstream& outfile, unsigned i);
	void save_defensors(std::fstream& outfile, unsigned i);
	void save_predators(std::fstream& outfile, unsigned i);
	void draw_all_nourritures();
	void draw_all_fourmilieres();
	void reset();

private :
	std::vector<Nourriture> nourritures;
	std::vector<Fourmiliere> fourmilieres; 
	
};

void clear_all();
void fonction_food();


#endif 