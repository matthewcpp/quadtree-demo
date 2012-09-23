#ifndef RQT_SHAPEBASE_HPP
#define RQT_SHAPEBASE_HPP

#include <wx/wx.h>
#include <wx/xml/xml.h>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "rqtTypes.hpp"
#include "recondite.hpp"

#include "rqtGameWorldBase.hpp"

class rqtGameWorldBase;

class rqtShape : public rDrawable{
public:
	rqtShape(const wxString& name, const wxColor& color);
	virtual ~rqtShape(){}

	virtual wxString GetType() const = 0;
	virtual wxString GetInfo() const = 0;
	virtual bool IntersectsRect(const rRectangle2& r) const = 0;
	virtual bool ContainsPoint(const rVector2& v) const = 0;

	inline wxString Name() const;
	inline void SetName(const wxString& name);
	inline wxColor Color() const;
	inline void SetColor(const wxColor& color);

	virtual bool SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world);
	virtual void SerializeShape(wxXmlNode* parent) = 0;

	virtual rVector2 GetPosition() const = 0;
	virtual void SetPosition(const rVector2& v) = 0;

protected:
	wxString m_name;
	wxColor m_color;
};

 wxString rqtShape::Name() const{
	 return m_name;
 }

 wxColor rqtShape::Color() const{
	 return m_color;
 }

 void rqtShape::SetColor(const wxColor& color){
	m_color = color;
 }

 void rqtShape::SetName(const wxString& name){
	 m_name = name;
 }

#endif