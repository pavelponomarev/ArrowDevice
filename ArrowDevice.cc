//------------------------------------------------------------------------------
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cairomm/context.h>
#include "ArrowDevice.h"
//#include "UniLed.h"


static const double LENGHT_ARROW = 0.75;
static const double ARROW_ROOT = 0.88;
static const double FAT_ARROW = 0.9;
static const double X_DIGITVAL = 0.07;
static const double Y_DIGITVAL = 0.17;
static const double X_LABEL = 0.50;
static const double Y_LABEL = 0.63;
static const double LENGHT_SCALEMARK = 0.9;
static const double LENGHT_SMALL_SCALEMARK = 0.95;
static const double RADIUS = 0.78;
static const double SCALE_RANGE = 3.1415/2;
static const double SCALE_SHIFT = 0;


using namespace std;

//------------------------------------------------------------------------------
ArrowDevice::ArrowDevice(string dName, bool glo_):
	value(0),
	facial(dName),
	devName(dName),
	minp(0),maxp(100),step(20),
	squareside(0),
	width(200),
	height(200),
	redrawTime(60),
	inerc(3),
	gg(true),
	redr(true),
	glo(glo_),
	secCnt(0),
	pFont("DSCrystal"),
	SMALL_SCALEMARKS_NUMBER(5),	SCALEMARKS_FONTSIZE(14),
	DIGITVAL_FONTSIZE(24),	LABEL_FONTSIZE(16),
	SCALEMARK_WIDTH(3), COLORLINE_WIDTH(16)
{
	this->set_size_request((int)width,(int)height);

//	label.modify_font(pFont);//Pango::FontDescription("DSCrystal 40"));
//	this->put(label,20,14);
//	label.show();

/*
	led.setTitle("LED Indicator",Gtk::POS_TOP);
	led.show();
	led.setFormat("%d");
	led.show();
*/
	GoDynamics();
}

//------------------------------------------------------------------------------
ArrowDevice::~ArrowDevice()
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool ArrowDevice::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window)
	{
		Gtk::Allocation allocation = get_allocation();
		width = allocation.get_width();
		height = allocation.get_height();

		int temp;
		temp = allocation.get_x();
		if(x_pos != temp)
		{
			x_pos = temp;
			redr = true;
		}
				
		temp = allocation.get_y();
		if(y_pos != temp)
		{
			y_pos = temp;
			redr = true;
		}

		temp = height < width ? height : width;
		if(squareside != temp)
		{
			squareside = temp;
			redr = true;
		}

		sIndex=squareside/200;
	
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
// minp - минимальное значение отображаемого параметра
// maxp - максимальное значение оного
// step - шаг нанесения больших рисок (чисел) на шкалу
// пример : от -600 до 600, шаг 200
// на шкале отобразятся:  -600 -400 -200  0  200  400  600

		cr->rectangle(x_pos,y_pos,squareside,squareside);
		cr->clip();

//		попытка улучшить производительность
		if(glo)
		{
			if(redr)
			{
				cr->push_group();
				ScaleInit(cr);//рисуем шкалу
				scalePatt = cr->pop_group();
				
				cr->push_group();
				GlassDrawing(cr);
				BlickDrawing(cr);			
				glassPatt = cr->pop_group();
				redr = false;
//				cout<<devName<<": redr"<<endl;							
			}
			cr->save();
			cr->set_source(scalePatt);
			cr->paint();
			cr->stroke();
			cr->restore();				
		}
		else
			ScaleInit(cr);//рисуем шкалу

		//инерционность стрелки прибора
			dValue=(value-prevValue)/inerc;
			aValue=prevValue+dValue;
			prevValue=aValue;
		
		//реализация процесса зашкаливания в двух направлениях
		// в оригинале max & min делились на номинал
		overshoot=false;

		if(aValue>maxp)
		{
			aValue = gg ? maxp*1.007 : maxp*1.002;							
			prevValue=aValue;
			gg=!gg;
			overshoot=true;
		}
		else if(aValue<minp)
		{
				if(gg)
					aValue=minp*1.002;
				else 
					aValue = minp ? minp*1.007 : -0.1;
						
				prevValue=aValue;
				gg=!gg;
				overshoot=true;
		}

		ArrowDrawing(cr);		

		if(glo)
		{
			cr->save();
			cr->set_source(glassPatt);
			cr->paint();
			cr->stroke();
			cr->restore();				
		}
		else
		{
			GlassDrawing(cr);//рисуем 
			BlickDrawing(cr);
		}
	}
	return 	Gtk::Fixed::on_expose_event(event);
}

