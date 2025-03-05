// rendu1 public mais ne PAS diffuser

// Nom de fichier : fourmiliere.cc
// Auteurs : EL QABLI Rim (45%), FUMEAUX Max(45%), SCHWARTZ Maxime (10%) 
// Scipers : 340997            , 326003          , 345352

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector> 
#include <fstream>
#include <sstream>
#include <random>
#include "constantes.h"
#include "squarecell.h"
#include "message.h"
#include "fourmi.h"
#include "fourmiliere.h"

using namespace std;
using namespace squarecell;
using namespace message;

enum Etat_lecture {NB_ELEM, lectC, lectD ,lectP ,FIN};

static std::vector<Square> elem_F; //tableau contenant les square fourmilière validés

static Etat_lecture next(size_t nbc, size_t nbd, size_t nbp) {
	return (nbc>0)? lectC:((nbd>0)? lectD:((nbp>0)? lectP:FIN));
}

void Fourmiliere::add_attributs(unsigned x, unsigned y, unsigned size, unsigned gx,
								unsigned gy, unsigned food, unsigned nbc,
								unsigned nbd, unsigned nbp)
{
	pos = {x,y};
	taille = size;
	posG = {gx, gy};
	nb_food = food;
	nbC=nbc;
	nbD=nbd;
	nbP=nbp;
}
	
void Fourmiliere::add_fourmi(Fourmi* fourmi) {
	fourmis.push_back(fourmi);
}

unsigned Fourmiliere::get_posx() const { return pos.x; }
unsigned Fourmiliere::get_posy() const { return pos.y; }
unsigned Fourmiliere::get_posGx() const { return posG.x; }
unsigned Fourmiliere::get_posGy() const { return posG.y; }
unsigned Fourmiliere::get_idF() const { return idF; }
double Fourmiliere::get_nb_food() const { return nb_food; }
unsigned Fourmiliere::get_nbC() const { return nbC; }
unsigned Fourmiliere::get_nbD() const { return nbD; }
unsigned Fourmiliere::get_nbP() const { return nbP; }
unsigned Fourmiliere::get_color() const { return color; }
unsigned Fourmiliere::get_size_fourmiliere() const { return taille; }
unsigned Fourmiliere::get_nombre_fourmis() const { return fourmis.size(); }
bool Fourmiliere::get_end_of_klan() const { return end_of_klan; }
bool Fourmiliere::get_state() const { return state; }


unsigned Fourmiliere::get_fourmi_posx(unsigned j) const 
{
	return fourmis[j]->get_posx(); 
}
unsigned Fourmiliere::get_fourmi_posy(unsigned j) const 
{
	return fourmis[j]->get_posy(); 
}
unsigned Fourmiliere::get_fourmi_age(unsigned j) const 
{ 	
	return fourmis[j]->get_age(); 
}
unsigned Fourmiliere::get_fourmi_side(unsigned j) const 
{ 
	return fourmis[j]->get_side(); 
}
unsigned Fourmiliere::get_fourmi_idF(unsigned j) const 
{ 
	return fourmis[j]->get_idF(); 
}
Fourmi* Fourmiliere::get_fourmi(size_t i)
{
	return fourmis[i];
}
Categorie Fourmiliere::get_categorie(unsigned j) const 
{ 
	return fourmis[j]->get_categorie_fourmi();
}
void Fourmiliere::end_of_life_true(unsigned j) { fourmis[j]->end_of_life_true(); }

void Fourmiliere::set_posx(unsigned posx) { pos.x=posx; }
void Fourmiliere::set_posy(unsigned posy) { pos.y=posy; }
void Fourmiliere::set_posGx(unsigned posGx) { posG.x=posGx; }
void Fourmiliere::set_posGy(unsigned posGy) { posG.y=posGy; }
void Fourmiliere::set_fourmiliere_idF(unsigned idf) { idF=idf; }

void Fourmiliere::set_idF_all_fourmis(unsigned idf) 
{
	for(size_t j(0); j < get_nombre_fourmis(); ++j) {
		fourmis[j]->set_idF(idf);	
	}
}
void Fourmiliere::set_nbC(unsigned nbc) { nbC=nbc; }
void Fourmiliere::set_nbD(unsigned nbd) { nbD=nbd; }
void Fourmiliere::set_nbP(unsigned nbp) { nbP=nbp; }
void Fourmiliere::set_size_fourmiliere(unsigned size) { taille=size; }
void Fourmiliere::set_nb_food(double miam) { nb_food=miam; }
void Fourmiliere::set_end_of_klan(bool life) { end_of_klan=life; }
void Fourmiliere::set_state(bool freedom) { state=freedom; }
void Fourmiliere::set_color(unsigned couleur) { color=couleur; }


