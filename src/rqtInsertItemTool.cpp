#include "rqtItemInsertTool.hpp"

rqtItemInsertTool::rqtItemInsertTool(rqtQuadtreeControllerI* controller,wxWindow* toolbarParent)
	:rwxShapeTool ("Insertion Tool")
{
	SetShapeMode(rwxSHAPE_TOOL_RECTANGLE);
	m_controller = controller;
	InitItemInsertToolbar(toolbarParent);
}


bool rqtItemInsertTool::OnLeftUp(wxMouseEvent& event, const rViewport& viewport){
	rwxShapeTool::OnLeftUp(event,viewport);

	wxString name = m_controller->GetNextItemName();
	rMatrix3 view = viewport.MatrixTransform2D();
	view.Invert();

	if (m_mode == rwxSHAPE_TOOL_POINT){
		rVector2 p = GetCurrentPoint();
		view.TransformVector2(p);
		m_controller->CreatePointObject(name ,m_color , p);
	}

	else if (m_mode == rwxSHAPE_TOOL_RECTANGLE){
		rRectangle2 r = GetCurrentRectangle();

		if (r.width > 0.0 && r.height > 0.0){
			rMath::Matrix3TransformRectangle(view, r);
			m_controller->CreateRectangleObject(name , m_color , r);
		}
	}

	else if (m_mode == rwxSHAPE_TOOL_CIRCLE){
		rCircle2 c = GetCurrentCircle();

		if (c.radius > 0.0f){
			rMath::Matrix3TransformCircle(view, c);
			m_controller->CreateCircleObject(name , m_color , c);
		}
	}

	return true;
}

void rqtItemInsertTool::OnColorChange(wxColourPickerEvent& event){
	m_color = event.GetColour();
	event.Skip();
}

void rqtItemInsertTool::OnShapeToolChange(wxCommandEvent& event){
	m_mode = (rwxShapeToolMode)event.GetId();
}

void rqtItemInsertTool::InitItemInsertToolbar(wxWindow* toolbarParent){
	m_toolbar = new wxAuiToolBar(toolbarParent , wxID_ANY);
	m_toolbar->AddControl(new wxStaticText(m_toolbar, wxID_ANY , "Insertion Tool"));
	m_toolbar->AddSeparator();
	m_toolbar->AddControl(new wxColourPickerCtrl(m_toolbar , wxID_ANY, *wxWHITE) , "Color");

	m_toolbar->AddTool(rwxSHAPE_TOOL_POINT, wxT("Point"), wxBitmap("content/point22.png", wxBITMAP_TYPE_PNG), "Insert Point" , wxITEM_RADIO);
	m_toolbar->AddTool(rwxSHAPE_TOOL_RECTANGLE, wxT("Rectangle"), wxBitmap("content/rect22.png", wxBITMAP_TYPE_PNG), "Insert Rectangle" , wxITEM_RADIO);
	m_toolbar->AddTool(rwxSHAPE_TOOL_CIRCLE, wxT("Circle"), wxBitmap("content/circle22.png", wxBITMAP_TYPE_PNG), "Insert Circle" , wxITEM_RADIO);
	m_toolbar->ToggleTool(m_mode, true);
	m_toolbar->Realize();

	m_toolbar->Bind(wxEVT_COMMAND_COLOURPICKER_CHANGED, &rqtItemInsertTool::OnColorChange, this);
	m_toolbar->Bind(wxEVT_COMMAND_MENU_SELECTED, &rqtItemInsertTool::OnShapeToolChange, this);

	m_toolbar->Hide();
}