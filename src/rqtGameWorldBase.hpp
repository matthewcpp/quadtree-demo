#ifndef RQT_GAMEWORLDBASE_HPP
#define RQT_GAMEWORLDBASE_HPP

#include <wx/wx.h>

#include "rqtTypes.hpp"

#include "rqtShapeBase.hpp"

class rqtShape;


class rqtGameWorldBase{
public:
	virtual bool UpdateItem(rqtShape* shape , const rVector2& p) = 0;
	virtual bool UpdateItem(rqtShape* shape , const rRectangle2& r) = 0;
	virtual bool UpdateItem(rqtShape* shape , const rCircle2& c) = 0;

	virtual rRectangle2 WorldArea() const = 0;
};

#endif