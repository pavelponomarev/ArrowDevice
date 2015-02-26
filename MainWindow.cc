#include "MainWindow.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
//#include "EArrowDevice.h"
//#include "CArrowDevice.h"

//------------------------------------------------------------------------------

//using namespace sigc;

using namespace std;


//------------------------------------------------------------------------------
MainWindow::MainWindow():
	dev1("dev1", 1),
	dev2("dev2", 1),//dev3("dev3"),dev4("dev4"),
	dev5("value %", 1),
	dev6("energy", 1),
	devC("circle dev", 1),
	vbox(false,0),
	flag(false)
{
	dev2.setFacialString("H*m");
	dev1.setFacialString("mass %");
//	dev4.setFacialString(Glib::locale_to_utf8("נעיגןע"));

	dev1.setScale(0,600,150);
	dev1.setScaleMarksNumber(4);
	dev1.addSect(0,450,0,1.,0.,0.4,0);
	dev1.addSect(450,600,1.,1.,0,0.4,0);
	dev1.setScaleMarkWidth(1);
	
	dev2.setScale(20,40,10);
	dev2.setScaleMarksNumber(10);
	dev2.addSect(25,30,0,1,0,0.3,1);
	dev2.addSect(30,35,1,1,0,0.6,0);
	dev2.addSect(35,40,1,0,0,1,0);
		
//	dev3.setSectors(20,45,60,80);	

//	dev4.setScale(0,100,20);

	dev5.setScale(0,100,20);
	dev5.addSect(0,40,1,1,0,0.6,1);
	dev5.addSect(40,80,0,0,1,0.2,1);
	dev5.addSect(80,100,1,0,0,0.4,1);

	dev6.setScale(-100,100,20);
	dev6.addSect(-100,-80,1.,0.,0.,0.9,0);
	dev6.addSect(-80,-60,1,1,0,0.9,0);
	dev6.addSect(-60,-20,0,1,0,0.9,0);
	dev6.addSect(0,20,0.8,0.8,0.8,0.4,1);
	dev6.addSect(20,60,0.3,0.7,0.9,0.4,1);	
	dev6.addSect(60,80,0.7,0.7,0.7,0.5,1);
	dev6.addSect(80,100,0.2,0.2,0.3,0.5,1);


	devC.setScale(-100,100,20);
	devC.addSect(-100,-80,0.6,0.,0.,1,0);
	devC.addSect(-80,-60,0.8,0.5,0,1,0);
	devC.addSect(-60,-20,0,0.5,0,1,0);
	devC.addSect(20,60,0,0.5,0,1,0);	
	devC.addSect(60,80,0.5,0.5,0,1,0);
	devC.addSect(80,100,0.5,0,0,1,0);
	
	devC.setScaleMarkWidth(2);


	dev1.setInerc(6);
	dev2.setInerc(18);
//	dev3.setInerc(0);
//	dev4.setInerc(20);

	dev2.set_size_request(300,300);
	dev6.set_size_request(350,350);
//	dev3.set_size_request(250,150);
	
	box1.add(dev1);
//	box1.add(vsep1);
	box1.add(dev2);
	box1.add(devC);
//	box1.add(vsep2);	
//	box1.add(dev3);
	
//	box2.add(dev4);
	box2.add(dev5);
	box2.add(dev6);
	
	vbox.add(box1);
	vbox.add(box2);

	add(vbox);

	dev1.setValue(700);
	dev2.setValue(22);
//	dev3.setValue(25);
//	dev4.setValue(10);
	dev5.setValue(78);
	dev6.setValue(-40);
	
	property_window_position() = Gtk::WIN_POS_CENTER;
	set_title(Glib::locale_to_utf8("ArrowDevices"));
	show_all();

	conn = Glib::signal_timeout().connect(sigc::mem_fun(this, &MainWindow::setNewValues),5000);
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
bool MainWindow::setNewValues(void)
{
	if(flag)
	{
	
		dev1.setValue(700);
		dev2.setValue(22);
//		dev3.setValue(5);
//		dev4.setValue(10);
		dev5.setValue(98);
		dev6.setValue(-40);
		devC.setValue(107);
	}
	else
	{
		dev1.setValue(200);
		dev2.setValue(39);
//		dev3.setValue(100);
//		dev4.setValue(80);
		dev5.setValue(43);
		dev6.setValue(107);
		devC.setValue(-40);
	}
	flag=!flag;
	
	return true;
}

