#ifndef RQT_MAINFRAME_HPP
#define RQT_MAINFRAME_HPP

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/listctrl.h>
#include <wx/aboutdlg.h>

#include "recondite.hpp"

#include "rqtGameWorld.hpp"
#include "rqtGLView.hpp"
#include "rqtQuadtreeController.hpp"
#include "rqtDialog.hpp"
#include "rqtQueryViewWindow.hpp"
#include "rqtToolManager.hpp"
#include "rwxObjectBrowser.hpp"
#include "rqtShapePropertyViewer.hpp"

enum rqtControlId{
	rqtTOOL_PALETTE=11111,
	rqtGL_VIEW,
	rqtOBJ_BROWSER,

	rqtINIT_TREE,
	rqtCLEAR_TREE,
	rqtTRIM_TREE,

	rqtINSERT_POINT,
	rqtINSERT_RECT,
	rqtINSERT_CIRCLE,

	rqtQUERY_POINT,
	rqtQUERY_RECT,
	rqtQUERY_CIRCLE,

	rqtVIEW_WIRE,
	rqtVIEW_SHADED,
	rqtVIEW_LAST_QUERY,
	rqtVIEW_RESET,
	rqtVIEW_ZOOM_IN,
	rqtVIEW_ZOOM_OUT
};

class rqtMainFrame : public wxFrame{
public:
	rqtMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos=wxDefaultPosition, 
				const wxSize &size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE, const wxString &name=wxFrameNameStr);

	~rqtMainFrame();

private:

	void OnSave(wxCommandEvent& event);
	void OnLoad(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnFrameClose(wxCloseEvent& event);

	void OnInitTree(wxCommandEvent& event);
	void OnClearTree(wxCommandEvent& event);
	void OnTrimTree(wxCommandEvent& event);

	void OnInsertPoint(wxCommandEvent& event);
	void OnInsertRectangle(wxCommandEvent& event);
	void OnInsertCircle(wxCommandEvent& event);

	void OnQueryPoint(wxCommandEvent& event);
	void OnQueryRectangle(wxCommandEvent& event);
	void OnQueryCircle(wxCommandEvent& event);

	void OnViewRenderMode(wxCommandEvent& event);
	void OnViewLastQuery(wxCommandEvent& event);
	void OnViewReset(wxCommandEvent& event);
	void OnViewZoom(wxCommandEvent& event);

	void OnHelpAbout(wxCommandEvent& event);

	void OnToolSelection(wxCommandEvent& event);

private:

	void SetActiveTool(int toolId);
	void CheckForUnsavedChanges();

	void InitMainFrame();
	void CreateMenuBar();
	void CreateToolBar();
	void InitAuiIbjects();

	wxAuiToolBar* m_toolBar;
	wxAuiToolBar* m_activeToolBar;

	rqtGLView* m_glView;
	rwxObjectBrowser* m_objectBrowser;
	wxStatusBar* m_statusBar;
	wxTextCtrl* m_logCtrl;
        wxImageList* m_imgList;


	wxAuiManager m_auiManager;

	rqtGameWorld* m_gameWorld;
	rqtQuadtreeController* m_controller;
	rOpenGLGraphicsDevice* m_graphicsDevice;
	rRenderer* m_renderer;
	rqtQueryViewWindow* m_queryView;
	rqtToolManager* m_toolManager;
	rqtShapePropertyViewer* m_propertyGrid;
	
	

	DECLARE_EVENT_TABLE()
};

#endif