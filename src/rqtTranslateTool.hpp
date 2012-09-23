#ifndef RQT_TRANSLATE_TOOL_HPP
#define RQT_TRANSLATE_TOOL_HPP

#include "rwxTool.hpp"
#include "rqtShapes.hpp"
#include "rqtQuadtreeControllerI.hpp"

#include "recondite.hpp"

enum rqtTranslateDirection{
	rqtTRANSLATE_NONE = 0, 
	rqtTRANSLATE_X , 
	rqtTRANSLATE_Y,
	rqtTRANSLATE_XY
};

struct rqtTranslateGizmo{
	rLineSegment2 m_xLine;
	rTriangle2 m_xHandle;

	rLineSegment2 m_yLine;
	rTriangle2 m_yHandle;

	rCircle2 freeCircle;
};

class rqtTranslateTool : public rwxTool{
public:

	rqtTranslateTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent);

	virtual bool OnLeftDown(wxMouseEvent& event, const rViewport& viewport) ;
	virtual bool OnMouseMotion(wxMouseEvent& event, const rViewport& viewport) ;
	virtual bool OnLeftUp(wxMouseEvent& event, const rViewport& viewport) ;
	virtual bool OnViewportRender(rRendererI* renderer, const rViewport& viewport) ;


private:

	rVector2 GetWorldPositionPoint(wxMouseEvent& event, const rViewport& viewport);
	void InitTranslateToolbar(wxWindow* toolbarParent);
	
	rqtTranslateDirection CheckGizmoHandleGrab(const rVector2& p);
	void UpdateGizmo(const rViewport& viewport);

	void DrawGizmo();

	rqtShape* m_shape;
	rqtQuadtreeControllerI* m_controller;
	rqtTranslateDirection m_translate;

	rqtTranslateGizmo m_gizmo;
	rVector2 m_previous;

	wxAuiToolBar* m_toolBar;
};

#endif