void check_fourmiliere(unsigned const& x, unsigned const& y, unsigned const& size,
					   unsigned& h, vector<Square>& elem_F, bool &error) {
	Fourmiliere F;
	Square fourmiliere{0, size, x, y}; 
	if(valid_square(fourmiliere)==false){
		error=false;
		return ;
	}
	if (elem_F.size() == 0) {
		elem_F.push_back(fourmiliere);
	} else {
		for (size_t i(0); i < elem_F.size(); ++i) {
			if (overlapping_squares(elem_F[i], fourmiliere,false)) {
				cout << homes_overlap(i, i+1);
				error=false;
				return ;
			} 
		}
		elem_F.push_back(fourmiliere);
	}
	++h;
}

void check_G(unsigned gx, unsigned gy,unsigned fx, unsigned fy, unsigned size,
			 unsigned h,bool &error)
{ 
	Square G_square({1, sizeG, gx, gy});
	Square F_square({0, size, fx, fy});

	if (valid_square(G_square)) {
		/// (1) check avec la grille => DOIT RENVOYER POSITION DE SUPERPOSITION
		unsigned x,y;
		if (overlapping_squares(G_square,x,y)) { 
			cout << generator_overlap(gx, gy, x, y);
			error=false;
			return ;
		}
		/// (2) check dans la fourmilière => SUPERPOSITION pour CHAQUE carré
		if(full_overlapping(G_square, F_square,true)==false) {
			cout<<generator_not_within_home(gx, gy, h);
			error=false;
			return ;
		}
		declare_square(G_square);
	}

	if (valid_square(G_square) == false) {
		error=false;
		return ;
	}
	
}

Fourmiliere lecture_fourmiliere(std::ifstream & fichier,bool &error) { 
	unsigned posx(0), posy(0), size(0), posGx(0), posGy(0), nbF(0);
	unsigned nbC(0), nbD(0), nbP(0), h(0);
	Fourmiliere F; 
	string line;
	Etat_lecture etat(NB_ELEM);
	while(getline(fichier>>ws, line)) {
		if(line[0]=='#') continue;
		istringstream iss(line);
		switch(etat) {
			case NB_ELEM:
				iss >>posx >>posy >>size >>posGx >>posGy >>nbF >>nbC >>nbD >>nbP;
				check_fourmiliere(posx, posy, size, h, elem_F,error);
				if(error==false) return F;
				check_G(posGx, posGy, posx, posy, size, h, error);
				if(error==false) return F;
				F.set_end_of_klan(false);
				F.add_attributs(posx, posy, size, posGx, posGy, nbF, nbC, nbD, nbP);
				break;

			case lectC:
				F.add_fourmi(lecture_fourmi_1(iss, COLLECTOR, error));
				if(error==false) return F;
				--nbC; break;
			case lectD: {
				Square fourmiliere {false,size,posx,posy};
				F.add_fourmi(lecture_fourmi_2(iss,DEFENSOR,fourmiliere,h,error));
				if(error==false) return F;
				--nbD; break;
			}
			case lectP:
				F.add_fourmi(lecture_fourmi_1(iss, PREDATOR, error));
				if(error==false) return F;
				--nbP; break;
			case FIN:
			default:  cout << "Erreur lors du décodage fourmilière" << endl;
		} etat = next(nbC, nbD, nbP);
		if(etat == FIN) return F; 
	} return F; 
}

void Fourmiliere::fonc_sizeF(unsigned i, unsigned &taille, unsigned &posx,
							 unsigned &posy, unsigned nbC, unsigned nbD,
							 unsigned nbP, bool &state)
{
	double somme=pow(sizeG,2) + pow(sizeC,2)*nbC + pow(sizeD,2)*nbD + pow(sizeP,2)*nbP;
	double size=sqrt(4*(somme));
    unsigned sizeF = (unsigned int) size;
	int diff=sizeF-taille;
	sizeF=sizeF+2;
	if(diff==0){
		return;
	}
	if(diff>0)
	{
		sizeF_pos(posx, posy, state, taille, sizeF, diff);

	}
	if(diff<0)
	{
		Square fourmiliere4{0, sizeF, posx, posy}; 
		if(valid_square_size(fourmiliere4)){
			taille=sizeF;
			state=true;
			return;
		}
	}
}

