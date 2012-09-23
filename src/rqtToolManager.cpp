#include "rqtToolManager.hpp"

rqtToolManager::rqtToolManager(rqtQuadtreeControllerI* controller, wxWindow* mainFrame)
{
	AddTool(rqtTOOL_ITEM_INSERT, new rqtItemInsertTool(controller, mainFrame));
	AddTool(rqtTOOL_ITEM_QUERY, new rqtItemQueryTool(controller, mainFrame));
	AddTool(rqtTOOL_ITEM_TRANSLATE, new rqtTranslateTool(controller, mainFrame));
	AddTool(rqtTOOL_ITEM_SELECT, new rqtItemSelectTool(controller, mainFrame));
}