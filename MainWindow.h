// -----------------------------------------------------------------------------
#ifndef _MainWindow_H_
#define _MainWindow_H_
// -----------------------------------------------------------------------------
#include <gtkmm.h>
#include "ArrowDevice.h"
#include "EArrowDevice.h"
#include "CArrowDevice.h"
// -----------------------------------------------------------------------------
class MainWindow:
	public Gtk::Window
{
	public:
	
		MainWindow();
		virtual ~MainWindow();

	protected:
		sigc::connection conn;
		ArrowDevice dev1,dev2;//,dev3,dev4;
		EArrowDevice dev6;
		CArrowDevice devC,dev5;
//		Gtk::VSeparator vsep1,vsep2;
		
		Gtk::HBox box1,box2;
		Gtk::VBox vbox;
		bool setNewValues(void);
		bool flag;

	
	private:

};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
