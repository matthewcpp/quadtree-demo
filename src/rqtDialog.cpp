#include "rqtDialog.hpp"

wxStaticBoxSizer* rqtRectDialog::CreateRectInputSizer(wxWindow *parent, 	wxTextCtrl*& xBox, wxTextCtrl*& yBox, wxTextCtrl*& widthBox, wxTextCtrl*& heightBox){
	wxFloatingPointValidator<rqtFloat> vf;

	wxStaticBoxSizer* areaSizer = new wxStaticBoxSizer(wxHORIZONTAL, parent, "Rectangle");
		wxStaticText* label = new wxStaticText(parent , wxID_ANY , "X:");
		areaSizer->Add(label,0,wxRIGHT , 5);
		xBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,vf);
		areaSizer->Add(xBox,0,wxRIGHT , 5);

		label = new wxStaticText(parent , wxID_ANY , "Y:");
		areaSizer->Add(label,0,wxRIGHT , 5);
		yBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,vf);
		areaSizer->Add(yBox,0,wxRIGHT , 5);

		vf.SetMin(1.0);

		label = new wxStaticText(parent , wxID_ANY , "Width:");
		areaSizer->Add(label,0,wxRIGHT , 5);
		widthBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,vf);
		areaSizer->Add(widthBox,0,wxRIGHT , 5);

		label = new wxStaticText(parent , wxID_ANY , "Height:");
		areaSizer->Add(label,0,wxRIGHT , 5);
		heightBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,vf);
		areaSizer->Add(heightBox,0,wxRIGHT , 5);

	return areaSizer;
}

rRectangle2 rqtRectDialog::GetArea() const{
	double x,y,w,h;

	m_xBox->GetValue().ToDouble(&x);
	m_yBox->GetValue().ToDouble(&y);
	m_widthBox->GetValue().ToDouble(&w);
	m_heightBox->GetValue().ToDouble(&h);

	return rRectangle2((rqtFloat)x,(rqtFloat)y,(rqtFloat)w,(rqtFloat)h);
}

//---------------------------------------------------------------

rVector2 rqtPtDialog::GetPoint() const{
	double valx,valy;
	m_xBox->GetValue().ToDouble(&valx);
	m_yBox->GetValue().ToDouble(&valy);

	return rVector2((rqtFloat)valx, rqtFloat(valy));
}

wxStaticBoxSizer* rqtPtDialog::CreatePointInputSizer(wxWindow* parent , wxTextCtrl*& xBox, wxTextCtrl*& yBox){
		wxFloatingPointValidator<rqtFloat> v;

		wxStaticBoxSizer* pointSizer = new wxStaticBoxSizer(wxHORIZONTAL,parent, "Point");
			wxStaticText* label = new wxStaticText(parent , wxID_ANY , "X:");
			pointSizer->Add(label,0,wxRIGHT , 5);
			xBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,v);
			pointSizer->Add(xBox,0,wxRIGHT , 5);
			label = new wxStaticText(parent , wxID_ANY , "Y:");
			pointSizer->Add(label,0,wxRIGHT , 5);
			yBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,v);
			pointSizer->Add(yBox,0,wxRIGHT , 5);

		return pointSizer;
}

//---------------------------------------------------------------
wxStaticBoxSizer* rqtCirDialog::CreateCircleInputSizer(wxWindow* parent , wxTextCtrl*& xBox, wxTextCtrl*& yBox,wxTextCtrl*& radiusBox){
		wxFloatingPointValidator<rqtFloat> v;

		wxStaticBoxSizer* circleSizer = new wxStaticBoxSizer(wxHORIZONTAL,parent, "Circle");
			wxStaticText* label = new wxStaticText(parent , wxID_ANY , "X:");
			circleSizer->Add(label,0,wxRIGHT , 5);
			xBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,v);
			circleSizer->Add(xBox,0,wxRIGHT , 5);
			label = new wxStaticText(parent , wxID_ANY , "Y:");
			circleSizer->Add(label,0,wxRIGHT , 5);
			yBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,v);
			circleSizer->Add(yBox,0,wxRIGHT , 5);

			label = new wxStaticText(parent , wxID_ANY , "Radius:");
			circleSizer->Add(label,0,wxRIGHT , 5);
			radiusBox = new wxTextCtrl(parent ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,v);
			circleSizer->Add(radiusBox,0,wxRIGHT , 5);

		return circleSizer;
}

rCircle2 rqtCirDialog::GetCircle() const{
	double valx,valy, valrad;
	m_xBox->GetValue().ToDouble(&valx);
	m_yBox->GetValue().ToDouble(&valy);
	m_radiusBox->GetValue().ToDouble(&valrad);

	return rCircle2(valx , valy, valrad);
}

//---------------------------------------------------------------

wxString rqtItemDialog::GetItemName() const{
	return m_nameBox->GetValue();
}

wxColor rqtItemDialog::GetItemColor() const{
	return m_colorPicker->GetColour();
}

