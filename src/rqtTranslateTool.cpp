#include "rqtTranslateTool.hpp"

rqtTranslateTool::rqtTranslateTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent)
	:rwxTool ("Translate Tool")
{
	m_controller = controller;
	m_shape = NULL;
	m_translate = rqtTRANSLATE_NONE;

	InitTranslateToolbar(toolbarParent);
}

rVector2 rqtTranslateTool::GetWorldPositionPoint(wxMouseEvent& event, const rViewport& viewport){
	wxPoint pos = event.GetPosition();
	rVector2 p(pos.x , pos.y);

	rMatrix3 mat = viewport.MatrixTransform2D();
	mat.Invert();
	return mat.GetTransformedVector2(p);
}

bool rqtTranslateTool::OnLeftDown(wxMouseEvent& event, const rViewport& viewport){
	m_shape = m_controller->GetSelectedItem();

	if (!m_shape)
		return false;

	UpdateGizmo(viewport);
	m_previous = GetWorldPositionPoint(event , viewport);

	m_translate = CheckGizmoHandleGrab(m_previous);

	return true;
}

bool rqtTranslateTool::OnMouseMotion(wxMouseEvent& event, const rViewport& viewport){
	if (!m_shape || m_translate == rqtTRANSLATE_NONE)
		return false;

	UpdateGizmo(viewport);
	rVector2 current = GetWorldPositionPoint(event , viewport);
	rVector2 delta = current - m_previous;
	rVector2 shapePos = m_shape->GetPosition();
	rVector2 targetPos = shapePos;

	if (m_translate == rqtTRANSLATE_X)
		targetPos.x += delta.x;
	else if (m_translate == rqtTRANSLATE_Y)
		targetPos.y += delta.y;
	else
		targetPos += delta;

	m_shape->SetPosition(targetPos);
	if (!m_controller->ShapeFitsInWorld(m_shape))
		m_shape->SetPosition(shapePos);

	m_previous = current;
	return true;
}

bool rqtTranslateTool::OnLeftUp(wxMouseEvent& event, const rViewport& viewport){
	if (!m_shape || !m_translate)
		return false;

	m_controller->UpdateObjectSelection();

	m_shape = NULL;
	m_translate = rqtTRANSLATE_NONE;
	return true;
}

bool rqtTranslateTool::OnViewportRender(rRendererI* renderer, const rViewport& viewport){
	m_shape = m_controller->GetSelectedItem();

	if (!m_shape)
		return false;

	UpdateGizmo(viewport);

	rFloatArray lines;
	lines.resize(4);
	lines[0] = m_gizmo.m_xLine.s1.x;
	lines[1] = m_gizmo.m_xLine.s1.y;
	lines[2] = m_gizmo.m_xLine.s2.x;
	lines[3] = m_gizmo.m_xLine.s2.y;
	renderer->DrawLines(lines, *wxBLUE);
	renderer->DrawTriangle(m_gizmo.m_xHandle, *wxBLUE, rRENDER_SHADED);

	lines[0] = m_gizmo.m_yLine.s1.x;
	lines[1] = m_gizmo.m_yLine.s1.y;
	lines[2] = m_gizmo.m_yLine.s2.x;
	lines[3] = m_gizmo.m_yLine.s2.y;
	renderer->DrawLines(lines, *wxRED);
	renderer->DrawTriangle(m_gizmo.m_yHandle, *wxRED, rRENDER_SHADED);

	renderer->DrawCircle(m_gizmo.freeCircle , *wxYELLOW, rRENDER_SHADED);

	return true;
}

rqtTranslateDirection rqtTranslateTool::CheckGizmoHandleGrab(const rVector2& p){
	if (m_gizmo.m_xHandle.ContainsPoint(p))
		return rqtTRANSLATE_X;
	else if (m_gizmo.m_yHandle.ContainsPoint(p))
		return rqtTRANSLATE_Y;
	else if (m_gizmo.freeCircle.ContainsPoint(p))
		return rqtTRANSLATE_XY;

	return rqtTRANSLATE_NONE;
}

void rqtTranslateTool::UpdateGizmo(const rViewport& viewport){
	rVector2 p = m_shape->GetPosition();

	m_gizmo.m_yLine.s1 = p;
	m_gizmo.m_yLine.s2.Set(p.x , p.y - 20.0f);

	m_gizmo.m_yHandle.p0.Set(p.x - 5.0f , m_gizmo.m_yLine.s2.y);
	m_gizmo.m_yHandle.p1.Set(p.x , m_gizmo.m_yLine.s2.y - 10.0f);
	m_gizmo.m_yHandle.p2.Set(p.x + 5.0f , m_gizmo.m_yLine.s2.y);

	m_gizmo.m_xLine.s1 = p;
	m_gizmo.m_xLine.s2.Set(p.x + 20.0f, p.y );

	m_gizmo.m_xHandle.p0.Set(m_gizmo.m_xLine.s2.x , p.y -5.0f);
	m_gizmo.m_xHandle.p1.Set(m_gizmo.m_xLine.s2.x +10.0f, p.y );
	m_gizmo.m_xHandle.p2.Set(m_gizmo.m_xLine.s2.x ,  p.y + 5.0f);

	m_gizmo.freeCircle.center = p;
	m_gizmo.freeCircle.radius = 3.0f;
}
void rqtTranslateTool::InitTranslateToolbar(wxWindow* toolbarParent){
	m_toolbar = new wxAuiToolBar(toolbarParent , wxID_ANY);
	m_toolbar->AddControl(new wxStaticText(m_toolbar, wxID_ANY , "Translate Tool"));
	m_toolbar->Realize();
	m_toolbar->Hide();
}
