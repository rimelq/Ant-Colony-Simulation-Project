
// Nom de fichier : graphic_gui.h
// Auteurs : EL QABLI Rim (100%), FUMEAUX Max (0%), SCHWARTZ Maxime (0%)
// Scipers : 340997             , 326003          , 345352

#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H

#include <gtkmm/drawingarea.h>
#include "graphic.h"

namespace graphic_gui
{
    void set_context(const Cairo::RefPtr<Cairo::Context>& cr);
}

#endif