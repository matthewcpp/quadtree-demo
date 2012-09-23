#include "rqtShapes.hpp"



rqtShape::rqtShape(const wxString& name, const wxColor& color){
	m_name = name;
	m_color = color;
}

rqtPoint::rqtPoint(const wxString& name, const wxColor& color, const rVector2& p)
	:rqtShape(name , color)
{
	m_point = p;
}

rqtRectangle::rqtRectangle(const wxString& name, const wxColor& color, const rRectangle2& r)
	:rqtShape(name , color)
{
	m_rectangle = r;
}

rqtCircle::rqtCircle(const wxString& name, const wxColor& color, const rCircle2& c)
	:rqtShape(name , color)
{
	m_circle = c;
}

void rqtPoint::Draw(rRendererI* renderer) const{
	renderer->DrawPoint(m_point , m_color);
}

void rqtRectangle::Draw(rRendererI* renderer) const{
	renderer->DrawRectangle(m_rectangle , m_color);
}

void rqtCircle::Draw(rRendererI* renderer) const{
	renderer->DrawCircle(m_circle , m_color);
}

wxString rqtPoint::GetType() const{
	return "Point";
}
wxString rqtRectangle::GetType() const{
	return "Rectangle";
}
wxString rqtCircle::GetType() const{
	return "Circle";
}

wxString rqtPoint::GetInfo() const{
	return wxString::Format("x: %.2f y: %.2f" , m_point.x , m_point.y);
}
wxString rqtRectangle::GetInfo() const{
	return wxString::Format("x: %.2f y: %.2f width: %.2f height: %.2f" , m_rectangle.x , m_rectangle.y, m_rectangle.width , m_rectangle.height);
}
wxString rqtCircle::GetInfo() const{
	return wxString::Format("center x: %.2f center y: %.2f radius: %f" , m_circle.center.x , m_circle.center.y, m_circle.radius);
}

bool rqtPoint::IntersectsRect(const rRectangle2& r) const{
	return r.ContainsPoint(m_point);
}

bool rqtRectangle::IntersectsRect(const rRectangle2& r) const{
	return r.IntersectsRectangle(m_rectangle);
}

bool rqtCircle::IntersectsRect(const rRectangle2& r)const{
	return rCollision2::RectangleInstersectsCircle(r , m_circle) ;
}

bool rqtPoint::SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world){
	if (rqtShape::SetProperty(key , value , world))
            return true;

	rVector2 p = m_point;

	if (key == "x")
		p.x = value.As<float>();
	else if (key == "y")
		p.y = value.As<float>();

	if (world->UpdateItem(this , p)){
		m_point = p;
		return true;
	}

	return false;
}

bool rqtRectangle::SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world){
	if (rqtShape::SetProperty(key , value , world))
            return true;

	rRectangle2 r = m_rectangle;

	if (key == "x")
		r.x = value.As<float>();
	else if (key == "y")
		r.y = value.As<float>();
	else if (key == "width")
		r.width = value.As<float>();
	else if (key == "height")
		r.height = value.As<float>();

	if (world->UpdateItem(this , r)){
		m_rectangle = r;
		return true;
	}

	return false;
}

bool rqtCircle::SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world){
	if (rqtShape::SetProperty(key , value , world))
            return true;

	rCircle2 c = m_circle;

	if (key == "center x")
		c.center.x = value.As<float>();
	else if (key == "center y")
		c.center.y = value.As<float>();
	else if (key == "radius")
		c.radius = value.As<float>();

	if (world->UpdateItem(this , c)){
		m_circle = c;
		return true;
	}

	return false;
}

rVector2 rqtPoint::GetPosition() const{
	return m_point;
}

rVector2 rqtRectangle::GetPosition() const{
	return rVector2(m_rectangle.x , m_rectangle.y);
}

rVector2 rqtCircle::GetPosition() const{
	return m_circle.center;
}

void rqtPoint::SetPosition(const rVector2& v){
	m_point = v;
}

void rqtRectangle::SetPosition(const rVector2& v){
	m_rectangle.x = v.x;
	m_rectangle.y = v.y;
}
void rqtCircle::SetPosition(const rVector2& v){
m_circle.center = v;
}

void rqtRectangle::SerializeShape(wxXmlNode* parent){
	wxXmlNode* shape = new wxXmlNode(wxXML_ELEMENT_NODE ,GetType());
	parent->AddChild(shape);

	rqtShape::SerializeShape(shape);

	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE ,"x");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "x" , wxString::Format("%f", m_rectangle.x)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"y");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "y" , wxString::Format("%f", m_rectangle.y)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"width");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "width" , wxString::Format("%f", m_rectangle.width)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"height");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "height" , wxString::Format("%f", m_rectangle.height)));
	shape->AddChild(node);

}
void rqtPoint::SerializeShape(wxXmlNode* parent){
	wxXmlNode* shape = new wxXmlNode(wxXML_ELEMENT_NODE ,GetType());
	parent->AddChild(shape);

	rqtShape::SerializeShape(shape);

	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE ,"x");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "x" , wxString::Format("%f", m_point.x)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"y");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "y" , wxString::Format("%f", m_point.y)));
	shape->AddChild(node);
}

void rqtCircle::SerializeShape(wxXmlNode* parent){
	wxXmlNode* shape = new wxXmlNode(wxXML_ELEMENT_NODE ,GetType());
	parent->AddChild(shape);

	rqtShape::SerializeShape(shape);

	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE ,"x");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "x" , wxString::Format("%f", m_circle.center.x)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"y");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "y" , wxString::Format("%f", m_circle.center.y)));
	shape->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"radius");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "radius" , wxString::Format("%f", m_circle.radius)));
	shape->AddChild(node);
}

bool rqtCircle::ContainsPoint(const rVector2& v) const{
	return m_circle.ContainsPoint(v);
}

bool rqtPoint::ContainsPoint(const rVector2& v) const{
	return m_point.Distance(v) >= 1.0f;
}

bool rqtRectangle::ContainsPoint(const rVector2& v) const{
	return m_rectangle.ContainsPoint(v);
}
