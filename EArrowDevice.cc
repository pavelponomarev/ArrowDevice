//------------------------------------------------------------------------------
#include "EArrowDevice.h"

	static const double LENGHT_ARROW = 0.43;
	static const double ARROW_ROOT = 0.5;
	static const double FAT_ARROW = 0.9;
	static const double X_DIGITVAL = 0.4;
	static const double Y_DIGITVAL = 0.72;
	static const double X_LABEL = 0.36;
	static const double Y_LABEL = 0.92;
	static const double LENGHT_SCALEMARK = 0.87;
	static const double LENGHT_SMALL_SCALEMARK = 0.93;
	static const double RADIUS = 0.45;
	static const double SCALE_RANGE = 3*3.1415/2;
	static const double SCALE_SHIFT = 3.1415/4;
	
//	static const int SCALEMARK_WIDTH = 3;
//	static const int COLORLINE_WIDTH = 14;


using namespace std;
//------------------------------------------------------------------------------
EArrowDevice::EArrowDevice(string dName, bool glo_):
	ArrowDevice(dName, glo_)
{
	COLORLINE_WIDTH-=4;
	setScale(0,100,20);
	this->set_size_request(200,200);
}

//------------------------------------------------------------------------------
EArrowDevice::~EArrowDevice()
{
}

//------------------------------------------------------------------------------
void EArrowDevice::ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr)
{

		float lengtArrow = LENGHT_ARROW*squareside;//длина стрелки
	    float phia = SCALE_RANGE *( ( aValue-minp)/(maxp-minp) );
		if(phia<0) 
			phia=0;
        float centerxy=ARROW_ROOT*squareside; //начало стрелки
//		float centery=ARROW_ROOT*squareside;
		float x1_new=centerxy+x_pos;
		float y1_new=centerxy+y_pos;
		float x2_new=centerxy-lengtArrow*cos(phia-SCALE_SHIFT)+x_pos;
		float y2_new=centerxy-lengtArrow*sin(phia-SCALE_SHIFT)+y_pos;
		
		cr->set_line_cap(Cairo::LINE_CAP_ROUND);
			
        // рисуем cтрелку
		cr->set_line_width(1);
		cr->move_to(x1_new,y1_new);
		cr->line_to(x2_new,y2_new);
		cr->stroke(); //тонкая часть
			
		float x2_n=centerxy-FAT_ARROW*lengtArrow*cos(phia-SCALE_SHIFT)+x_pos;
		float y2_n=centerxy-FAT_ARROW*lengtArrow*sin(phia-SCALE_SHIFT)+y_pos;
			
		cr->set_line_width(3);
		cr->move_to(x1_new,y1_new);
		cr->line_to(x2_n,y2_n);
		cr->stroke();		//толстая
	
	
		if(overshoot)
			cr->set_source_rgba(1, 0, 0, 1);   // red
		else
			cr->set_source_rgba(0, 0, 0, 1);

		char digitValue[10];
		sprintf(digitValue,"%02d",value);
			  
//		const string myFont="cristal";
//		cr->select_font_face(myFont,CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
		cr->set_font_size(DIGITVAL_FONTSIZE*sIndex);
		cr->move_to(X_DIGITVAL*squareside+x_pos,Y_DIGITVAL*squareside+y_pos);
		cr->show_text(digitValue);
		cr->stroke();		

//		label.set_text(DigitValue);

/*
		led.setWidgetSize(60*sIndex,26*sIndex);
		led.setWidgetFont("Arial 24");
		this->put(led,squareside/30,squareside/30);
		led.setValue(value);
*/

}

