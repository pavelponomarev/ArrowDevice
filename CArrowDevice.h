#ifndef _CArrowDevice_H_
#define _CArrowDevice_H_

#include "ArrowDevice.h"
// -----------------------------------------------------------------------------
/*! ����� ������ ������� ���������� ������ 
*/
class CArrowDevice : public ArrowDevice
{
    public:

        CArrowDevice(std::string facial, bool glo_);
        virtual ~CArrowDevice();

    protected:

		void ScaleInit(Cairo::RefPtr<Cairo::Context>& cr);
		void ColorizeSectors(Cairo::RefPtr<Cairo::Context>& cr);
		void ArrowDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		void GlassDrawing(Cairo::RefPtr<Cairo::Context>& cr);
		void BlickDrawing(Cairo::RefPtr<Cairo::Context>& cr);
	private:
		
		
};

#endif

