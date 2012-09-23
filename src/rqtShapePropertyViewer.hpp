#ifndef RQT_SHAPEPROPERTYVIEWER_HPP
#define RQT_SHAPEPROPERTYVIEWER_HPP

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "rqtShapes.hpp"

class rqtShapePropertyViewer : public wxPropertyGrid{
public:
	rqtShapePropertyViewer(wxWindow *parent, wxWindowID id=wxID_ANY);

	void DisplayShape(rqtShape* shape);

private:
	void DisplayPoint(rqtPoint* point);
	void DisplayRectangle(rqtRectangle* rectangle);
	void DisplayCircle(rqtCircle* circle);


};

#endif