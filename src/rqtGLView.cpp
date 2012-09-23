#include "rqtGLView.hpp"

wxGLContext* rqtGLView::sharedContext = NULL;
const float rqtGLView::rqtMIN_ZOOM = 0.1f;
const float rqtGLView::rqtMAX_ZOOM = 2.0f;
const float rqtGLView::rqtZOOM_STEP = 0.05f;

BEGIN_EVENT_TABLE(rqtGLView , wxGLCanvas)
	EVT_PAINT (rqtGLView::OnPaint)

	EVT_LEFT_DOWN(rqtGLView::OnLeftDown)
	EVT_RIGHT_DOWN(rqtGLView::OnRightDown)
	EVT_LEFT_UP(rqtGLView::OnLeftUp)
	EVT_MOTION(rqtGLView::OnMotion)
	EVT_MOUSEWHEEL(rqtGLView::OnMousewheel)

END_EVENT_TABLE()

rqtGLView::rqtGLView(rqtGameWorld* gameWorld, rRenderer* renderer ,rOpenGLGraphicsDevice* graphicsDevice ,wxStatusBar* viewStatus ,wxWindow *parent, wxWindowID id, const int *attribList, const wxPoint &pos, const wxSize &size, 
	long style, const wxString &name, const wxPalette &palette)
	:wxGLCanvas(parent , id, attribList, pos ,size , style , name , palette)
{
	m_graphicsDevice =graphicsDevice;
	m_renderer = renderer;
	m_gameWorld = gameWorld;
	m_toolManager = NULL;
	m_viewStatus = viewStatus;

	m_viewport.m_type = rVIEWPORT_2D;
	m_renderMode = rRENDER_SHADED;
	m_renderQuadtree = true;
	m_renderQuery = true;
	m_nodesRendered = 0;
	m_itemsRendered = 0;

	m_quadtreeColor = wxColour(127,127,127 , 255);
}

void rqtGLView::SetToolManager(rqtToolManager* manager){
	m_toolManager = manager;
}

rqtGLView::~rqtGLView(){
}

void rqtGLView::SetRenderMode(rRenderMode mode){
	if (mode != m_renderMode){
		m_renderMode = mode;
		Refresh();
	}
}

void rqtGLView::ShouldRenderQuadtree(bool render){
	if (render != m_renderQuadtree){
		m_renderQuadtree = render;
		Refresh();
	}
}

void rqtGLView::InitSharedContext(wxGLCanvas* glCanvas){
	if (sharedContext)
		return;

	sharedContext = new wxGLContext(glCanvas);

}

void rqtGLView::OnPaint(wxPaintEvent& event){
	wxPaintDC(this);
	DrawScene();
}

void rqtGLView::DrawGameWorld(){
	rRectangle2 viewArea = m_viewport.Get2DScreenRect();
	rqtShapeResult result;

	m_gameWorld->GetDrawablesInRect(viewArea, result);

	rqtShape* shape;
	for (rqtShapeTreeItr it = result.begin(); it != result.end(); ++it){
		shape = *it;

		m_renderer->Render(shape , m_renderMode);

		if (m_gameWorld->ItemIsSelected(shape)){
			rColor c = shape->Color();
			shape->SetColor(*wxCYAN);
			m_renderer->Render(shape , rRENDER_WIRE);
			shape->SetColor(c);
		}
			
	}

	m_itemsRendered = result.size();
}

void rqtGLView::DrawQuadtree(){
	rRectangle2 viewArea = m_viewport.Get2DScreenRect();
	rqtShapeNodeList nodes, selected;
	rqtShape* lastQuery = m_gameWorld->GetLastQuery();

	m_gameWorld->GetDrawableNodes(viewArea, nodes);
	m_nodesRendered = nodes.size();
	m_renderer->SetRenderMode(rRENDER_WIRE);
	wxColour gray(150,150,150 , 255);

	for (size_t i = 0; i < nodes.size(); i++){
		if (lastQuery && lastQuery->IntersectsRect(nodes[i]->m_region))
			selected.push_back(nodes[i]);
		else 
			m_renderer->DrawRectangle(nodes[i]->m_region , m_quadtreeColor);
	}

	for (size_t i = 0; i < selected.size(); i++)
			m_renderer->DrawRectangle(selected[i]->m_region , *wxYELLOW);
}

