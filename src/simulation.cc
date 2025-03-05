// rendu1 public mais ne PAS diffuser

// Nom de fichier : simulation.cc
// Auteurs : EL QABLI Rim (33%), FUMEAUX Max (33%), SCHWARTZ Maxime (33%)
// Scipers : 340997            , 326003           , 345352

#include <cstdlib>
#include <string>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <memory>
#include "nourriture.h"
#include "fourmiliere.h"
#include "simulation.h"

using namespace std;

//Initialisation d'une unique instance de simulation
static Simulation simu; 

//Retourne l'adresse de cette instance de simulation
Simulation* Simulation::get_simulation_adresse() {
    return &simu;
}

enum Lecture{lectN, lectF, lectFIN};

//getters :
size_t Simulation::get_nombre_fourmilieres() const { return fourmilieres.size(); }
size_t Simulation::get_nombre_nourritures() const { return nourritures.size(); }
unsigned Simulation::get_idF_fourmiliere(unsigned i) const { 
	return fourmilieres[i].get_idF(); 
}
void Simulation::set_idF_fourmiliere(Fourmiliere& fourmiliere, unsigned idf) 
{
	fourmiliere.set_fourmiliere_idF(idf); 
}	
void Simulation::set_idF_fourmis(Fourmiliere fourmiliere, unsigned idf) 
{
	fourmiliere.set_idF_all_fourmis(idf);
}
void Simulation::get_size(int& taille) const { taille=fourmilieres.size(); }
unsigned Simulation::get_nbC_fourmiliere(int i) const 
{ 
	return fourmilieres[i].get_nbC(); 
}
unsigned Simulation::get_nbD_fourmiliere(int i) const 
{ 
	return fourmilieres[i].get_nbD(); 
}
unsigned Simulation::get_nbP_fourmiliere(int i) const 
{ 
	return fourmilieres[i].get_nbP(); 
}
double Simulation::get_nb_food_fourmiliere(int i) const 
{ 
	return fourmilieres[i].get_nb_food(); 
}

Simulation::Simulation() 
{
}

Simulation::~Simulation() 
{
}

void Simulation::add_new_food(Nourriture food) 
{
	nourritures.push_back(food);
}

void Simulation::add_fourmiliere(Fourmiliere fourmiliere) 
{
	unsigned idF = get_nombre_fourmilieres();
	set_idF_fourmiliere(fourmiliere, idF);
	set_idF_fourmis(fourmiliere, idF);

	fourmilieres.push_back(fourmiliere);
}

void Simulation::lecture(string nom_fichier, bool& error)
{
    string line;
    size_t nbN(0), nbF(0);
    Lecture etat(lectN);
    ifstream fichier(nom_fichier); 
	bool error_food(true);
	bool error_fourmiliere(true);
    if(!fichier.fail())  {
        while(getline(fichier >> ws,line)) {
			if(line[0]=='#')  continue;  
			istringstream data(line); 
			switch(etat)
			{
				case lectN:
					data >> nbN;
					for(size_t i(0) ; i < nbN ; ++i) {
						simu.add_new_food(lecture_nourriture(fichier,error_food));
						if(error_food==false) { error=false; return; }
						error=true;
						}
					etat = lectF;
					break;
				case lectF:
					data >> nbF;
					for(size_t i(0) ; i < nbF ; ++i)  {
						simu.add_fourmiliere(lecture_fourmiliere(fichier,
															     error_fourmiliere));
						if(error_fourmiliere==false){error=false; return;}
						error=true;
						}
					etat = lectFIN;
					break;
				case lectFIN:
					default:  cout << "Error decoding tests" << endl;
			}
		if(etat == lectFIN)  return; 
		}
	}
}

