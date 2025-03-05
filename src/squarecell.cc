// rendu1 public mais ne PAS diffuser

// Nom de fichier : squarecell.cc
// Auteurs : EL QABLI Rim (60%), FUMEAUX Max (20%), SCHWARTZ Maxime (20%)
// Scipers : 340997            , 326003           , 345352

#include <vector>
#include <cmath>
#include <iostream>
#include <random>
#include "error_squarecell.h"
#include "graphic.h"
#include "squarecell.h"
#include "constantes.h"

using namespace std;

void return_indices(Square square, unsigned& ci0, unsigned& ci_f, 
					unsigned& cj0, unsigned& cj_f,unsigned& nci0, 
					unsigned& nci_f, unsigned& ncj0, unsigned& ncj_f);

constexpr short unsigned g_dim(7) ;
constexpr short unsigned g_max(pow(2,g_dim)) ;

static vector<vector<bool>> grid(g_max,vector<bool>(g_max));
static default_random_engine e;


bool squarecell::valid_square(Square square){
	//cotes
	if (square.centered==true and (square.side%2)==0){
		cout<<"Le carre n'est pas possible"<<endl;
		return false;
	}
	if (square.centered==false and square.side==0){
		cout<<"Le carre n'est pas possible"<<endl;
		return false;
	}
	//origin:
	if (square.origin.x>g_max-1){
		cout << error_squarecell::print_index(square.origin.x, g_max-1);
		return false;
	}
	if (square.origin.y>g_max-1){
		cout << error_squarecell::print_index(square.origin.y, g_max-1);
		return false;
	}
	//centered outside
	if (square.centered==true and (square.origin.x+square.side/2>g_max-1 
							   or  square.origin.x-square.side/2>g_max-1)){
		cout << error_squarecell::print_outside(square.origin.x,square.side, g_max-1);
		return false;
	}
	if (square.centered==true and (square.origin.y+square.side/2>g_max-1
							   or  square.origin.y-square.side/2>g_max-1)){
		cout << error_squarecell::print_outside(square.origin.y,square.side, g_max-1);
		return false;
	}
	//non-centered outside
	if (square.centered==false and square.origin.x+square.side>g_max)
	{
		cout << error_squarecell::print_outside(square.origin.x,square.side, g_max-1);
		return false;
	}
	if (square.centered==false and square.origin.y+square.side>g_max){
		cout << error_squarecell::print_outside(square.origin.y,square.side, g_max-1);
		return false;
	}

	return true;
}

bool squarecell::valid_square_size(Square square){
	//cotes
	if (square.centered==true and (square.side%2)==0){
		return false;
	}
	if (square.centered==false and square.side==0){
		return false;
	}
	//origin:
	if (square.origin.x>g_max-1){
		return false;
	}
	if (square.origin.y>g_max-1){
		return false;
	}
	//centered outside
	if (square.centered==true and (square.origin.x+square.side/2>g_max-1 
							   or  square.origin.x-square.side/2>g_max-1)){
		return false;
	}
	if (square.centered==true and (square.origin.y+square.side/2>g_max-1
							   or  square.origin.y-square.side/2>g_max-1)){
		return false;
	}
	//non-centered outside
	if (square.centered==false and square.origin.x+square.side>g_max)
	{
		return false;
	}
	if (square.centered==false and square.origin.y+square.side>g_max){
		return false;
	}
	else{
		return true;
	}
}

bool squarecell::overlapping_squares(Square square1, Square square2, bool completely){
	double x1(square1.origin.x), y1(square1.origin.y);
	double x2(square2.origin.x), y2(square2.origin.y);

	if (square1.centered==false){
		x1 += double(square1.side)/2;
		y1 += double(square1.side)/2;
	} else {
		x1 += 0.5;
		y1 += 0.5;
	}

	if (square2.centered==false){
		x2 += double(square2.side)/2;
		y2 += double(square2.side)/2;
	} else {
		x2 += 0.5;
		y2 += 0.5;
	}
	
	double max_distance;

	if (completely==true) {
		max_distance = double(abs(int(square1.side) - int(square2.side))-2)/2;
	} else {
		max_distance = double(square1.side + square2.side)/2;
	}

	if (abs(x1-x2)<max_distance and abs(y1-y2)<max_distance) return true;

	return false;
}

