#ifndef RQT_ITEMQUERYTOOL_HPP
#define RQT_ITEMQUERYTOOL_HPP

#include <wx/wx.h>
#include "rwxShapeTool.hpp"
#include "rqtQuadtreeControllerI.hpp"

class rqtItemQueryTool : public rwxShapeTool{
public:
	rqtItemQueryTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent);

	virtual bool OnLeftUp(wxMouseEvent& event, const rViewport& viewport);

	void OnShapeToolChange(wxCommandEvent& event);

private:
	void InitItemSelectToolbar(wxWindow* toolbarParent);

private:
	rqtQuadtreeControllerI* m_controller;

};

#endif