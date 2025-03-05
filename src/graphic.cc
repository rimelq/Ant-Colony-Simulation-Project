
// Nom de fichier : graphic.cc
// Auteurs : EL QABLI Rim (100%), FUMEAUX Max (0%), SCHWARTZ Maxime (0%)
// Scipers : 340997             , 326003          , 345352


#include "graphic_gui.h"
#include <iostream>
#include <cairomm/context.h>

#define ROUGE_RGB 1.0, 0.0, 0.0
#define ROUGE_LIGHT_RGB 1.0, 0.55, 0.55
#define VERT_RGB 0.0, 1.0, 0.0
#define VERT_LIGHT_RGB 0.75, 1.0, 0.75
#define BLEU_RGB 0.0, 0.0, 1.0
#define BLEU_LIGHT_RGB 0.55, 0.55, 1.0
#define JAUNE_RGB 0.96, 0.96, 0.02
#define JAUNE_LIGHT_RGB 0.96, 0.98, 0.82
#define MAGENTA_RGB 1.0, 0.0, 1.0
#define MAGENTA_LIGHT_RGB 1.0, 0.55, 1.0
#define CYAN_RGB 0.0, 0.98, 1.0
#define CYAN_LIGHT_RGB 0.68, 1.0, 1.0
#define BLANC_RGB 1.0, 1.0, 1.0
#define NOIR_RGB 0.0, 0.0, 0.0
#define GRIS_RGB 0.5, 0.5, 0.5

using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_gui::set_context(const Cairo::RefPtr<Cairo::Context>& cr) 
{
    ptcr = &cr;
}

void graphic::set_color(Couleur couleur) 
{
    switch(couleur)
    {
        case ROUGE: 
            (*ptcr)->set_source_rgb(ROUGE_RGB); break;
        case VERT: 
            (*ptcr)->set_source_rgb(VERT_RGB); break;
        case BLEU: 
            (*ptcr)->set_source_rgb(BLEU_RGB); break;
        case JAUNE: 
            (*ptcr)->set_source_rgb(JAUNE_RGB); break;
        case MAGENTA: 
            (*ptcr)->set_source_rgb(MAGENTA_RGB); break;
        case CYAN: 
            (*ptcr)->set_source_rgb(CYAN_RGB); break;
        case ROUGE_LIGHT: 
            (*ptcr)->set_source_rgb(ROUGE_LIGHT_RGB); break;
        case VERT_LIGHT: 
            (*ptcr)->set_source_rgb(VERT_LIGHT_RGB); break;
        case BLEU_LIGHT: 
            (*ptcr)->set_source_rgb(BLEU_LIGHT_RGB); break;
        case JAUNE_LIGHT: 
            (*ptcr)->set_source_rgb(JAUNE_LIGHT_RGB); break;
        case MAGENTA_LIGHT: 
            (*ptcr)->set_source_rgb(MAGENTA_LIGHT_RGB); break;
        case CYAN_LIGHT: 
            (*ptcr)->set_source_rgb(CYAN_LIGHT_RGB); break;
        case BLANC: 
            (*ptcr)->set_source_rgb(BLANC_RGB); break; 
        default: break;  
    }
}

Couleur graphic::to_light(Couleur couleur) 
{
    switch(couleur)
    {
        case ROUGE: 
            return ROUGE_LIGHT; break;
        case VERT: 
            return VERT_LIGHT; break;
        case BLEU: 
            return BLEU_LIGHT; break;
        case JAUNE: 
            return JAUNE_LIGHT; break;
        case MAGENTA:
            return MAGENTA_LIGHT; break;
        case CYAN: 
            return CYAN_LIGHT; break;  
        default: return NOIR; break;
    }
}

