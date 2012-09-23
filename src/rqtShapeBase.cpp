#include "rqtShapeBase.hpp"

bool rqtShape::SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world){
	if (key == "Color"){
		wxColourPropertyValue cpv = wxANY_AS(value, wxColour);
		m_color = cpv.m_colour;
		return true;
	}

	return false;
}

void rqtShape::SerializeShape(wxXmlNode* parent){


	wxXmlNode* nameNode = new wxXmlNode(wxXML_ELEMENT_NODE ,"name");
	nameNode->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "name" , m_name));
	parent->AddChild(nameNode);

	wxXmlNode*  colorNode = new wxXmlNode(wxXML_ELEMENT_NODE ,"color");
	parent->AddChild(colorNode);

	wxXmlNode* c = new wxXmlNode(wxXML_ELEMENT_NODE ,"r");
	c->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "r" ,wxString::Format("%u", m_color.Red())));
	colorNode->AddChild(c);

	c = new wxXmlNode(wxXML_ELEMENT_NODE ,"g");
	c->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "g" ,wxString::Format("%u", m_color.Green())));
	colorNode->AddChild(c);

	c = new wxXmlNode(wxXML_ELEMENT_NODE ,"b");
	c->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "b" ,wxString::Format("%u", m_color.Blue())));
	colorNode->AddChild(c);

	c = new wxXmlNode(wxXML_ELEMENT_NODE ,"a");
	c->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "a" ,wxString::Format("%u", m_color.Alpha())));
	colorNode->AddChild(c);
}