void rqtGLView::DrawScene(){
	if (!sharedContext){
		InitSharedContext(this);
		SetCurrent(*sharedContext);
		m_graphicsDevice->Init();
	}
	
	SetCurrent(*sharedContext);
	
	int width,height;
	GetSize(&width,&height);
	m_viewport.SetWindowSize(width,height);

	m_graphicsDevice->Clear();
	m_graphicsDevice->SetActiveViewport(m_viewport);
		
	if (m_gameWorld->IsInit()){
		m_renderer->Begin();
			DrawQuadtree();
			DrawGameWorld();

			DrawPreviousQuery();
			DrawTool();
		m_renderer->End();

		m_renderer->ExecuteCommands();
		UpdateViewStatus();
	}

	SwapBuffers();


}

void rqtGLView::DrawTool(){
	if (m_gameWorld->IsInit() && m_toolManager)
		m_toolManager->OnViewportRender(m_renderer, m_viewport);
}

void rqtGLView::DrawPreviousQuery(){
	if (!m_renderQuery)
		return;

	rqtShape* query = m_gameWorld->GetLastQuery();

	if (query)
		m_renderer->Render(query , rRENDER_WIRE);
}

void rqtGLView::OnLeftDown(wxMouseEvent& event){
	SetFocus();
	if (m_gameWorld->IsInit() && m_toolManager){
		if (m_toolManager->OnLeftDown(event, m_viewport))
			Refresh();
	}
}

void rqtGLView::OnLeftUp(wxMouseEvent& event){
	if (m_gameWorld->IsInit() && m_toolManager){
		if (m_toolManager->OnLeftUp(event, m_viewport))
			Refresh();
	}
}

void rqtGLView::OnMotion(wxMouseEvent& event){
	if (!m_gameWorld->IsInit())
		return;

	if (event.RightIsDown()){
		wxPoint pos = event.GetPosition();

		wxPoint delta = pos - m_mousePrev;
		m_mousePrev = pos;
		m_viewport.IncrementWorldTranslate(delta.x , delta.y);
		Refresh();
	}
	else if (m_toolManager){
		if (m_toolManager->OnMouseMotion(event, m_viewport))
			Refresh();
	}
}

void rqtGLView::OnRightDown(wxMouseEvent& event){
	SetFocus();
	if (m_gameWorld->IsInit())
		m_mousePrev = event.GetPosition();
}

void rqtGLView::IncrementWorldScale(float amount){
	float s =  m_viewport.m_worldScale + amount;

	if (s < rqtMAX_ZOOM && s > rqtMIN_ZOOM){
		m_viewport.SetWorldScale(s);
		Refresh();
	}
}

void rqtGLView::OnMousewheel(wxMouseEvent& event){
	if (!m_gameWorld->IsInit())
		return;

	int wheelRotation = event.GetWheelRotation();

	if (wheelRotation < 0)
		IncrementWorldScale(rqtZOOM_STEP);
	else if (wheelRotation > 0)
		IncrementWorldScale(-rqtZOOM_STEP);
	
		
	
}

void rqtGLView::ShouldRenderQuery(bool render){
	m_renderQuery = render;
}

void rqtGLView::UpdateViewStatus(){
	rRectangle2 r = m_viewport.Get2DScreenRect();

	size_t numNodes = m_gameWorld->TreeSize();
	size_t numItems = m_gameWorld->NumShapes();

	wxString str = wxString::Format("view: %.2f,%.2f %.2fx%.2f", r.x , r.y , r.width ,r.height);
	
	m_viewStatus->SetStatusText(str, 0);

	str = wxString::Format("%i nodes / %i shown" , numNodes, m_nodesRendered);
	m_viewStatus->SetStatusText(str, 3);

	str = wxString::Format("%i items / %i drawn" , numItems, m_itemsRendered);
	m_viewStatus->SetStatusText(str, 2);
}

void rqtGLView::ResetView(){
	int width,height;
	GetSize(&width,&height);

	m_viewport.SetWorldTranslate(0,0);
	m_viewport.SetWorldScale(1.0f);
	Refresh();
}