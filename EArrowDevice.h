#ifndef _EArrowDevice_H_
#define _EArrowDevice_H_

#include "ArrowDevice.h"
// -----------------------------------------------------------------------------
/*! Класс Трехквадрантный Стрелочный прибор 
*/
class EArrowDevice : public ArrowDevice
{
    public:

        EArrowDevice(std::string facial, bool glo_);
        virtual ~EArrowDevice();

    protected:

		void ScaleInit(Cairo::RefPtr<Cairo::Context>& cr);
		void ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr);
		void ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		void GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr);

	private:
		
		
};

#endif

