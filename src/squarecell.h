
// Nom de fichier : squarecell.h
// Auteurs : EL QABLI Rim (60%), FUMEAUX Max (20%), SCHWARTZ Maxime (20%)
// Scipers : 340997            , 326003           , 345352

#ifndef SQUARECELL_H
#define SQUARECELL_H

#include "error_squarecell.h"
#include "graphic_gui.h"

enum Style {VIDE, UNIFORME, LOSANGE, DIAGONALE, GRILLE, DIAGONALE_LOSANGE};

struct Point 
{
	unsigned x, y;
};

struct Square 
{
	bool centered;
	unsigned side;
	Point origin;
};

typedef std::vector<Point> Neighbors;
constexpr Point no_place_found({200, 200});

namespace squarecell 
{ 
	bool valid_square(Square square);
	bool valid_square_size(Square square);
	bool overlapping_squares(Square square1, Square square2, bool completely);
	bool full_overlapping(Square square1, Square square2, bool completely);
	void declare_square(Square square);
	void delete_square(Square square);
	bool overlapping_squares(Square square, unsigned& x, unsigned& y);
	bool overlapping_squares(Square square);
	bool overlapping_squares_sizeF(Square square1, Square square2);
	bool food_overlapping(unsigned taille, unsigned pos_fx, unsigned pos_fy, 
						  unsigned posx, unsigned posy);
	bool move_square(unsigned x, unsigned y, unsigned i);

	Couleur to_color(unsigned i_modulo_6);
	void draw_square(Square square, Style style_carre, Couleur couleur);

	bool bool_creation_random(double proba);
	bool detect_empty_cell(unsigned x, unsigned y);
	Point find_size3_square(unsigned xF, unsigned yF, unsigned sideF);
	Point find_size1_square(unsigned xF, unsigned yF, unsigned sideF);
	
	bool detect_full_cell(unsigned posx, unsigned posy);
	Neighbors detect_neighbors_1(unsigned posx, unsigned posy, unsigned side);
	Neighbors detect_neighbors_2(unsigned posx, unsigned posy, unsigned side);
}

void return_indices(Square square, unsigned& ci0, unsigned& ci_f, 
					unsigned& cj0, unsigned& cj_f,unsigned& nci0, 
					unsigned& nci_f, unsigned& ncj0, unsigned& ncj_f);
bool operator==(Point pos1, Point pos2);


void clear_grid();
bool show_grid(unsigned x, unsigned y);


#endif
