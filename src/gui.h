
// Nom de fichier : gui.h
// Auteurs : EL QABLI Rim (10%), FUMEAUX Max (55%), SCHWARTZ Maxime (35%)
// Scipers : 340997            , 326003           , 345352

#ifndef GTKMM_EXAMPLE_GUI_H
#define GTKMM_EXAMPLE_GUI_H

#include <gtkmm.h>
#include <iostream>
#include <string>
#include "simulation.h"

struct Frame // Model Framing and window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio
	int height;  // window height
	int width;   // window width
};

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();
	void clear();			
	void draw();			
	void setFrame(Frame x); 
	void adjustFrame();
	//void refresh();

protected:
	//Override default signal handler:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);

private:
	Frame frame;
	bool empty;
	void refresh();
};

//==================================================================================
// includes keyboard and mouse events
class Gui : public Gtk::Window
{
public:
	Gui();
	virtual ~Gui();
	std::string access_file();
	void set_simu(std::string fileName, bool& error);
	bool on_timeout();
	void refresh();
protected:
	//Button Signal handlers:
	void on_button_clicked_Exit();
	void on_button_clicked_Open();
	void on_button_clicked_Save();
	void on_button_clicked_Start();
	void stop_timer();
	void on_button_clicked_Step();
	void on_button_clicked_Previous();
	void on_button_clicked_Next();
	
	// Keyboard signal handler:
	bool on_key_press_event(GdkEventKey * key_event);

	// interface components
	Gtk::Box m_Box, m_Box_Left, m_Box_Right;
	MyArea          m_Area;
	Gtk::Label		m_Label_General;
	Gtk::Label		m_Label_Info;
	Gtk::Label		m_Label_Anthillinfo;
	Gtk::Label		m_Label_Informations;
	Gtk::Label		m_Label_nbfood;
	Gtk::Button		m_Button_Exit;
	Gtk::Button		m_Button_Open;
	Gtk::Button		m_Button_Save;
	Gtk::Button		m_Button_Start;
	Gtk::Button		m_Button_Step;
	Gtk::Button		m_Button_Previous;
	Gtk::Button		m_Button_Next;

	bool timer_added;
	bool disconnect;
	const int timeout_value;
};

#endif 
