#ifndef _ArrowDevice_H_
#define _ArrowDevice_H_

#include <string>
#include <gtkmm.h>
//#include "UniLed.h"
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/surface.h>

// -----------------------------------------------------------------------------
/*! ����� ���������� ������ 
*/
class ArrowDevice : public Gtk::Fixed
{
    public:

        ArrowDevice(std::string facial, bool glo_);		/*!< ���������: ������ �� ������� ������ 
																	� ���� ����������� ����������� */
        virtual ~ArrowDevice();

		void setValue(int val);							/*!< �������� ������� ������ �������� */
		void setScale(int minp, int maxp, float step);	/*!< ��������� ����� (default 0,100,20) */
		void setScaleMarksNumber(int num);				/*!< ���������� ��������� ����� ����� �������� */

		void setFacialString(std::string newFacial);	/*!< ������ �� ������� ������ ������� (default Facial) */
		void setInerc(float inerciya);					/*!< ������������� ������� (default 3) */

		void setRedrawTime(int rTime);	/*!< ������ ����������� � ��. (default 60), ������ �� ������������� */
		/*! ���������� �������� ������� */
		void addSect(int startBorder,int endBorder,
				double red,	double green, double blue,double alfa, bool solid);
		void delLastSect();
		
		void setMarksFontSize(int num);				/*!< ������ ������ ����� */
		void setDigitFontSize(int num);				/*!< ������ ������ ��������� �������� */
		void setLabelFontSize(int num);				/*!< ������ ������ ������� */
		void setScaleMarkWidth(int pix);			/*!< ������� ����� */
		void setColorLineWidth(int pix);			/*!< ������� ������� ����� */

    protected:

		void GoDynamics(void);			/*!< ������ ����������� */	
        bool Redraw(void);				/*!< ������ ����������� */
        bool on_expose_event(GdkEventExpose* event); 
		

		/*! ������������� ����� */ 
		virtual void ScaleInit(Cairo::RefPtr<Cairo::Context>& cr);
		/*! ��������� �������� */
		virtual void ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr);
		/*! ��������� ��������� */
		virtual void ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		/*! ��������� �������� ���� (���������) */
		virtual void GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		
		virtual void BlickDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		
//		Gtk::Label label;
//		UniLed led;

		int value;						/*!< �������� �������� �������� */
	    float prevValue,dValue,aValue;	/*!< �������� ��� ��������, aValue - ������������ �������� �������� */
		float x_pos,y_pos;				/*!< ������� ������� */
		std::string facial, devName, myFont;	/*!< ������ �� ������� ������, �������� �������, ����� */

		int minp,maxp;					/*!< max & min �������� �� ����� */
		float step;						/*!< ��� ����� */
		
		float sIndex;					/*!< ����� ������������������ */
		float squareside,width,height;	/*!< ������� �������� - ����������� �� ������ � ������ ������� */

		int redrawTime;					/*!< ����� �����������, ������ �� ������������� */
		float inerc;					/*!< ������������� ������� ( <1-��������������, max 15, default 3) */
		bool gg,overshoot,redr,glo;		/*!< ����� �������������, ����������, ����������� � ����������� �����������
														�������������� */
		
		sigc::connection tmrConn;		/*!< ���������� � �������� ������� */


		Cairo::RefPtr< Cairo::Pattern > scalePatt, glassPatt;

		Cairo::RefPtr< Cairo::RadialGradient> radgrad;

		
		int secCnt;						/*!< ������� ������� �������� */
		struct sects					/*!< ��������� ������������� �������� ������� */
		{	
			int startBorder;			/*!< ������ ������� */
			int endBorder;				/*!< ����� ������� */
			double red;					/*!< ������� �������� */
			double green;				/*!< �������� */
			double blue;				/*!< ������ */
			double alfa;				/*!< �����-����� (��������������) */
			bool solid;					/*!< ���� ������� ����� ������� */
		};

		sects sectList[9];				/*!< ��������� ��������������� ������� �������� */
		
		Pango::FontDescription pFont;

			int SMALL_SCALEMARKS_NUMBER;
			int SCALEMARKS_FONTSIZE;
			int DIGITVAL_FONTSIZE;
			int LABEL_FONTSIZE ;
			int SCALEMARK_WIDTH ;
			int COLORLINE_WIDTH ;

	private:

};

#endif

