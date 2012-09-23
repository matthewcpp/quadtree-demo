#ifndef RQT_DIALOG_HPP
#define RQT_DIALOG_HPP

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/valnum.h>

#include "rqtTypes.hpp"

class rqtRectDialog{
public:

	rRectangle2 GetArea() const;

protected:
	wxStaticBoxSizer* CreateRectInputSizer(wxWindow *parent, 	wxTextCtrl*& xBox, wxTextCtrl*& yBox, wxTextCtrl*& widthBox, wxTextCtrl*& heightBox);

protected:
	wxTextCtrl* m_xBox;
	wxTextCtrl* m_yBox;
	wxTextCtrl* m_widthBox;
	wxTextCtrl* m_heightBox;
};

//---------------------------------------------------------------

class rqtPtDialog{
public:
	rVector2 GetPoint() const;

protected:

	static wxStaticBoxSizer* CreatePointInputSizer(wxWindow* parent , wxTextCtrl*& xBox, wxTextCtrl*& yBox);

protected:

	wxTextCtrl* m_xBox;
	wxTextCtrl* m_yBox;
};

//---------------------------------------------------------------

class rqtCirDialog{
public:
	rCircle2 GetCircle() const;

protected:

	static wxStaticBoxSizer* CreateCircleInputSizer(wxWindow* parent , wxTextCtrl*& xBox, wxTextCtrl*& yBox,wxTextCtrl*& radiusBox);

protected:

	wxTextCtrl* m_xBox;
	wxTextCtrl* m_yBox;
	wxTextCtrl* m_radiusBox;
};

//---------------------------------------------------------------
class rqtItemDialog{
public:
	wxString GetItemName() const;
	wxColor GetItemColor() const;

protected:
	static wxStaticBoxSizer* CreateItemInputSizer(wxWindow* parent , wxTextCtrl*& nameBox , wxColourPickerCtrl*& colorPicker);

protected:
	wxTextCtrl* m_nameBox;
	wxColourPickerCtrl* m_colorPicker;
};

//---------------------------------------------------------------
class rqtPointDialog : public wxDialog, public rqtPtDialog, public rqtItemDialog{
public:
	rqtPointDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitPointDialog();
};

//---------------------------------------------------------------
class rqtRectangleDialog : public wxDialog, public rqtRectDialog, public rqtItemDialog{
public:
	rqtRectangleDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitRectangleDialog();
};
//---------------------------------------------------------------

class rqtCircleDialog : public wxDialog , public rqtCirDialog, public rqtItemDialog{
public:
	rqtCircleDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitCircleDialog();
};

//---------------------------------------------------------------
class rqtInitTreeDialog : public wxDialog ,public rqtRectDialog{
public:
	rqtInitTreeDialog(wxWindow *parent, wxWindowID id, const wxString &title);

	int GetDepth();

private:
	void InitTreeDialog();

private:
	wxTextCtrl* m_depthBox;
};

//---------------------------------------------------------------

class rqtQueryRectangleDialog : public wxDialog , public rqtRectDialog{
public:
	rqtQueryRectangleDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitQueryRectDialog();
};

//---------------------------------------------------------------

class rqtQueryPointDialog : public wxDialog , public rqtPtDialog{
public:
	rqtQueryPointDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitQueryPointDialog();
};

//---------------------------------------------------------------

class rqtQueryCircleDialog : public wxDialog , public rqtCirDialog{
public:
	rqtQueryCircleDialog(wxWindow *parent, wxWindowID id, const wxString &title);

private:
	void InitQueryCircleDialog();
};
#endif