void Simulation::fonction_fourmiliere()
{
	Fourmiliere f;
	for(unsigned int i(0);i<simu.get_nombre_fourmilieres();i++)
	{
		unsigned taille=fourmilieres[i].get_size_fourmiliere();
		unsigned posx(fourmilieres[i].get_posx()),posy(fourmilieres[i].get_posy());
		bool freedom=fourmilieres[i].get_state();
		unsigned posGx(fourmilieres[i].get_posGx()),
						posGy(fourmilieres[i].get_posGy());
		unsigned nbC(fourmilieres[i].get_nbC()), nbD(fourmilieres[i].get_nbD()), 
				 nbP(fourmilieres[i].get_nbP()); 
		f.fonc_sizeF(i, taille, posx, posy, nbC, nbD, nbP, freedom);
		Square G_square({1, sizeG, posGx, posGy});
		Square F_square({0, taille, posx, posy});
		if(squarecell::full_overlapping(G_square, F_square,true)==false) {
			fourmilieres[i].set_end_of_klan(true);
		}
		for (unsigned int j(0);j<simu.get_nombre_fourmilieres();j++) 
		{
			if(j!=i){
				Square fourm({0, fourmilieres[j].get_size_fourmiliere(), 
							 fourmilieres[j].get_posx(), fourmilieres[j].get_posy()});
				if (squarecell::overlapping_squares_sizeF(F_square,fourm)) {
					
					if(taille>fourmilieres[i].get_size_fourmiliere()){
						taille=fourmilieres[i].get_size_fourmiliere();
						posx=fourmilieres[i].get_posx();
						posy=fourmilieres[i].get_posy();
					}
					freedom=false;
				} 
				else{
					freedom=true;
				}
			}
		}
		fourmilieres[i].set_size_fourmiliere(taille);
		fourmilieres[i].set_posx(posx);
		fourmilieres[i].set_posy(posy);
		fourmilieres[i].set_state(freedom);
	}
}

void Simulation::consommation_food()
{
	for(unsigned int i(0); i<simu.get_nombre_fourmilieres(); i++)	{
		double nbF=fourmilieres[i].get_nb_food();
		unsigned nbC=fourmilieres[i].get_nbC();
		unsigned nbD=fourmilieres[i].get_nbD();
		unsigned nbP=fourmilieres[i].get_nbP();
		int nbT=nbC+nbD+nbP+1;
		
		nbF=nbF-(nbT*food_rate);
        if(nbF<=0) {
            fourmilieres[i].set_end_of_klan(true);
        } else{
			fourmilieres[i].set_nb_food(nbF);
        }
	}
}

void Simulation::generator()
{
	for(unsigned int i(0); i<simu.get_nombre_fourmilieres(); i++){
		unsigned taille=fourmilieres[i].get_size_fourmiliere();
		unsigned posx=fourmilieres[i].get_posx();
		unsigned posy=fourmilieres[i].get_posy();
		unsigned posGx=fourmilieres[i].get_posGx();
		unsigned posGy=fourmilieres[i].get_posGy();
		int x=(int) posGx;
		int y=(int) posGy;
		unsigned centre_x=(posx)+taille/2;
		unsigned centre_y=(posy)+taille/2;
		
		int cx=(int) centre_x;
		int cy=(int) centre_y;
		
		if(abs(x-cx)>=abs(y-cy)) {
			generator_x( centre_x,  posGx,  posGy, i);
		}
		if(abs(x-cx)<abs(y-cy))	{
			generator_y( centre_y,  posGx,  posGy, i);
		}
	}
}

void Simulation::generator_x(unsigned centre_x, unsigned posGx, unsigned posGy, 
							 unsigned i)
{
	unsigned m(0);
	if(centre_x>posGx) {
		posGx=posGx+1;
		m=1;
		if(squarecell::move_square(posGx, posGy, m)==false) {
			posGx=posGx-1;
		} else {
			fourmilieres[i].set_posGx(posGx);
		}		
	
	if(centre_x < posGx) {
		posGx=posGx-1;
		m=3;
		if(squarecell::move_square(posGx,posGy,m)==false) {
			posGx=posGx+1;
		}
		else {
			fourmilieres[i].set_posGx(posGx);
		}		
	}
	}
}