//--------------------------------------------------------------------------------
void ArrowDevice::ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr)
{
	float lengtArrow = LENGHT_ARROW*squareside;//длина стрелки
    float phia = SCALE_RANGE * ( (aValue-minp)/(maxp-minp) );
	if(phia<0) 
		phia=0;
    float center = ARROW_ROOT*squareside; //начало стрелки
	float x1_new = center + x_pos;
	float y1_new = center + y_pos;
	float x2_new = center - lengtArrow*cos( phia - SCALE_SHIFT ) + x_pos;
	float y2_new = center - lengtArrow*sin( phia - SCALE_SHIFT ) + y_pos;
		
	cr->set_line_cap(Cairo::LINE_CAP_ROUND);
			
       // рисуем cтрелку
	cr->set_line_width(1);
	cr->move_to( x1_new, y1_new );
	cr->line_to( x2_new, y2_new );
	cr->stroke(); //тонкая часть
			
	float x2_n = center - FAT_ARROW*lengtArrow*cos( phia - SCALE_SHIFT ) + x_pos;
	float y2_n = center - FAT_ARROW*lengtArrow*sin( phia - SCALE_SHIFT ) + y_pos;
			
	cr->set_line_width(3);
	cr->move_to( x1_new, y1_new );
	cr->line_to( x2_n, y2_n );
	cr->stroke();	//толстая
	
		
	char digitValue[10];
	sprintf(digitValue,"%02d",value);
		
	if(overshoot)
		cr->set_source_rgba(1, 0, 0, 1);   // red

//	const string myFont="DSCrystal Bold 17";
//	cr->select_font_face(Pango::FontDescription("DSCrystal"));
//	cr->select_font_face(myFont,CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);

	cr->set_font_size( DIGITVAL_FONTSIZE*sIndex );
	cr->move_to( X_DIGITVAL*squareside + x_pos, Y_DIGITVAL*squareside + y_pos );
	cr->show_text(digitValue);
	cr->stroke();		

//	move(label,(int)(X_DIGITVAL*squareside),(int)(Y_DIGITVAL*squareside));
//	label.set_size_request(20,20);
//	pFont.set_size(30);//DIGITVAL_FONTSIZE*sIndex);
//	label.modify_font(pFont);

//	label.set_text(digitValue);

/*
		led.setWidgetSize(60*sIndex,26*sIndex);
		led.setWidgetFont("Arial 24");
		this->put(led,squareside/30,squareside/30);
		led.setValue(value);
*/
}

