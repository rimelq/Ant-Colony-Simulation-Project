// rendu1 public mais ne PAS diffuser

// Nom de fichier : projet.cc
// Auteurs : EL QABLI Rim (33%), FUMEAUX Max (33%), SCHWARTZ Maxime (33%)
// Scipers : 340997            , 326003           , 345352

// contient le main et ouvre le fichier 
// BUT : transmettre le nom du fichier à simulation.cc

#include <iostream>
#include <gtkmm.h>
#include "simulation.h"
#include "gui.h"


int main(int argc, char * argv[])
{
	int const_argc(1);

	auto app = Gtk::Application::create(const_argc, argv, "org.gtkmm.example");
	
	Gui win;
	
	if (argc == 2) {
		bool error(true);
		std::cout <<"Avec fichier en entrée" << std::endl;
		win.set_simu(argv[1],error);
	} else if (argc == 1) {
		std::cout <<"Sans fichier en entrée" << std::endl;
	} else {
		std::cout<<"Trop de fichiers en entrée"<< std::endl;
	 	exit(0); 
	}
	
	win.set_title("Side Drawing");
	win.set_default_size(500,400);
	
	return app->run(win);
}