void graphic::draw_frame(double Min, double Max)
{   
    (*ptcr)->set_source_rgb(NOIR_RGB);
	(*ptcr)->paint();

    (*ptcr)->set_line_width(1);
    (*ptcr)->set_source_rgb(BLANC_RGB);

    (*ptcr)->set_line_width(1);
    (*ptcr)->set_source_rgb(BLANC_RGB);
    (*ptcr)->move_to(0.5, 0.5);
    (*ptcr)->line_to(127.5, 0.5);
    (*ptcr)->line_to(127.5, 127.5);
    (*ptcr)->line_to(0.5, 127.5);
    (*ptcr)->line_to(0.5, 0.0);

    (*ptcr)->stroke();

    for(double i(Min); i<=Max; i+=(128)/Max) {
        (*ptcr)->set_line_width(0.1);
        (*ptcr)->set_source_rgb(GRIS_RGB);
        (*ptcr)->move_to(i, Min);
        (*ptcr)->line_to(i, Max);

        (*ptcr)->stroke();
    }

    for(double i(Min); i<=Max; i+=(128)/(Max)) {
        (*ptcr)->set_line_width(0.1);
        (*ptcr)->set_source_rgb(GRIS_RGB);
        (*ptcr)->move_to(Min, i);
        (*ptcr)->line_to(Max, i);

        (*ptcr)->stroke();
    }
}

void graphic::draw_vide(unsigned x, unsigned y, unsigned side, Couleur couleur) 
{
    set_color(couleur);

    double xD=(double) x; //conversion des unsigned en double
    double yD=(double) y;

    (*ptcr)->set_line_width(0.3); //epaisseur du contour de la fourmiliere
    (*ptcr)->rectangle(xD+0.5, yD+0.5, side-1, side-1); 
   
    (*ptcr)->close_path(); 
    (*ptcr)->stroke();
}

void graphic::draw_uniforme(unsigned x, unsigned y, unsigned side, Couleur couleur) 
{
    set_color(couleur);
    
    (*ptcr)->set_line_width(0.1); //petite epaisseur pour precision optimale
    (*ptcr)->rectangle(x-side/2, y-side/2, side, side);

    (*ptcr)->close_path(); 
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
}

void graphic::draw_losange(unsigned x, unsigned y, unsigned side, Couleur couleur) 
{   
    set_color(couleur);

    double xD=(double) x; //conversion des unsigned en double
    double yD=(double) y;
    double sideD=(double) side;
    xD=x+0.5;
    yD=y+0.5;
    
    (*ptcr)->set_line_width(0.1);  //petite epaisseur pour precision optimale
    (*ptcr)->move_to(xD-sideD/2, yD); 
    (*ptcr)->line_to(xD, yD-sideD/2); 
    (*ptcr)->line_to(xD+sideD/2, yD);
    (*ptcr)->line_to(xD, yD+sideD/2);
    (*ptcr)->line_to(xD-sideD/2, yD);
   
    (*ptcr)->close_path(); 
    (*ptcr)->fill_preserve();
    (*ptcr)->stroke();
    
}

void graphic::draw_diagonale(unsigned x, unsigned y, unsigned side, Couleur couleur) 
{
//cellules de couleur foncee
    graphic::draw_uniforme(x, y, 1, couleur);
    graphic::draw_uniforme(x+1, y+1, 1, couleur);
    graphic::draw_uniforme(x-1, y+1, 1, couleur);
    graphic::draw_uniforme(x-1, y-1, 1, couleur);
    graphic::draw_uniforme(x+1, y-1, 1, couleur);

//cellules de couleur light
    graphic::draw_uniforme(x, y+1, 1, to_light(couleur));  
    graphic::draw_uniforme(x-1, y, 1, to_light(couleur));
    graphic::draw_uniforme(x, y-1, 1, to_light(couleur));
    graphic::draw_uniforme(x+1, y, 1, to_light(couleur));
    
}

void graphic::draw_grille(unsigned x, unsigned y, unsigned side, Couleur couleur) 
{
//cellules de couleur foncee
    graphic::draw_uniforme(x, y, 1, couleur);  
    graphic::draw_uniforme(x, y+1, 1, couleur);
    graphic::draw_uniforme(x-1, y, 1, couleur);
    graphic::draw_uniforme(x, y-1, 1, couleur);
    graphic::draw_uniforme(x+1, y, 1, couleur);

// //cellules de couleur light
    graphic::draw_uniforme(x+1, y+1, 1, to_light(couleur));    
    graphic::draw_uniforme(x-1, y+1, 1, to_light(couleur));
    graphic::draw_uniforme(x-1, y-1, 1, to_light(couleur));
    graphic::draw_uniforme(x+1, y-1, 1, to_light(couleur));

}

void graphic::draw_diagonale_losange(unsigned x, unsigned y, unsigned side, 
                                     Couleur couleur) 
{
    graphic::draw_diagonale(x, y, side, couleur);
    graphic::draw_losange(x, y, side/3, BLANC);
}