//------------------------------------------------------------------------------
void ArrowDevice::ScaleInit(Cairo::RefPtr<Cairo::Context>& cr)
{//рисуем шкалу

		//всю поверхность красим в черное
		cr->set_source_rgba(0, 0, 0, 1);//black - the colour of the pen
	    cr->paint();
		cr->stroke();
		
		//заливаем белым прямоугольник
		cr->rectangle( squareside/30 + x_pos, squareside/30 + y_pos,
						squareside - 2*squareside/30, squareside - 2*squareside/30 );
		cr->clip();
		cr->set_source_rgba(1, 1, 1, 1);
		cr->paint();
		cr->stroke();

		//shadow
		cr->set_source_rgba(0, 0, 0, 0.3);		
		cr->set_line_width(6);
		cr->move_to( squareside/30 + x_pos, squareside + y_pos );
		cr->line_to( squareside/30 + x_pos, squareside/30 + y_pos );
		cr->line_to( squareside + x_pos, squareside/30 + y_pos );
		cr->stroke();
	
		//пишем какое-нить слово
		cr->set_source_rgba(0, 0, 0, 1);   // black
		cr->set_font_size( LABEL_FONTSIZE*sIndex );	
		cr->move_to( X_LABEL*squareside + x_pos, Y_LABEL*squareside + y_pos );
		cr->show_text(facial);
		cr->stroke();		

		//красим сектора
		ColorizeSectors(cr);

		//начинаем рисовать риски
		static const int tstep[]={10,5,6,4,3,7,2,8,9};
		int i, bigMax=5;
		double phi, lc=LENGHT_SCALEMARK;//lc - длина риски шкалы
	
		if ( maxp*minp < 0 ) 
			bigMax=6;
			
		for ( i = 0; i < 9 && ( step != (maxp-minp)/bigMax ); i++ )
			bigMax = tstep[i];

		int small = 1,big = bigMax;// big - счетчик больших черточек small - счетчик маленьких черточек
				
		float otstoys = RADIUS*squareside;//радиус шкалы
		float center = ARROW_ROOT*squareside;
				// далее идет расчет координат концов рисок (в зависимости от угла j (phi))
				// через каждые пять маленьких рисок (lc=0.95) идет большая (lc=0.9)

		cr->set_source_rgba(0, 0, 0, 1);		
		float x1s,y1s,x2s,y2s;
		for( phi = 0; phi <= SCALE_RANGE; phi += SCALE_RANGE/(SMALL_SCALEMARKS_NUMBER*bigMax) )
		{
			x1s = ( ( center - otstoys*cos( phi - SCALE_SHIFT ) ) + x_pos );
			y1s = ( ( center - otstoys*sin( phi - SCALE_SHIFT ) ) + y_pos );
			x2s = ( ( center - (otstoys*lc)*cos( phi - SCALE_SHIFT ) ) + x_pos );
			y2s = ( ( center - (otstoys*lc)*sin( phi - SCALE_SHIFT ) ) + y_pos );
	
			cr->set_line_width(SCALEMARK_WIDTH);
			cr->move_to(x1s,y1s);
	        cr->line_to(x2s,y2s);
	        cr->stroke();

			if( lc == LENGHT_SCALEMARK )
			{
				char szBuf[25];
				sprintf( szBuf,"%d",(int)( maxp-(big--)*step ) );

//				cr->select_font_face("Arial 16");
				cr->move_to( x2s - 14*sIndex*sin(phi) + 4,
							 y2s + 10*sIndex*sin(phi) + 7*sIndex );
				cr->set_font_size( sIndex*SCALEMARKS_FONTSIZE );
				cr->show_text(szBuf);
				cr->stroke();		
			}
			
			lc=LENGHT_SMALL_SCALEMARK;

			if( small == SMALL_SCALEMARKS_NUMBER && phi != 0)
			{
				small=0; 
				lc=LENGHT_SCALEMARK;
			}
	   		small++;
		}
}

//------------------------------------------------------------------------------
void ArrowDevice::ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr)
{
	float theta1,theta2;

	float otstoys = RADIUS*squareside;//радиус шкалы
	float center = ARROW_ROOT*squareside;

	for(int i = 0; i < secCnt; i++)
	{
		theta1 = SCALE_RANGE*(( (float)sectList[i].startBorder - minp )/( maxp - minp ) );
		theta2 = SCALE_RANGE*(( (float)sectList[i].endBorder - 
								(float)sectList[i].startBorder )/( maxp - minp ) );
		cr->set_source_rgba(sectList[i].red, sectList[i].green, 
							sectList[i].blue, sectList[i].alfa);
				
		if(sectList[i].solid)
		{
			cr->set_line_width(0);
			cr->arc( center + x_pos, center + y_pos,
				 otstoys, M_PI + theta1 - SCALE_SHIFT, M_PI + theta2 + theta1 - SCALE_SHIFT );
			cr->line_to( center + x_pos, center + y_pos );
			cr->close_path();
			cr->fill_preserve();
		}
		else
		{
			cr->set_line_width(COLORLINE_WIDTH*sIndex);
			cr->arc(center+x_pos, center+y_pos, 
				 otstoys-8*sIndex, M_PI+theta1 - SCALE_SHIFT, M_PI+theta2+theta1 - SCALE_SHIFT);
		}
		cr->stroke();
	}	
}

