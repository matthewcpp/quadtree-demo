#ifndef RQT_ITEMINSERTTOOL_HPP
#define RQT_ITEMINSERTTOOL_HPP

#include <wx/wx.h>
#include <wx/clrpicker.h>

#include "rwxShapeTool.hpp"
#include "rqtQuadtreeControllerI.hpp"



class rqtItemInsertTool : public rwxShapeTool{
public:
	rqtItemInsertTool(rqtQuadtreeControllerI* controller, wxWindow* toolbarParent);

	virtual bool OnLeftUp(wxMouseEvent& event, const rViewport& viewport);

	void OnColorChange(wxColourPickerEvent& event);
	void OnShapeToolChange(wxCommandEvent& event);

private:

	void InitItemInsertToolbar(wxWindow* toolbarParent);

private:
	rqtQuadtreeControllerI* m_controller;
};

#endif