#ifndef _ArrowDevice_H_
#define _ArrowDevice_H_

#include <string>
#include <gtkmm.h>
//#include "UniLed.h"
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/surface.h>

// -----------------------------------------------------------------------------
/*! Класс Стрелочный прибор 
*/
class ArrowDevice : public Gtk::Fixed
{
    public:

        ArrowDevice(std::string facial, bool glo_);		/*!< параметры: строка на лицевой панели 
																	и флаг графической оптимизации */
        virtual ~ArrowDevice();

		void setValue(int val);							/*!< передача прибору нового значения */
		void setScale(int minp, int maxp, float step);	/*!< установка шкалы (default 0,100,20) */
		void setScaleMarksNumber(int num);				/*!< количество маленьких рисок между большими */

		void setFacialString(std::string newFacial);	/*!< строка на лицевой панели прибора (default Facial) */
		void setInerc(float inerciya);					/*!< инерционность прибора (default 3) */

		void setRedrawTime(int rTime);	/*!< период перерисовки в мс. (default 60), влияет на инерционность */
		/*! добавление цветного сектора */
		void addSect(int startBorder,int endBorder,
				double red,	double green, double blue,double alfa, bool solid);
		void delLastSect();
		
		void setMarksFontSize(int num);				/*!< размер шрифта шкалы */
		void setDigitFontSize(int num);				/*!< размер шрифта числового значения */
		void setLabelFontSize(int num);				/*!< размер шрифта надписи */
		void setScaleMarkWidth(int pix);			/*!< толщина рисок */
		void setColorLineWidth(int pix);			/*!< толщина цветной линии */

    protected:

		void GoDynamics(void);			/*!< фунция перерисовки */	
        bool Redraw(void);				/*!< фунция перерисовки */
        bool on_expose_event(GdkEventExpose* event); 
		

		/*! инициализация шкалы */ 
		virtual void ScaleInit(Cairo::RefPtr<Cairo::Context>& cr);
		/*! раскраска секторов */
		virtual void ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr);
		/*! рисование стрелочки */
		virtual void ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		/*! рисование верхнего слоя (стеклышка) */
		virtual void GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		
		virtual void BlickDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		
//		Gtk::Label label;
//		UniLed led;

		int value;						/*!< реальное значение величины */
	    float prevValue,dValue,aValue;	/*!< значения для динамики, aValue - показываемое стрелкой значение */
		float x_pos,y_pos;				/*!< позиция виджета */
		std::string facial, devName, myFont;	/*!< строка на лицевой панели, название девайса, шрифт */

		int minp,maxp;					/*!< max & min значения на шкале */
		float step;						/*!< шаг шкалы */
		
		float sIndex;					/*!< индех пропорциональности */
		float squareside,width,height;	/*!< сторона квадрата - минимальное из ширины и высоты виджета */

		int redrawTime;					/*!< время перерисовки, влияет на инерционность */
		float inerc;					/*!< инерционность стрелки ( <1-безынерционная, max 15, default 3) */
		bool gg,overshoot,redr,glo;		/*!< флаги инерционности, перегрузки, перерисовки и графической оптимизации
														соответственно */
		
		sigc::connection tmrConn;		/*!< соединение с сигналом таймера */


		Cairo::RefPtr< Cairo::Pattern > scalePatt, glassPatt;

		Cairo::RefPtr< Cairo::RadialGradient> radgrad;

		
		int secCnt;						/*!< счетчик цветных секторов */
		struct sects					/*!< структура инициализации цветного сектора */
		{	
			int startBorder;			/*!< начало сектора */
			int endBorder;				/*!< конец сектора */
			double red;					/*!< уровень красного */
			double green;				/*!< зеленого */
			double blue;				/*!< синего */
			double alfa;				/*!< альфа-канал (непрозрачность) */
			bool solid;					/*!< флаг заливки всего сектора */
		};

		sects sectList[9];				/*!< хранилище инициализаторов цветных секторов */
		
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

