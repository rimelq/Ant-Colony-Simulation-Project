
// Nom de fichier : gui.cc
// Auteurs : EL QABLI Rim (10%), FUMEAUX Max (55%), SCHWARTZ Maxime (35%)
// Scipers : 340997            , 326003           , 345352

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <iomanip>
#include "graphic_gui.h"
#include "simulation.h"
#include "gui.h"

using namespace std;

namespace 
{
	//Initialisation d'un pointeur sur l'unique instance de simulation
	Simulation* simu=Simulation::get_simulation_adresse();

}



static int i(0); //indice de la fourmiliere utilisé pour previous et next
static bool save_error(false); //passe à false en cas d'erreur dans le squarecell
static unsigned int val(1);
static unsigned nbC;
static unsigned nbD;
static unsigned nbP;
static double nbF;

//================================================================================

Gui::Gui():
	m_Box(Gtk::ORIENTATION_HORIZONTAL,0),
	m_Box_Left(Gtk::ORIENTATION_VERTICAL, 0),
	m_Box_Right(Gtk::ORIENTATION_VERTICAL, 0),
	m_Label_General("General",0,0),
	m_Label_Info("Info",0,0),
	m_Label_Anthillinfo("none selected",0,0),
	m_Label_Informations("Anthill info",false,false),
	m_Label_nbfood("Nb food: "),
	m_Button_Exit("Exit"),
	m_Button_Open("Open"),
	m_Button_Save("Save"),
	m_Button_Start("Start"),
	m_Button_Step("Step"),
	m_Button_Previous("Previous"),
	m_Button_Next("Next"),
	timer_added(false),
	disconnect(false),
	timeout_value(100) 

{
	// Set title and border of the window
	set_title("Anthill simulation");
	set_border_width(0);

	add(m_Box);
	
	m_Box.pack_start(m_Box_Left, 0, 0);
	m_Box.pack_start(m_Box_Right);
	m_Box_Left.pack_start(m_Label_General,0,0);
	m_Box_Left.pack_start(m_Button_Exit,false,false);
	m_Box_Left.pack_start(m_Button_Open,false,false); 
	m_Box_Left.pack_start(m_Button_Save,false,false);
	m_Box_Left.pack_start(m_Button_Start,false,false);
	m_Box_Left.pack_start(m_Button_Step,false,false);
	m_Box_Left.pack_start(m_Label_Info,0,0);
	m_Box_Left.pack_start(m_Label_nbfood,0,0);
	m_Box_Left.pack_start(m_Label_Informations,0,0);
	m_Box_Left.pack_start(m_Button_Previous,false,false);
	m_Box_Left.pack_start(m_Button_Next,false,false);
	m_Box_Left.pack_start(m_Label_Anthillinfo,0,0);

	m_Area.set_size_request(500,500);
	m_Box_Right.pack_start(m_Area);

	m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Exit) );

	m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Open) );

	m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Save) );

	m_Button_Start.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Start) );

	m_Button_Step.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Step) );

	m_Button_Previous.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Previous) );

	m_Button_Next.signal_clicked().connect(sigc::mem_fun(*this,
				&Gui::on_button_clicked_Next) );

	show_all_children();
}

Gui::~Gui()
{
}

void Gui::on_button_clicked_Exit()
{
	exit(0);
	m_Area.clear();
}

void Gui::refresh()
{
	auto win = get_window();
	if(win) {
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						      get_allocation().get_height());		
		win->invalidate_rect(r,false);
	}
}

//Starts the simulation
void Gui::set_simu(string file, bool& error)
{
	m_Label_Anthillinfo.set_text("none selected"); 
	i=0;
	val=1;

	clear_all();
	simu->reset();
	m_Area.clear();
	save_error=true;
	simu->lecture(file, error);

	if(error) {
		m_Area.draw();
		cout<<"You CAN display this folder\n"<<endl;
		m_Label_nbfood.set_text("Nb food : " + 
						to_string(simu->Simulation::get_nombre_nourritures()));
		save_error=true;
	} else {
		cout<<"You CANNOT display this folder, there is an error\n"<<endl;
		save_error=false;
		m_Label_nbfood.set_text("Nb food : ");
	}
}