//------------------------------------------------------------------------------
void EArrowDevice::ScaleInit(Cairo::RefPtr<Cairo::Context>& cr)
{//рисуем шкалу

		//всю поверхность красим в черное
//		cr->rectangle(x_pos,y_pos,squareside,squareside);
//		cr->clip();
		cr->set_source_rgba(0, 0, 0, 1);//black - the colour of the pen
	    cr->paint();
		cr->stroke();
		
		//заливаем белым прямоугольник
		cr->rectangle(squareside/30+x_pos,squareside/30+y_pos,
						squareside-2*squareside/30,squareside-2*squareside/30);
		cr->clip();
		cr->set_source_rgba(1, 1, 1, 1);
		cr->paint();
		cr->stroke();

		//shadow
		cr->set_source_rgba(0, 0, 0, 0.3);		
		cr->set_line_width(6);
		cr->move_to(squareside/30+x_pos,squareside+y_pos);
		cr->line_to(squareside/30+x_pos,squareside/30+y_pos);
		cr->line_to(squareside+x_pos,squareside/30+y_pos);
		cr->stroke();


		//пишем какое-нить слово
		cr->set_source_rgba(0, 0, 0, 1);   // black
		cr->set_font_size( LABEL_FONTSIZE*sIndex );	
		cr->move_to( X_LABEL*squareside + x_pos, Y_LABEL*squareside + y_pos);
		cr->show_text(facial);
		cr->stroke();		

		//красим сектора
		ColorizeSectors(cr);

		//начинаем рисовать риски
		int i, tstep[]={10,5,6,4,3,7,2,8,9}, bigMax=5;
		double phi, lc=LENGHT_SCALEMARK;//lc - длина риски шкалы
	
		if (maxp*minp<0) 
			bigMax=6;
			
		for (i=0;i<7 && ((step!=(maxp-minp)/bigMax));i++)
			bigMax=tstep[i];

		int small=1,big=bigMax;// big - счетчик больших черточек small - счетчик маленьких черточек
				
		float otstoys=RADIUS*squareside;//радиус шкалы
		float centerxy=ARROW_ROOT*squareside;
//		float centery=0.5*squareside;
				// далее идет расчет координат концов рисок (в зависимости от угла j (phi))
				// через каждые пять маленьких рисок (lc=0.95) идет большая (lc=0.9)

		cr->set_source_rgba(0, 0, 0, 1);		
		
		float x1s,y1s,x2s,y2s;

		for( phi=0; phi<=(SCALE_RANGE); phi+=SCALE_RANGE/(SMALL_SCALEMARKS_NUMBER*bigMax) )
		{
			x1s=( (centerxy-otstoys*cos(phi-SCALE_SHIFT))+x_pos );
			y1s=( (centerxy-otstoys*sin(phi-SCALE_SHIFT))+y_pos );
			x2s=( (centerxy-(otstoys*lc)*cos(phi-SCALE_SHIFT))+x_pos );
			y2s=( (centerxy-(otstoys*lc)*sin(phi-SCALE_SHIFT))+y_pos );
	
			cr->set_line_width(SCALEMARK_WIDTH);
			cr->move_to(x1s,y1s);
	        cr->line_to(x2s,y2s);
	        cr->stroke();

			if(lc==LENGHT_SCALEMARK)
			{
				char szBuf[25];
				sprintf( szBuf,"%d",(int)( maxp-(big--)*step ) );

//				cr->select_font_face("Arial 16");
				if(phi==0)
					cr->move_to(x2s,y2s);
				else
					cr->move_to( x2s-54*sIndex*sin(phi*0.15)+8*sIndex,
							y2s+16*sIndex*sin(phi*0.66)-3);

				cr->set_font_size(sIndex*SCALEMARKS_FONTSIZE);
				cr->show_text(szBuf);
				cr->stroke();		
			}
			
			lc=LENGHT_SMALL_SCALEMARK;

			if(small==SMALL_SCALEMARKS_NUMBER && phi!=0)
			{
				small=0; 
				lc=LENGHT_SCALEMARK;
			}
	   		small++;
		}
}

//------------------------------------------------------------------------------
void EArrowDevice::ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr)
{
	float theta1,theta2;

	float otstoys=RADIUS*squareside;//радиус шкалы
	float center=ARROW_ROOT*squareside;

	for(int i=0;i<secCnt;i++)
	{
		theta1=SCALE_RANGE*(((float)sectList[i].startBorder-minp)/(maxp-minp) );
		theta2=SCALE_RANGE*(((float)sectList[i].endBorder-
					(float)sectList[i].startBorder)/(maxp-minp));
		cr->set_source_rgba(sectList[i].red, sectList[i].green, 
							sectList[i].blue, sectList[i].alfa);
				
		if(sectList[i].solid)
		{
			cr->set_line_width(0);
			cr->arc(center+x_pos, center+y_pos, otstoys-8*sIndex, 
					M_PI+theta1-SCALE_SHIFT, M_PI+theta2+theta1-SCALE_SHIFT);
			cr->line_to(center+x_pos, center+y_pos);
			cr->close_path();
			cr->fill_preserve();
		}
		else
		{
			cr->set_line_width(COLORLINE_WIDTH*sIndex);
			cr->arc(center+x_pos, center+y_pos, otstoys-COLORLINE_WIDTH/2*sIndex,
					 M_PI+theta1-SCALE_SHIFT, M_PI+theta2+theta1-SCALE_SHIFT);
		}
		cr->stroke();
	}	
}


//------------------------------------------------------------------------------
void EArrowDevice::GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr)
{
        float centerxy=ARROW_ROOT*squareside; //начало стрелки
			//штука, которая закрывает основание стрелки
			cr->set_source_rgba(0, 0, 0, 0.2);
			cr->arc(centerxy+x_pos+1,centerxy+y_pos+1,
						(int)squareside*0.1,0,2*M_PI);
			cr->fill_preserve();
			cr->stroke();		
	
			cr->set_source_rgba(1, 1, 1, 1);
			cr->arc(centerxy+x_pos,centerxy+y_pos,
						(int)squareside*0.09,0,2*M_PI);
			cr->fill_preserve();
			cr->stroke();		

			cr->set_source_rgba(0.3, 0.3, 0.3, 1);
			cr->arc(centerxy+x_pos,centerxy+y_pos,
						(int)squareside*0.01,0,2*M_PI);
			cr->fill_preserve();
			cr->stroke();		

}
//------------------------------------------------------------------------------

