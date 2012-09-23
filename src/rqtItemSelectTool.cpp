#include "rqtItemSelectTool.hpp"

rqtItemSelectTool::rqtItemSelectTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent)
	:rwxTool("Selection Tool")
{
	m_controller = controller;

	InitItemSelectTool(toolbarParent);
}

void rqtItemSelectTool::InitItemSelectTool(wxWindow* toolbarParent){
	m_toolbar = new wxAuiToolBar(toolbarParent , wxID_ANY);
	m_toolbar->AddControl(new wxStaticText(m_toolbar, wxID_ANY , "Selection Tool"));
	m_toolbar->Realize();
	m_toolbar->Hide();
}

bool rqtItemSelectTool::OnLeftDown(wxMouseEvent& event, const rViewport& viewport){
	wxPoint mousePos = event.GetPosition();
	rMatrix3 view = viewport.MatrixTransform2D();
	view.Invert();
	rVector2 p(mousePos.x , mousePos.y);
	view.TransformVector2(p);

	if (m_controller->SelectItemByPoint(p))
		return true;

	return false;
}

bool rqtItemSelectTool::OnMouseMotion(wxMouseEvent& event, const rViewport& viewport){
	return false;
}

bool rqtItemSelectTool::OnLeftUp(wxMouseEvent& event, const rViewport& viewport){
	return false;
}

bool rqtItemSelectTool::OnViewportRender(rRendererI* renderer, const rViewport& viewport){
	return false;
}