void Simulation::generator_y(unsigned centre_y, unsigned posGx, unsigned posGy, 
							 unsigned i)
{
	unsigned m(0);
	if(centre_y>posGy) {
		posGy=posGy+1;
		m=2;
		if(squarecell::move_square(posGx,posGy,m)==false) {
			posGy=posGy-1;
		} else {
			fourmilieres[i].set_posGy(posGy);
		}		
	}
	if(centre_y<posGy) {
		posGy=posGy-1;
		m=4;
		if(squarecell::move_square(posGx,posGy,m)==false) {
			posGy=posGy+1;
		} else {
			fourmilieres[i].set_posGy(posGy);
		}		
	}
}
void Simulation::fonction_food()
{
	
	Nourriture food(0,0);
	Nourriture miam(new_food());

	cout<<"enters in food"<<endl;

	for(unsigned i(0); i<simu.get_nombre_fourmilieres(); i++)
	{
		unsigned taille=fourmilieres[i].get_size_fourmiliere();
		unsigned pos_fx=fourmilieres[i].get_posx(); 
		unsigned pos_fy=fourmilieres[i].get_posy();
		unsigned posx=miam.get_posx();
		unsigned posy=miam.get_posy();

		if(squarecell::food_overlapping(taille, pos_fx, pos_fy, posx, posy)) {
			miam=food;
		}
	}

	unsigned posx=miam.get_posx();
	unsigned posy=miam.get_posy();

	if(posx!=0 && posy!=0) {
		simu.add_new_food(miam);
		cout<<"new food"<<endl;
	}
}

void Simulation::death_row()
{
	for(unsigned int i(0);i<simu.get_nombre_fourmilieres();i++)
	{
		if(fourmilieres[i].get_end_of_klan())
		{
			fourmilieres.erase(fourmilieres.begin()+i);
		}
	}
}

Point Simulation::get_id(unsigned x, unsigned y)
{
	Point id_fourmiliere_fourmi(no_place_found);
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
			
			if( (x == fourmilieres[i].get_fourmi_posx(j)) 
				 and (y == fourmilieres[i].get_fourmi_posy(j)) ) {
				
				unsigned ui = (unsigned) i;
				unsigned uj = (unsigned) j;
				id_fourmiliere_fourmi = {ui, uj};
				return id_fourmiliere_fourmi;
			} 
		}
	}
	return id_fourmiliere_fourmi;
}

bool Simulation::check_if_predator(unsigned i, unsigned j) 
{
	if(fourmilieres[i].get_categorie(j) == PREDATOR) {
		return true;
	} else return false;
}

bool Simulation::check_if_defensor(unsigned i, unsigned j) 
{
	if(fourmilieres[i].get_categorie(j) == DEFENSOR) {
		return true;
	} else return false;
}

bool Simulation::is_predator(unsigned idF, unsigned idf) 
{
	if(check_if_predator(idF, idf)) {
		return true;
	} else return false;
}

bool Simulation::is_defensor(unsigned idF, unsigned idf) 
{
	if(check_if_defensor(idF, idf)) {
		return true;
	} else return false;
}

bool Simulation::same_coord_pred_pred(unsigned x, unsigned y, unsigned idF)
{
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
			if( (x == fourmilieres[i].get_fourmi_posx(j)) 
				and (y == fourmilieres[i].get_fourmi_posy(j)) 
				and (is_predator(i, j))
				and (i!=idF) ) {
					return true;
			} 
		}
	}
	return false;
}

Point Simulation::get_pred_pred_id(unsigned x, unsigned y, unsigned idF)
{
	Point id_fourmiliere_fourmi(no_place_found);

	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
			if( (x == fourmilieres[i].get_fourmi_posx(j)) 
				and (y == fourmilieres[i].get_fourmi_posy(j)) 
				and (is_predator(i, j))
				and (i!=idF) ) {
				unsigned ui = (unsigned) i;
				unsigned uj = (unsigned) j;
				id_fourmiliere_fourmi = {ui, uj};
				return id_fourmiliere_fourmi;
			} 
		}
	}
	return id_fourmiliere_fourmi;
}

