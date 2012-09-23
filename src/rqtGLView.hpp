#ifndef RQT_GLVIEW_HPP
#define RQT_GLVIEW_HPP

#include "recondite.hpp"

#include "rqtTypes.hpp"

#include <wx/wx.h>
#include <wx/glcanvas.h>


#include "rqtGameWorld.hpp"
#include "rqtToolManager.hpp"


class rqtGLView : public wxGLCanvas{
public:
	rqtGLView(rqtGameWorld* gameWorld, rRenderer* renderer , rOpenGLGraphicsDevice* graphicsDevice , wxStatusBar* viewStatus , wxWindow *parent, wxWindowID id=wxID_ANY, const int *attribList=NULL, const wxPoint &pos=wxDefaultPosition,
		const wxSize &size=wxDefaultSize, long style=0, const wxString &name="GLCanvas", const wxPalette &palette=wxNullPalette);

	~rqtGLView();

	void SetRenderMode(rRenderMode mode);
	void ShouldRenderQuadtree(bool render);
	void ShouldRenderQuery(bool render);
	void ResetView();

	void SetToolManager(rqtToolManager* manager);

	static const float rqtMIN_ZOOM;
	static const float rqtMAX_ZOOM;
	static const float rqtZOOM_STEP;

	void IncrementWorldScale(float amount);

protected:
	void DrawScene();
	void DrawGameWorld();
	void DrawQuadtree();
	void DrawPreviousQuery();
	void DrawTool();
	void UpdateViewStatus();

	void OnPaint(wxPaintEvent& event);

	void OnLeftDown(wxMouseEvent& event);
	void OnRightDown(wxMouseEvent& event);

	void OnLeftUp(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);

	void OnMousewheel(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()

protected:
	rOpenGLGraphicsDevice* m_graphicsDevice;
	rRenderer* m_renderer;

	rViewport m_viewport;
	rqtGameWorld* m_gameWorld;
	rqtToolManager* m_toolManager;

	rRenderMode m_renderMode;
	bool m_renderQuadtree;
	bool m_renderQuery;

	wxPoint m_mousePrev;
	wxStatusBar* m_viewStatus;

	size_t m_nodesRendered;
	size_t m_itemsRendered;

	wxColour m_quadtreeColor;

protected:
	static void InitSharedContext(wxGLCanvas* glCanvas);
	static wxGLContext* sharedContext;
};

#endif