#include "rqtItemQueryTool.hpp"

rqtItemQueryTool::rqtItemQueryTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent)
:rwxShapeTool ("Query Tool")
{
	SetShapeMode(rwxSHAPE_TOOL_POINT);
	m_controller = controller;

	InitItemSelectToolbar(toolbarParent);
}

bool rqtItemQueryTool::OnLeftUp(wxMouseEvent& event, const rViewport& viewport){
	rwxShapeTool::OnLeftUp(event,viewport);

	rMatrix3 view = viewport.MatrixTransform2D();
	view.Invert();

	if (m_mode == rwxSHAPE_TOOL_POINT){
		rVector2 p = GetCurrentPoint();
		view.TransformVector2(p);
		m_controller->QueryPoint(p);
	}

	else if (m_mode == rwxSHAPE_TOOL_RECTANGLE){
		rRectangle2 r = GetCurrentRectangle();
		rMath::Matrix3TransformRectangle(view, r);
		m_controller->QueryRectangle(r);
	}

	else if (m_mode == rwxSHAPE_TOOL_CIRCLE){
		rCircle2 c = GetCurrentCircle();
		rMath::Matrix3TransformCircle(view, c);
		m_controller->QueryCircle(c);
	}

	return true;
}

void rqtItemQueryTool::OnShapeToolChange(wxCommandEvent& event){
	m_mode = (rwxShapeToolMode)event.GetId();
}

void rqtItemQueryTool::InitItemSelectToolbar( wxWindow* toolbarParent){
	m_toolbar = new wxAuiToolBar(toolbarParent , wxID_ANY);
	m_toolbar->AddControl(new wxStaticText(m_toolbar, wxID_ANY , "Query Tool"));
	m_toolbar->AddSeparator();
	m_toolbar->AddTool(rwxSHAPE_TOOL_POINT, wxT("Point"), wxBitmap("content/point22.png", wxBITMAP_TYPE_PNG), "Query Using Point" , wxITEM_RADIO);
	m_toolbar->AddTool(rwxSHAPE_TOOL_RECTANGLE, wxT("Rectangle"), wxBitmap("content/rect22.png", wxBITMAP_TYPE_PNG), "Query Using Rectangle" , wxITEM_RADIO);
	m_toolbar->AddTool(rwxSHAPE_TOOL_CIRCLE, wxT("Circle"), wxBitmap("content/circle22.png", wxBITMAP_TYPE_PNG), "Query Using Circle" , wxITEM_RADIO);
	m_toolbar->ToggleTool(m_mode, true);
	m_toolbar->Realize();

	m_toolbar->Bind(wxEVT_COMMAND_MENU_SELECTED, &rqtItemQueryTool::OnShapeToolChange, this);

	m_toolbar->Hide();
}