bool Simulation::pred_within_coll(unsigned xP, unsigned yP, unsigned xC, unsigned yC)
{
	if( ((xP>=xC-1) and (xP<=xC+1)) and ((yP>=yC-1) and (yP<=yC+1)) ) {
		return true;
	} else return false;
}

bool Simulation::same_coord_pred_coll(unsigned x, unsigned y, unsigned idF)
{
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
			
			if( is_predator(i, j)
				and (i!=idF)
				and pred_within_coll(fourmilieres[i].get_fourmi_posx(j), 
									 fourmilieres[i].get_fourmi_posy(j), x, y) ) {
					
					cout << is_predator(i, j);
					return true;
			} 
		}
	}
	return false;
}

bool Simulation::neighbor_defensor(unsigned xD, unsigned yD, unsigned idF)
{
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
			
			if( (xD == fourmilieres[i].get_fourmi_posx(j)) 
				 and (yD == fourmilieres[i].get_fourmi_posy(j)) 
				 and (is_defensor(i, j))
				 and (i!=idF) ) {
					return true;
			} 
		}
	}
	return false;
}

void Simulation::contact_predator(unsigned i, unsigned j, 
								  Neighbors potential_predators) 
{
	//Id du no. Fourmiliere et no. fourmi du potentiel predator
	unsigned pred_idF, pred_idf;
	bool not_same_fourmiliere(false);

	for(auto not_empty_cell : potential_predators) {
		if(not (get_id(not_empty_cell.x, not_empty_cell.y) == no_place_found) ) {
			pred_idF = get_id(not_empty_cell.x, not_empty_cell.y).x;
			pred_idf = get_id(not_empty_cell.x, not_empty_cell.y).y;

			if(pred_idF != i) {
				not_same_fourmiliere = true;
			} else {
				not_same_fourmiliere = false;
			}

			if(is_predator(pred_idF, pred_idf) and not_same_fourmiliere) {					
				switch(fourmilieres[i].get_categorie(j))
				{
					case COLLECTOR:
						fourmilieres[i].end_of_life_true(j);
						break;
					case DEFENSOR:
						break;
					case PREDATOR:
						fourmilieres[i].end_of_life_true(j);
						fourmilieres[pred_idF].end_of_life_true(pred_idf);
						break;
				}
			}
		}
	}
}

void Simulation::superpo_predator(unsigned i, unsigned j)
{
	unsigned pred_idF, pred_idf;
	switch(fourmilieres[i].get_categorie(j))
	{
		case COLLECTOR:
			if(same_coord_pred_coll(fourmilieres[i].get_fourmi_posx(j), 
									fourmilieres[i].get_fourmi_posy(j), i)) {
				fourmilieres[i].end_of_life_true(j);
			}
			break;
		case DEFENSOR:
			break;
		case PREDATOR:
			if(same_coord_pred_pred(fourmilieres[i].get_fourmi_posx(j), 
									fourmilieres[i].get_fourmi_posy(j), i)) {
				if(not (get_pred_pred_id(fourmilieres[i].get_fourmi_posx(j), 
										 fourmilieres[i].get_fourmi_posy(j), i) 
						== no_place_found) ) {
					fourmilieres[i].end_of_life_true(j);
					pred_idF = get_pred_pred_id(fourmilieres[i].get_fourmi_posx(j), 
												fourmilieres[i].get_fourmi_posy(j), 
												i).x;
					pred_idf = get_pred_pred_id(fourmilieres[i].get_fourmi_posx(j), 
												fourmilieres[i].get_fourmi_posy(j), 
												i).y;
					fourmilieres[pred_idF].end_of_life_true(pred_idf);
				}
			}
			break;
	}	
}