bool squarecell::overlapping_squares_sizeF(Square square1, Square square2){
	unsigned x1(square1.origin.x), y1(square1.origin.y);
	unsigned x2(square2.origin.x), y2(square2.origin.y);
	unsigned taille1(square1.side);

	if(x1<=x2 && x2<=x1+taille1){
		if(y2>=y1 and y2<=y1+taille1)
		{
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

bool squarecell::move_square(unsigned x, unsigned y, unsigned i) 
{
	if(i==1){
		x=x+3;
		for(unsigned j(y-2);j<y+3;j++){
			if(grid[x][y] == true){return false;}
		}
		return true;
	}
	if(i==2){
		y=y+3;
		for(unsigned j(x-2);j<x+3;j++){
			if(grid[x][y] == true){return false;}
		}
		return true;
	}
	if(i==3){
		x=x-3;
		for(unsigned j(y-2);j<y+3;j++){
			if(grid[x][y] == true){return false;}
		}
		return true;
	}
	if(i==4){
		y=y-3;
		for(unsigned j(x-2);j<x+3;j++){
			if(grid[x][y] == true){return false;}
		}
		return true;
	}
	return false;
}

bool show_grid(unsigned x, unsigned y) 
{
	if(grid[x][y] == true) 
	{
		return true;
	}
	else 
	{ 
		return false;
	}
}

bool squarecell::full_overlapping(Square square1, Square square2, bool completely){
	double x1(square1.origin.x), y1(square1.origin.y);
	double x2(square2.origin.x), y2(square2.origin.y);

	if (square1.centered==false){
		x1 += double(square1.side)/2;
		y1 += double(square1.side)/2;
	} else {
		x1 += 0.5;
		y1 += 0.5;
	}

	if (square2.centered==false){
		x2 += double(square2.side)/2;
		y2 += double(square2.side)/2;
	} else {
		x2 += 0.5;
		y2 += 0.5;
	}
	double max_distance;
	if (completely==true) {
		max_distance = double(abs(int(square1.side) - int(square2.side))-2)/2;
	} else {
		max_distance = double(square1.side + square2.side)/2;
	}
	if (abs(x1-x2)<=max_distance and abs(y1-y2)<=max_distance) return true;
	return false;
}

bool squarecell::food_overlapping(unsigned taille, unsigned pos_fx, unsigned pos_fy, 
								  unsigned posx, unsigned posy)
{
	if(posx>=pos_fx && posx<=pos_fx+taille) {
		if(posy>=pos_fy && posy<=pos_fy+taille)	{
			return true;
		}
	}
	return false;
}

void squarecell::declare_square(Square square){
	unsigned ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f;
	return_indices(square, ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f);
	if (square.centered){
		for (unsigned i(ci0);i<ci_f;++i){
			for (unsigned j(cj0);j<=cj_f;++j ){
				grid[i][j]=true;
			}
		}
	} else {
		for (unsigned i(nci0); i<nci_f; ++i ){
			for (unsigned j(ncj0); j<ncj_f; ++j ){
				grid[i][j] = true;
			}
		}
	}
	
}

void squarecell::delete_square(Square square){
	unsigned ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f;
	return_indices(square, ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f);
	if (square.centered){
		for (unsigned i(ci0); i<ci_f; ++i){
			for (unsigned j(cj0); j<=cj_f; ++j){
				grid[i][j]=false;
			}
		}
	} else {
		for (unsigned i(nci0); i<nci_f; ++i){
			for (unsigned j(ncj0);j<ncj_f;++j){
				grid[i][j]=false;
			}
		}
	}
	
}

bool squarecell::overlapping_squares(Square square, unsigned& x, unsigned& y){
	unsigned ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f;
	return_indices(square, ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f);
	if (square.centered==true){
		for (unsigned i(ci0); i<ci_f; ++i){
			for (unsigned j(cj0); j<=cj_f; ++j){
				if (grid[i][j]==true){
					x = j;
					y = g_max-1-i;
					return true;
				}
			}
		}
	} else {
		for (unsigned i(nci0); i<nci_f; ++i){
			for (unsigned j(ncj0); j<ncj_f; ++j){
				if (grid[i][j]==true){
					x = j;
					y = g_max-1-i;
					return true;
				}
			}
		}
	}
	return false;
}

bool squarecell::overlapping_squares(Square square){
	unsigned x, y;
	return squarecell::overlapping_squares(square, x,y );
}

void return_indices(Square square, unsigned& ci0, unsigned& ci_f, 
					unsigned& cj0, unsigned& cj_f, unsigned& nci0, 
					unsigned& nci_f, unsigned& ncj0, unsigned& ncj_f) {
	ci0 = g_max-1-square.origin.y-square.side/2;
	ci_f = g_max-square.origin.y+square.side/2;
	cj0 = square.origin.x-square.side/2;
	cj_f = square.origin.x+square.side/2;
	nci0 = g_max-square.origin.y-square.side;
	nci_f = g_max-square.origin.y;
	ncj0 = square.origin.x;
	ncj_f = square.origin.x+square.side;
}

void clear_grid()
{
	for(int i(0); i<g_max; i++) {
		for(int j(0); j<g_max; j++) {
			grid[i][j]=false;
		}
	}
}

Couleur squarecell::to_color(unsigned i_modulo_6) {
    switch(i_modulo_6)
    {
        case 0: 
            return ROUGE; break;
        case 1: 
            return VERT; break;
        case 2: 
            return BLEU; break;
        case 3: 
            return JAUNE; break;
        case 4:
            return MAGENTA; break;
        case 5: 
            return CYAN; break; 
		default: 
			return  NOIR; break;
    }
}

void squarecell::draw_square(Square square, Style style_carre, Couleur couleur) {
	switch(style_carre)
    {
        case VIDE:
			graphic::draw_vide(square.origin.x, square.origin.y, square.side,
							   couleur);
            break;
        case UNIFORME: 
            graphic::draw_uniforme(square.origin.x, square.origin.y, square.side,
								   couleur);
            break;
        case LOSANGE: 
       		graphic::draw_losange(square.origin.x, square.origin.y, square.side,
			   					  couleur);
            break;
		case DIAGONALE: 
			graphic::draw_diagonale(square.origin.x, square.origin.y, square.side,
									couleur);
            break;
        case GRILLE: 
            graphic::draw_grille(square.origin.x, square.origin.y, square.side,
								 couleur);
            break;
		case DIAGONALE_LOSANGE: 
			graphic::draw_diagonale_losange(square.origin.x, square.origin.y, 
											square.side, couleur);
            break;
    }
}

//surcharge externe de l'operateur == pour des structures de type Point
bool operator==(Point pos1, Point pos2) 
{
	return (pos1.x == pos2.x) && (pos1.y == pos2.y);
}

bool squarecell::bool_creation_random(double proba) 
{
	bernoulli_distribution b(proba);
	return b(e);
}

//check si une cellule est vide
bool squarecell::detect_empty_cell(unsigned x, unsigned y) 
{
	Square square {true, sizeC, x, y};
	unsigned ci0(0), ci_f(0), cj0(0), cj_f(0), nci0(0), nci_f(0), ncj0(0), ncj_f(0);
	return_indices(square, ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f);

	return not grid[ci0+1][cj0+1];
}

//trouve l'emplacement pour un espace vide d'un collector/defensor dans sa fourmiliere
Point squarecell::find_size3_square(unsigned xF, unsigned yF, unsigned sideF)  
{
	Point empty_place;
	for(unsigned y(yF+sideF-2); y > yF+2; --y) {
		for(unsigned x(xF+1); x < xF+sideF-3; ++x) {
			bool square_is_empty = true;
			for(unsigned i(0); i <= 2; ++i) {
				for(unsigned j(0); j <= 2; ++j) {
					square_is_empty = square_is_empty && detect_empty_cell(x+i, y-j);
				}
			}
			if (square_is_empty) {
				empty_place = {x+1, y-1};
				return empty_place;
			}
		}
	}
	return no_place_found;
}

//trouver un emplacement vide pour la creation d'un predator dans sa fourmiliere
Point squarecell::find_size1_square(unsigned xF, unsigned yF, unsigned sideF) 
{
	Point empty_place;
	for(unsigned y(yF+sideF-2); y > yF+1; --y) {
		for(unsigned x(xF+1); x < xF+sideF-1; ++x) {
			if(detect_empty_cell(x, y)) {
				empty_place = {x, y};
				return empty_place;
			}
		}
	}
	Point no_place_found = {200, 200};
	return no_place_found;
}

//check si une cellule est remplie
bool squarecell::detect_full_cell(unsigned posx, unsigned posy)
{
	Square square {true, sizeC, posx, posy};
	unsigned ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f;
	return_indices(square, ci0, ci_f, cj0, cj_f, nci0, nci_f, ncj0, ncj_f);

	return grid[ci0+1][cj0+1];
}

//detecte une cellule non vide dans une premiere "couche" autour du carre
Neighbors squarecell::detect_neighbors_1(unsigned posx, unsigned posy, unsigned side) 
{
	Neighbors not_empty_cells;
	Point not_empty;
	
	for(unsigned x(posx-(side/2)-1); x<=posx+(side/2)+1; ++x) {
		if(squarecell::detect_full_cell(x, posy-(side/2)-1)) {
			not_empty = {x,posy-(side/2)-1};
			not_empty_cells.push_back(not_empty);
		}
		if(squarecell::detect_full_cell(x, posy+(side/2)+1)) {
			not_empty = {x,posy+(side/2)+1};
			not_empty_cells.push_back(not_empty);
		}
	}

	for(unsigned y(posy-(side/2)); y<=posy+(side/2); ++y) {
		if(squarecell::detect_full_cell(posx-(side/2)-1, y)) {
			not_empty = {posx-(side/2)-1,y};
			not_empty_cells.push_back(not_empty);
		}
		if(squarecell::detect_full_cell(posx+(side/2)+1, y)) {
			not_empty = {posx+(side/2)+1,y};
			not_empty_cells.push_back(not_empty);
		}
	}
	return not_empty_cells;
}

//detecte une cellule non vide dans une seconde "couche" autour du carre
Neighbors squarecell::detect_neighbors_2(unsigned posx, unsigned posy, unsigned side) 
{
	Neighbors not_empty_cells;
	Point not_empty;
	
	for(unsigned x(posx-(side/2)-2); x<=posx+(side/2)+2; ++x) {
		if(posy > 2) {
			if(squarecell::detect_full_cell(x, posy-(side/2)-2)) {
				not_empty = {x,posy-(side/2)-2};
				not_empty_cells.push_back(not_empty);
			}
		}
		if(posy < 125) {
			if(squarecell::detect_full_cell(x, posy+(side/2)+2)) {
				not_empty = {x,posy+(side/2)+2};
				not_empty_cells.push_back(not_empty);
			}
		}
	}

	for(unsigned y(posy-(side/2)-1); y<=posy+(side/2)+1; ++y) {
		if(posx > 2) {
			if(squarecell::detect_full_cell(posx-(side/2)-2, y)) {
				not_empty = {posx-(side/2)-2,y};
				not_empty_cells.push_back(not_empty);
			}
		}
		if(posx < 125) {
			if(squarecell::detect_full_cell(posx+(side/2)+2, y)) {
				not_empty = {posx+(side/2)+2,y};
				not_empty_cells.push_back(not_empty);
			}
		}
	}
	return not_empty_cells;
}