//------------------------------------------------------------------------------
void ArrowDevice::GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr)
{
/*	cr->rectangle(x_pos,y_pos,squareside,squareside);
	cr->clip();
*/
	//штука, которая закрывает основание стрелки
	cr->arc( squareside + x_pos, squareside + y_pos, squareside*0.3, -(M_PI/2), M_PI );
	cr->close_path();
	cr->fill_preserve();
	cr->stroke();		
}

//------------------------------------------------------------------------------
void ArrowDevice::BlickDrawing(Cairo::RefPtr<Cairo::Context>& cr)
{
/*
	radgrad = Cairo::RadialGradient::create(0.1*squareside+x_pos,0.2*squareside+y_pos,0.05*squareside,
												0.3*squareside+x_pos,0.3*squareside+y_pos,0.4*squareside);
	radgrad->add_color_stop_rgba(0, 1, 1, 1, 0.8);
	radgrad->add_color_stop_rgba(0.3, 1, 1, 1, 0.2);															
	radgrad->add_color_stop_rgba(1, 1, 1, 1, 0);

	cr->save();
	cr->set_source(radgrad);
	cr->paint();
	cr->stroke();
	cr->restore();
*/
}

//------------------------------------------------------------------------------
void ArrowDevice::setFacialString(string newFacial)
{
	facial = newFacial;
	redr = true;
}
//------------------------------------------------------------------------------
void ArrowDevice::setValue(int val)
{
	value = val;
	GoDynamics();
	
}
//------------------------------------------------------------------------------
void ArrowDevice::setScale(int newMin, int newMax, float newStep)
{
	minp = newMin;
	maxp = newMax;
	step = newStep;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::setScaleMarksNumber(int num)
{
	SMALL_SCALEMARKS_NUMBER = num;
	redr = true;
}

//------------------------------------------------------------------------------
bool ArrowDevice::Redraw(void)
{
	this->queue_draw();

	if( abs(value-aValue) <= 0.004*(maxp-minp))
		return false;

    return true;
}

//------------------------------------------------------------------------------
void ArrowDevice::GoDynamics(void)
{
	tmrConn.disconnect();
	tmrConn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &ArrowDevice::Redraw), redrawTime);
}

//------------------------------------------------------------------------------
void ArrowDevice::setInerc(float inerciya)
{
	if( inerciya < 1.1 )
		inerc = 1.1;
	else
		inerc = inerciya>15 ? 15 : inerciya;
}

//------------------------------------------------------------------------------
void ArrowDevice::setRedrawTime(int rTime)
{
	if( rTime < 20 )
		redrawTime = 20;
	else
		redrawTime = rTime>1000 ? 1000 : rTime;
}

//------------------------------------------------------------------------------
void ArrowDevice::addSect(int startBorderN,int endBorderN,
			double redN,double greenN, double blueN,double alfaN,bool solidN)
{
	sectList[secCnt].startBorder = startBorderN;
	sectList[secCnt].endBorder = endBorderN;	
	sectList[secCnt].red = redN;
	sectList[secCnt].green = greenN;
	sectList[secCnt].blue = blueN;
	sectList[secCnt].alfa = alfaN;
	sectList[secCnt++].solid = solidN;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::delLastSect()
{
	secCnt--;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::setMarksFontSize(int num)
{
	SCALEMARKS_FONTSIZE = num;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::setDigitFontSize(int num)
{
	DIGITVAL_FONTSIZE = num;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::setLabelFontSize(int num)
{
	LABEL_FONTSIZE = num;
	redr = true;
}

//------------------------------------------------------------------------------		
void ArrowDevice::setScaleMarkWidth(int pix)
{
	SCALEMARK_WIDTH = pix;
	redr = true;
}

//------------------------------------------------------------------------------
void ArrowDevice::setColorLineWidth(int pix)
{
	COLORLINE_WIDTH = pix;
	redr = true;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