void Simulation::check_touch_killers(unsigned i)  
{
	Neighbors potential_predators, potential_defensors;
	
	for(size_t j(0); j < fourmilieres[i].get_nombre_fourmis(); ++j) {
		
		if(fourmilieres[i].get_categorie(j) != DEFENSOR) {
			potential_predators = fourmilieres[i].get_not_empty_cells_1(j);
			potential_defensors = fourmilieres[i].get_not_empty_cells_2(j);

			if(not potential_predators.empty()) {
				if( (not potential_defensors.empty()) and 
					(fourmilieres[i].get_categorie(j) == COLLECTOR) ) {
					//check si collector au contact defensor d'autre fourmiliere
					for(auto not_empty_cell : potential_defensors) {
						if(neighbor_defensor(not_empty_cell.x, not_empty_cell.y, i)) {
							fourmilieres[i].end_of_life_true(j);
						}
					}
				}
				//check si collector/predator au contact defensor d'autre fourmiliere
				contact_predator(i, j, potential_predators);
			}
			//check si collector/predator est superpose avec predator
			superpo_predator(i, j);
		}
	}
}
		
//Algorithme de simulation :
void Simulation::simulation_algorithm() 
{
	fonction_food();
	generator();
	fonction_fourmiliere();
	consommation_food();

	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		fourmilieres[i].naissance_fourmis();
		fourmilieres[i].check_age_fourmis();
		check_touch_killers(i);
	}
	vector<Point> new_food;
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		fourmilieres[i].erase_dead_fourmis(new_food);
	}
	for(unsigned i(0);i<new_food.size();i++){
		Nourriture miam(new_food[i].x,new_food[i].y);
		simu.add_new_food(miam);
	}

	chemin_all_fourmilieres();

	new_food.clear();

	death_row();
}


void Simulation::draw_all_nourritures() 
{
	for(auto nourriture : nourritures) {
		nourriture.draw_nourriture(nourriture.get_posx(), nourriture.get_posy());
	}
}

void Simulation::draw_all_fourmilieres() 
{	
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		fourmilieres[i].draw_fourmiliere(fourmilieres[i].get_size_fourmiliere(),
										 fourmilieres[i].get_posx(), 
										 fourmilieres[i].get_posy());
	}
}

void Simulation::reset()
{
	nourritures.clear();
	fourmilieres.clear();
}

void Simulation::save_me(string filename)
{	
	ofstream file_saved(filename);
    fstream outfile;
	string infos("#"+filename+"\n\n");
    outfile.open(filename, ios_base::out);
    outfile.write(infos.data(), infos.size());
	unsigned taille=nourritures.size();
	string ajout2("# nb food\n" + to_string(taille) + "\n\n" + "# food\n");
    outfile.write(ajout2.data(), ajout2.size());
	for(auto nourriture : nourritures) {
		string ajout(to_string(nourriture.get_posx())+ " "
					 + to_string(nourriture.get_posy()) + "\n");
    	outfile.write(ajout.data(), ajout.size());
	}
	unsigned taille_f=simu.get_nombre_fourmilieres();
	string ajout("\n\n"+to_string(taille_f)+" # nb anthill\n");
    outfile.write(ajout.data(),ajout.size());
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		string ajout1("\t"+to_string(fourmilieres[i].get_posx())+" "
							+to_string(fourmilieres[i].get_posy())+" "
							+to_string(fourmilieres[i].get_size_fourmiliere())+" "
							+to_string(fourmilieres[i].get_posGx())+" "
							+to_string(fourmilieres[i].get_posGy())+" "
							+to_string(fourmilieres[i].get_nb_food())+" "
							+to_string(fourmilieres[i].get_nbC())+" "
							+to_string(fourmilieres[i].get_nbD())+" "
							+to_string(fourmilieres[i].get_nbP())+
							" # anthill #"+to_string(i+1)+"\n");
    	outfile.write(ajout1.data(), ajout1.size());
		string ajout5("\t# collectors:\n");
    	outfile.write(ajout5.data(), ajout5.size());
		save_collectors(outfile,i);
		string ajout6("\t# defensors:\n");
    	outfile.write(ajout6.data(), ajout6.size());
		save_defensors(outfile,i);
		string ajout7("\t# predators:\n");
    	outfile.write(ajout7.data(), ajout7.size());
		save_predators(outfile,i);
	}
	outfile.close();
}

