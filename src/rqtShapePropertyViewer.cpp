#include "rqtShapePropertyViewer.hpp"

rqtShapePropertyViewer::rqtShapePropertyViewer(wxWindow *parent, wxWindowID id)
:wxPropertyGrid(parent , id)
{

}

void rqtShapePropertyViewer::DisplayShape(rqtShape* shape){
	Clear();

	if (!shape)
		return;

	Append( new wxPropertyCategory("Item",wxPG_LABEL) );
	Append( new wxStringProperty("Name",wxPG_LABEL,shape->Name()) );
	Append( new wxColourProperty("Color",wxPG_LABEL,shape->Color()) );

	wxString type = shape->GetType();

	if (type == "Point")
		DisplayPoint(static_cast<rqtPoint*>(shape));
	else if (type == "Rectangle")
		DisplayRectangle(static_cast<rqtRectangle*>(shape));
	else if (type == "Circle")
		DisplayCircle(static_cast<rqtCircle*>(shape));
}

void rqtShapePropertyViewer::DisplayPoint(rqtPoint* point){
	Append( new wxPropertyCategory("Point",wxPG_LABEL) );
	rVector2 pt = point->Point();

	Append( new wxFloatProperty("x",wxPG_LABEL,pt.x) );
	Append( new wxFloatProperty("y",wxPG_LABEL,pt.y) );
}

void rqtShapePropertyViewer::DisplayRectangle(rqtRectangle* rectangle){
	Append( new wxPropertyCategory("Rectangle",wxPG_LABEL) );

	rRectangle2 r = rectangle->Rectangle();

	Append( new wxFloatProperty("x",wxPG_LABEL,r.x) );
	Append( new wxFloatProperty("y",wxPG_LABEL,r.y) );

	Append( new wxFloatProperty("width",wxPG_LABEL,r.width) );
	Append( new wxFloatProperty("height",wxPG_LABEL,r.height) );
}

void rqtShapePropertyViewer::DisplayCircle(rqtCircle* circle){
	Append( new wxPropertyCategory("Circle",wxPG_LABEL) );

	rCircle2 c = circle->Circle();

	Append( new wxFloatProperty("center x",wxPG_LABEL,c.center.x) );
	Append( new wxFloatProperty("center y",wxPG_LABEL,c.center.y) );
	Append( new wxFloatProperty("radius",wxPG_LABEL,c.radius) );
}