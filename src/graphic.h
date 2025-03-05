
// Nom de fichier : graphic.h
// Auteurs : EL QABLI Rim (100%), FUMEAUX Max (0%), SCHWARTZ Maxime (0%)
// Scipers : 340997             , 326003          , 345352

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <gtkmm/drawingarea.h>


enum Couleur {ROUGE, VERT, BLEU, JAUNE, MAGENTA, CYAN,
              ROUGE_LIGHT, VERT_LIGHT, BLEU_LIGHT,
              JAUNE_LIGHT, MAGENTA_LIGHT, CYAN_LIGHT,
              BLANC, NOIR};

namespace graphic 
{
    void set_color(Couleur couleur);
    Couleur to_light(Couleur couleur);

    void draw_frame(double Min, double Max);
    void draw_vide(unsigned x, unsigned y, unsigned side, Couleur couleur);
    void draw_uniforme(unsigned x, unsigned y, unsigned side, Couleur couleur);
    void draw_losange(unsigned x, unsigned y, unsigned side, Couleur couleur);
    void draw_diagonale(unsigned x, unsigned y, unsigned side, Couleur couleur);
    void draw_grille(unsigned x, unsigned y, unsigned side, Couleur couleur);
    void draw_diagonale_losange(unsigned x, unsigned y, unsigned side, 
                                Couleur couleur); 
}

#endif 
