#ifndef RQT_ITEMSELECTTOOL_HPP
#define RQT_ITEMSELECTTOOL_HPP

#include <wx/wx.h>

#include "rqtTypes.hpp"
#include "rwxTool.hpp"
#include "rqtQuadtreeControllerI.hpp"

class rqtItemSelectTool : public rwxTool{
public:
	rqtItemSelectTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent);

	virtual bool OnLeftDown(wxMouseEvent& event, const rViewport& viewport);
	virtual bool OnMouseMotion(wxMouseEvent& event, const rViewport& viewport);
	virtual bool OnLeftUp(wxMouseEvent& event, const rViewport& viewport);
	virtual bool OnViewportRender(rRendererI* renderer, const rViewport& viewport);

private:
	void InitItemSelectTool(wxWindow* toolbarParent);

private:
	rqtQuadtreeControllerI* m_controller;
};

#endif