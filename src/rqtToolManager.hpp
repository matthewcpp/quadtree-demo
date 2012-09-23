#ifndef RQT_TOOL_MANAGER_HPP
#define RQT_TOOL_MANAGER_HPP

#include <wx/wx.h>

#include "rwxToolManager.hpp"
#include "rwxTool.hpp"
#include "rqtQuadtreeControllerI.hpp"

#include "rqtItemInsertTool.hpp"
#include "rqtItemQueryTool.hpp"
#include "rqtTranslateTool.hpp"
#include "rqtItemSelectTool.hpp"

enum rqtToolId{
	rqtTOOL_ITEM_INSERT=16000,
	rqtTOOL_ITEM_QUERY,
	rqtTOOL_ITEM_SELECT,
	rqtTOOL_ITEM_TRANSLATE
};

class rqtToolManager : public rwxToolManager{
public:

	rqtToolManager(rqtQuadtreeControllerI* controller, wxWindow* mainFrame);
};

#endif