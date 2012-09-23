#ifndef RQT_QUADTREECONTROLLER_I_HPP
#define RQT_QUADTREECONTROLLER_I_HPP

#include <wx/wx.h>
#include "rqtTypes.hpp"
#include "rqtShapes.hpp"

class rqtQuadtreeControllerI{
public:
	virtual bool CreatePointObject(const wxString& name , const wxColor& color , const rVector2& p) = 0;
	virtual bool CreateRectangleObject(const wxString& name , const wxColor& color , const rRectangle2& r) = 0;
	virtual bool CreateCircleObject(const wxString& name , const wxColor& color , const rCircle2& circle) = 0;
	
	
	virtual void QueryPoint(const rVector2& p) = 0;
	virtual void QueryRectangle(const rRectangle2& r) = 0;
	virtual void QueryCircle(const rCircle2& c) = 0;

	virtual wxString GetNextItemName() = 0;

	virtual rqtShape* SelectItemByName(const wxString& name) = 0;
	virtual rqtShape* SelectItemByPoint(const rVector2& p) = 0;
	virtual rqtShape* GetSelectedItem() = 0;

	virtual void UpdateObjectSelection() = 0;
	virtual bool ShapeFitsInWorld(const rqtShape* shape) = 0;
};

#endif