void Fourmiliere::sizeF_pos(unsigned &posx, unsigned &posy,bool &state, 
							unsigned &taille, unsigned sizeF, int diff)
{
	Square fourmiliere{0, sizeF, posx, posy}; 
	if(valid_square_size(fourmiliere)) {
		taille=sizeF;
		state=true;
		return;
	}
	if(valid_square_size(fourmiliere)==false){
		Square fourmiliere1{0, sizeF, posx, posy-diff};
		if(valid_square_size(fourmiliere1)){
			taille=sizeF;
			posy=posy-diff;
			state=true;
			return;
		}
		if(valid_square_size(fourmiliere1)==false){
			Square fourmiliere2{0, sizeF, posx-diff, posy-diff};
			if(valid_square_size(fourmiliere2)) {
				taille=sizeF;
				posx=posx-diff;
				posy=posy-diff;
				state=true;
				return;
			}
			if(valid_square_size(fourmiliere2)==false){
				Square fourmiliere3{0, sizeF, posx-diff, posy};
				if(valid_square_size(fourmiliere3)){
					taille=sizeF;
					posx=posx-diff;
					state=true;
					return;
				}
				if(valid_square_size(fourmiliere3)==false){
					state=false;
					return;
				}
			}
		}
	}
}

void add_nourriture(Point pos) 
{
	nourritureVec.push_back(pos);
}

void add_fpredator(Fourmi *fourmi) 
{
	fpredatorVec.push_back(fourmi);
}

void Fourmiliere::draw_fourmiliere(unsigned size_fourmiliere, 
					  			   unsigned posx, unsigned posy) 
{
	Square fourmiliere {false, size_fourmiliere, posx, posy};
	draw_square(fourmiliere, VIDE, to_color(get_idF()%6));

	Square generator {true, sizeG, get_posGx(), get_posGy()};
	draw_square(generator, UNIFORME, to_color(get_idF()%6));
	
	for(size_t j(0); j < get_nombre_fourmis(); ++j) {
		fourmis[j]->draw_fourmi(to_color((fourmis[j]->get_idF())%6));	
	}
}

void Fourmiliere::chemin_fourmiliere(size_t i, unsigned size_fourmiliere,
							unsigned posx, unsigned posy, unsigned nb_nourritures,
							Point &food_eaten)
{
	nourritureVec.clear();
	nourritureVector.clear();

	for (size_t b(0); b < nb_nourritures ; b++) {
		Point pos_nour = {nourritureVec[b].x,nourritureVec[b].y};
		add_nourritureVector(pos_nour, nb_nourritures);
	}
	for(size_t c(0); c < fpredatorVec.size() ; ++c) {
		add_fpredatorVector(fpredatorVec[c]);
	}

	fpredatorVec.clear();
	fpredatorVector.clear();

	for (size_t j(0); j < get_nombre_fourmis(); ++j) {
		bool gagner=false;
		
		fourmis[j]->chemin_fourmi(size_fourmiliere, posx, posy, posG.x, posG.y,
								  gagner, food_eaten);
		if(gagner){
			double miam=get_nb_food();
			miam=miam+50;
			set_nb_food(miam);
		}
	}
}

void clear_elem_F()
{
	elem_F.clear();
	nourritureVec.clear();
	nourritureVector.clear();
	fpredatorVec.clear();
	fpredatorVector.clear();
}

void Fourmiliere::naissance_fourmis() 
{	
	double p = min(1.0, get_nb_food()*birth_rate);
	
	bernoulli_distribution b(p);
	default_random_engine e;
	if(b(e)) {
		double nbT=get_nbC()+get_nbD()+get_nbP()+1;
		double nbC= (double) get_nbC();
		double nbD= (double) get_nbD();
		double ratioC = nbC/(nbT-1);
		double ratioD = nbD/(nbT-1);

		if(get_state())	{
			naissance_fourmis_free(get_idF(), ratioC, ratioD, pos.x, pos.y, taille);
		}
		else {
			naissance_fourmis_constrained(get_idF(), ratioC, ratioD, pos.x, pos.y, 
										  taille);
		}				
	}
}