string Gui::access_file()
{
	Gtk::FileChooserDialog dialog("Choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();
	
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::cout << "Open clicked." << std::endl;
			std::string filename = dialog.get_filename();
			std::cout << "File selected: " <<  filename << std::endl;
			return filename;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
	return "\0";
}

void Gui::on_button_clicked_Open() 
{
	string file(access_file());
	bool error(true);

	if (file!="\0") {
		set_simu(file,error);
	}
}

void Gui::on_button_clicked_Save()
{
	if(!save_error) {
		cout<<"You CANNOT save this folder, there is an error or there is no folder\n"<<endl;
		clear_all();
		simu->reset();
		m_Area.clear();
		return;
	}

	static int save_nb(0);
    ++save_nb;
    string saved_filename("saved_file_"+to_string(save_nb)+".txt");
	simu->save_me(saved_filename);
}

void Gui::on_button_clicked_Start()
{
	m_Button_Start.set_label("Stop");

	if(not timer_added)	{
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &Gui::on_timeout), 
										timeout_value );
		timer_added = true;
		cout << "Timer has been started" <<endl;
	} else {
		Gui::stop_timer();
	}
}

bool Gui::on_timeout()
{
    MyArea ma;
    if(disconnect) {
        disconnect = false; 
        return false;
    }
    cout << "Timer update : " << val <<endl;
   
	simu->simulation_algorithm();

	m_Label_nbfood.set_text("Nb food : " + 
						to_string(simu->Simulation::get_nombre_nourritures()));

			
	unsigned nbT=simu->get_nombre_fourmilieres();
	if(nbT==0)
	{
		nbC=0;
		nbD=0;
		nbP=0;
		nbF=0;
	}
	else{
		nbC=simu->get_nbC_fourmiliere(i);
		nbD=simu->get_nbD_fourmiliere(i);
		nbP=simu->get_nbP_fourmiliere(i);
		nbF=simu->get_nb_food_fourmiliere(i);

	}
	m_Label_Anthillinfo.set_text("id: " + to_string(i) + "\nTotal food: " 
								 + to_string(nbF) + "\n\nnbC: " 
								 + to_string(nbC) + "\nnbD: " 
								 + to_string(nbD) + "\nnbP: " + to_string(nbP));
	
	refresh();
	++val;
    return true;
}

void Gui::stop_timer()
{
    cout << "Timer is stopped" << std::endl;
    disconnect = true;
    timer_added = false;
    m_Button_Start.set_label("Start");
}

void Gui::on_button_clicked_Step()
{
	if(not timer_added) {
		Gui::on_timeout();
	} else {
		cout<<"Timer is already running"<<endl;
	}
}

void Gui::on_button_clicked_Previous()
{	
	i--;
	
	int taille(0);
	
	simu->get_size(taille);
	
	if(i<0) {
		i=taille;
	}
	if(i==0) {
		m_Label_Anthillinfo.set_text("none selected");
		i=taille+1;
		return;
	}
}

void Gui::on_button_clicked_Next()
{	
	int taille(0);
	
	simu->get_size(taille);
	i++;

	if(i>taille+1) {
		i=1;
	}
	if(i==taille+1) {
		m_Label_Anthillinfo.set_text("none selected");
		i=0;
		return;
	}
}

bool Gui::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 's':
				on_button_clicked_Start();
				break;
			case '1':
				on_button_clicked_Step();
				break;
			case 'n':
				on_button_clicked_Next();
				break;
			case 'p' :
				on_button_clicked_Previous();
				break;
			case 'q':
				on_button_clicked_Exit();
				break;
		}
	}
	return Gtk::Window::on_key_press_event(key_event);
}

//=====================================================================================================

// default Model Framing and window parameters
static Frame default_frame = {0., 128.0, 0., 128.0, 1, 300, 200}; 
static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame);

MyArea::MyArea(): empty(false)
{
}

MyArea::~MyArea()
{
}

void MyArea::clear()
{
	empty = true; 
	refresh();
}

void MyArea::draw()
{
	empty = false;
	refresh();
}

void MyArea::refresh()
{
	auto win = get_window();
	if(win) {
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						      get_allocation().get_height());		
		win->invalidate_rect(r,false);
	}
}

void MyArea::adjustFrame()
{	
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	
	frame.width = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp) { 
	    frame.yMax = default_frame.yMax ;
	    frame.yMin = default_frame.yMin ;	
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
        
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
    } else { 
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
	  	  
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);

	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    }
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame)
{	
	cr->translate(frame.width/2, frame.height/2); 
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
			  -frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>&cr)
{
	adjustFrame();
	orthographic_projection(cr,frame);
	graphic_gui::set_context(cr);

	double Min(0);
    double Max(128);

    if(frame.xMax>frame.yMax) {
        Min=frame.yMin;
        Max=frame.yMax;
    }
    if(frame.xMax<frame.yMax) {
        Min=frame.xMin;
        Max=frame.xMax;
    }
	
	//Drawing frame and entities
	graphic::draw_frame(Min,Max);

	if(not empty) {
		simu->draw_all_nourritures();
		simu->draw_all_fourmilieres();
	} else { 
		simu->reset();
	}

	return true;
}