wxStaticBoxSizer* rqtItemDialog::CreateItemInputSizer(wxWindow* parent , wxTextCtrl*& nameBox , wxColourPickerCtrl*& colorPicker){
	wxStaticBoxSizer* itemSizer = new wxStaticBoxSizer(wxHORIZONTAL,parent, "Item");
		wxStaticText* label = new wxStaticText(parent , wxID_ANY , "Name:");
		itemSizer->Add(label,0,wxRIGHT , 5);
		nameBox = new wxTextCtrl(parent ,wxID_ANY);
		itemSizer->Add(nameBox,0,wxRIGHT , 5);

		label = new wxStaticText(parent , wxID_ANY , "Color:");
		itemSizer->Add(label,0,wxRIGHT , 5);
		colorPicker = new wxColourPickerCtrl(parent , wxID_ANY,*wxWHITE);
		itemSizer->Add(colorPicker);

	return itemSizer;
}

//---------------------------------------------------------------

rqtPointDialog::rqtPointDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent , id , title)
{
	InitPointDialog();
}

void rqtPointDialog::InitPointDialog(){
	
	wxStaticBoxSizer* itemSizer = CreateItemInputSizer(this , m_nameBox , m_colorPicker);
	wxStaticBoxSizer* pointSizer = CreatePointInputSizer(this , m_xBox , m_yBox);



	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(itemSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(pointSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}
//---------------------------------------------------------------

rqtRectangleDialog::rqtRectangleDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent , id , title)
{
	InitRectangleDialog();
}

void rqtRectangleDialog::InitRectangleDialog(){
	wxStaticBoxSizer* itemSizer = CreateItemInputSizer(this , m_nameBox , m_colorPicker);
	wxBoxSizer* rectSizer = CreateRectInputSizer(this , m_xBox , m_yBox , m_widthBox , m_heightBox);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(itemSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(rectSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}
//---------------------------------------------------------------
rqtCircleDialog::rqtCircleDialog(wxWindow *parent, wxWindowID id, const wxString &title)
:wxDialog(parent , id , title)
{
	InitCircleDialog();
}

void rqtCircleDialog::InitCircleDialog(){
	wxStaticBoxSizer* itemSizer = CreateItemInputSizer(this , m_nameBox , m_colorPicker);
	wxStaticBoxSizer* circleSizer = CreateCircleInputSizer(this , m_xBox , m_yBox, m_radiusBox);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(itemSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(circleSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}

//---------------------------------------------------------------

rqtInitTreeDialog::rqtInitTreeDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent, id , title)
{
	InitTreeDialog();
}

int rqtInitTreeDialog::GetDepth(){
	long d;

	m_depthBox->GetValue().ToLong(&d);

	return d;
}


void rqtInitTreeDialog::InitTreeDialog(){
	wxIntegerValidator<unsigned int> vi;
	vi.SetMin(1);
	vi.SetMax(100);

	wxBoxSizer* rectSizer = CreateRectInputSizer(this , m_xBox , m_yBox , m_widthBox , m_heightBox);

	wxBoxSizer* depthSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* label = new wxStaticText(this , wxID_ANY , "Max Depth:");
		depthSizer->Add(label,0,wxRIGHT , 5);
		m_depthBox = new wxTextCtrl(this ,wxID_ANY, wxEmptyString,wxDefaultPosition,wxDefaultSize,0,vi);
		depthSizer->Add(m_depthBox,0,wxRIGHT , 5);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(rectSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(depthSizer ,0 ,  wxALL|wxEXPAND, 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);
	
	SetSizerAndFit(mainSizer);
}

//---------------------------------------------------------------

rqtQueryRectangleDialog::rqtQueryRectangleDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent, id , title)
{
	InitQueryRectDialog();
}

void rqtQueryRectangleDialog::InitQueryRectDialog(){
	wxStaticText* prompt = new wxStaticText(this ,wxID_ANY , "Enter Query Dimensions:");
	wxBoxSizer* rectSizer = CreateRectInputSizer(this , m_xBox , m_yBox , m_widthBox , m_heightBox);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(prompt , 0 , wxALL , 5);
	mainSizer->Add(rectSizer , 0 , wxALL , 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}

//---------------------------------------------------------------

rqtQueryPointDialog::rqtQueryPointDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent, id , title)
{
	InitQueryPointDialog();
}

void rqtQueryPointDialog::InitQueryPointDialog(){
	wxStaticText* prompt = new wxStaticText(this ,wxID_ANY , "Enter Query Dimensions:");
	wxStaticBoxSizer* pointSizer = CreatePointInputSizer(this , m_xBox , m_yBox);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(prompt , 0 , wxALL , 5);
	mainSizer->Add(pointSizer , 0 , wxALL , 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}

//---------------------------------------------------------------

rqtQueryCircleDialog::rqtQueryCircleDialog(wxWindow *parent, wxWindowID id, const wxString &title)
	:wxDialog(parent, id , title)
{
	InitQueryCircleDialog();
}

void rqtQueryCircleDialog::InitQueryCircleDialog(){
	wxStaticText* prompt = new wxStaticText(this ,wxID_ANY , "Enter Query Dimensions:");
	wxStaticBoxSizer* circleSizer = CreateCircleInputSizer(this , m_xBox , m_yBox, m_radiusBox);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(prompt , 0 , wxALL , 5);
	mainSizer->Add(circleSizer , 0 , wxALL , 5);
	mainSizer->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL|wxNO_DEFAULT),0, wxALL|wxEXPAND, 5);

	SetSizerAndFit(mainSizer);
}