void Fourmiliere::naissance_fourmis_free(size_t i, double ratioC, double ratioD, 
										 unsigned posx, unsigned posy, unsigned size) 
{
	if(ratioC < prop_free_collector) {
		if(not(squarecell::find_size3_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_collector(squarecell::find_size3_square(posx, posy,
			 													 size), i));
			unsigned nbC=get_nbC();
			++nbC;
			set_nbC(nbC);

		} else return;
	} else if(ratioD < prop_free_defensor) {
		if(not(squarecell::find_size3_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_defensor(squarecell::find_size3_square(posx, posy, 
																	   size), i));
			unsigned nbD=get_nbD();
			++nbD;
			set_nbD(nbD);
		} else return;
	} else {
		if(not(squarecell::find_size1_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_predator(squarecell::find_size1_square(posx, posy, 
																       size), i));
			unsigned nbP=get_nbP();
			++nbP;
			set_nbP(nbP);
		} else return;
	}
} 

void Fourmiliere::naissance_fourmis_constrained(size_t i,double ratioC, double ratioD, 
										 		unsigned posx, unsigned posy, 
												unsigned size) 
{
	if(ratioC < prop_constrained_collector) {
		if(not(squarecell::find_size3_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_collector(squarecell::find_size3_square(posx, 
															     posy, size),
							to_color(i%6)));
			unsigned nbC=get_nbC();
			++nbC;
			set_nbC(nbC);
		} else return;
	} else if(ratioD < prop_constrained_defensor) {
		if(not(squarecell::find_size3_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_defensor(squarecell::find_size3_square(posx, 
															    posy, size),
								to_color(i%6)));
			unsigned nbD=get_nbD();
			++nbD;
			set_nbD(nbD);
		} else return;
	} else {
		if(not(squarecell::find_size1_square(posx, posy, size) 
			== no_place_found)) {
			add_fourmi(nouvelle_predator(squarecell::find_size1_square(posx, 
															    posy, size),
					   to_color(i%6)));
			unsigned nbP=get_nbP();
			++nbP;
			set_nbP(nbP);
		} else return;
	}
} 

void Fourmiliere::check_age_fourmis()
{
	for(size_t j(0); j < get_nombre_fourmis(); ++j) {
		fourmis[j]->increment_age(); 
		
		if(fourmis[j]->get_age() == bug_life) {
			
			fourmis[j]->end_of_life_true();
			
		}
	}
}	
		
void Fourmiliere::erase_dead_fourmis(vector<Point>& new_food)
{
	size_t j(0);
	while(j < get_nombre_fourmis()) {
		if(fourmis[j]->get_end_of_life() == true) {
			Square dead_fourmi{1, get_fourmi_side(j), get_fourmi_posx(j), 
						  get_fourmi_posy(j)};
			switch (get_categorie(j))
			{
				case COLLECTOR : {
					if(fourmis[j]->get_fourmi_dispo() == true) {
						Point foody = {fourmis[j]->get_posx(),fourmis[j]->get_posy()};
						new_food.push_back(foody);
					}
					unsigned nbC=get_nbC();
					if(nbC>0) {
						nbC=nbC-1;
						set_nbC(nbC);
					} break;
				}
				case DEFENSOR : {
					unsigned nbD=get_nbD();
					if(nbD>0){
						nbD=nbD-1;
						set_nbD(nbD);
					} break;
				}
				case PREDATOR : {
					unsigned nbP=get_nbP();
					if(nbP>0) {
						nbP=nbP-1;
						set_nbP(nbP);
					} break;
				}
			}
			squarecell::delete_square(dead_fourmi);
			delete fourmis[j];
			fourmis.erase(fourmis.begin() + j);
		} else ++j;
	}	
}	
	
//retourne les cellules non-vides dans une premiere "couche" autour de la fourmi	
Neighbors Fourmiliere::get_not_empty_cells_1(unsigned j)
{	
	//cout << "im in GET NOT EMPTY CELLS 1 " << endl;
	return squarecell::detect_neighbors_1(fourmis[j]->get_posx(), 
										  fourmis[j]->get_posy(),
										  fourmis[j]->get_side());
}

//retourne les cellules non-vides dans une seconde "couche" autour de la fourmi
Neighbors Fourmiliere::get_not_empty_cells_2(unsigned j)
{	
	//cout << "im in GET NOT EMPTY CELLS 2 " << endl;
	return squarecell::detect_neighbors_2(fourmis[j]->get_posx(), 
										  fourmis[j]->get_posy(),
										  fourmis[j]->get_side());
}