#ifndef RQT_SHAPES_HPP
#define RQT_SHAPES_HPP

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "recondite.hpp"

#include "rqtTypes.hpp"
#include "rqtShapeBase.hpp"




//------------------------------------------------------------

 class rqtPoint : public rqtShape{
 public:
	 rqtPoint(const wxString& name, const wxColor& color, const rVector2& p);

	virtual void Draw(rRendererI* renderer) const;
	inline rVector2 Point() const;

	virtual wxString GetType() const;
	virtual wxString GetInfo() const;
	virtual bool IntersectsRect(const rRectangle2& r) const;
	virtual bool ContainsPoint(const rVector2& v) const;

	virtual void SerializeShape(wxXmlNode* parent);
	virtual bool SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world);

	virtual rVector2 GetPosition() const;
	virtual void SetPosition(const rVector2& v);

 private:
	 rVector2 m_point;
 };

 rVector2 rqtPoint::Point() const{
	 return m_point;
 }

 //------------------------------------------------------------

 class rqtRectangle : public rqtShape{
 public:
	 rqtRectangle(const wxString& name, const wxColor& color, const rRectangle2& r);

	virtual void Draw(rRendererI* renderer) const;
	inline rRectangle2 Rectangle() const;

	virtual wxString GetType() const;
	virtual wxString GetInfo() const;
	virtual bool IntersectsRect(const rRectangle2& r) const;
	virtual bool ContainsPoint(const rVector2& v) const;

	virtual void SerializeShape(wxXmlNode* parent);
	virtual bool SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world);

	virtual rVector2 GetPosition() const;
	virtual void SetPosition(const rVector2& v);

 private:
	 rRectangle2 m_rectangle;
 };

 rRectangle2 rqtRectangle::Rectangle() const{
	 return m_rectangle;
 }

  //------------------------------------------------------------

class rqtCircle : public rqtShape{
public:
	rqtCircle(const wxString& name, const wxColor& color, const rCircle2& c);

	virtual void Draw(rRendererI* renderer) const;
	inline rCircle2 Circle() const;

	virtual wxString GetType() const;
	virtual wxString GetInfo() const;
	virtual bool IntersectsRect(const rRectangle2& r) const;
	virtual bool ContainsPoint(const rVector2& v) const;

	virtual void SerializeShape(wxXmlNode* parent);
	virtual bool SetProperty(const wxString& key , wxAny& value, rqtGameWorldBase* world);

	virtual rVector2 GetPosition() const;
	virtual void SetPosition(const rVector2& v);

 private:
	rCircle2 m_circle;
 };

 rCircle2 rqtCircle::Circle() const{
	 return m_circle;
 }

#endif