void Simulation::save_collectors(fstream& outfile, unsigned i)
{
	if(fourmilieres[i].get_nbC()>0) {
		for(unsigned j(0); j<fourmilieres[i].get_nbC(); j++) {
			string ajout("\t" + to_string(fourmilieres[i].get_fourmi_posx(j)) 
			+ " " +to_string(fourmilieres[i].get_fourmi_posy(j))
			+ " " +to_string(fourmilieres[i].get_fourmi_age(j)) + "\n");
    		outfile.write(ajout.data(), ajout.size());
		}
		string ajout_l("\n");
    	outfile.write(ajout_l.data(), ajout_l.size());	
	}
}

void Simulation::save_defensors(fstream& outfile, unsigned i)
{
	if(fourmilieres[i].get_nbD()>0) {
		for(unsigned j(fourmilieres[i].get_nbC()); 
			j<fourmilieres[i].get_nbC()+fourmilieres[i].get_nbD(); j++) {
			string ajout("\t" + to_string(fourmilieres[i].get_fourmi_posx(j)) 
			+ " " +to_string(fourmilieres[i].get_fourmi_posy(j))
			+ " " +to_string(fourmilieres[i].get_fourmi_age(j)) + "\n");
    		outfile.write(ajout.data(), ajout.size());
		}
		string ajout_l("\n");
    	outfile.write(ajout_l.data(), ajout_l.size());
	}
}

void Simulation::save_predators(fstream& outfile, unsigned i)
{
	if(fourmilieres[i].get_nbP()>0) {
		unsigned total=fourmilieres[i].get_nbC()+fourmilieres[i].get_nbD()
						+fourmilieres[i].get_nbP();
		for(unsigned j(fourmilieres[i].get_nbC()+fourmilieres[i].get_nbD()); 
			j<total; j++) {
			string ajout("\t" + to_string(fourmilieres[i].get_fourmi_posx(j)) 
			+ " " +to_string(fourmilieres[i].get_fourmi_posy(j))
			+ " " +to_string(fourmilieres[i].get_fourmi_age(j)) + "\n");
    		outfile.write(ajout.data(), ajout.size());
		}
		string ajout_l("\n");
    	outfile.write(ajout_l.data(), ajout_l.size());
	}
}

void Simulation::chemin_all_fourmilieres()
{
	int a (get_nombre_nourritures());
	for (size_t z(0) ; z < get_nombre_nourritures() ; ++z) {
		Point nourr_pos = {nourritures[z].get_posx(), nourritures[z].get_posy()};
		add_nourriture(nourr_pos);
	}
	vector<Fourmi*> all_fourmis;
	for(size_t i(0); i < simu.get_nombre_fourmilieres(); ++i) {
		for(size_t e(0); e < simu.get_nombre_fourmilieres(); ++e) {
			if (i != e) {
				for (size_t f(0); f < fourmilieres[e].get_nombre_fourmis(); ++f) {
					all_fourmis.push_back(fourmilieres[e].get_fourmi(f));
				}
			}
		}
		for (size_t d(0); d < all_fourmis.size() ; ++d) {
			add_fpredator(all_fourmis[d]);
		}
		Point food_eaten({0,0});
		fourmilieres[i].chemin_fourmiliere(i, fourmilieres[i].get_size_fourmiliere(),
										fourmilieres[i].get_posx(),
										fourmilieres[i].get_posy(), a, food_eaten);
		if(food_eaten.x!=0 && food_eaten.y!=0){
			kill_food(food_eaten);
		}
		all_fourmis.clear();
	}
}

void Simulation::kill_food(Point food)
{
	for(unsigned i(0);i<get_nombre_nourritures();i++) {
		if(nourritures[i].get_posx()==food.x) {
			if( nourritures[i].get_posy()==food.y) {
				nourritures.erase(nourritures.begin()+i);
			}
		}
	}
}

void clear_all()
{
	clear_grid();
	clear_elem_F();
}