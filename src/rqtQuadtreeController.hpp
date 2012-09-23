#ifndef RQT_QUADTREE_CONTROLLER_HPP
#define RQT_QUADTREE_CONTROLLER_HPP

#include <vector>

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "recondite.hpp"
#include "container/rQuadtree.hpp"

#include "rqtTypes.hpp"
#include "rqtGLView.hpp"

#include "rqtShapes.hpp"
#include "rqtGameWorld.hpp"
#include "rqtQueryViewWindow.hpp"
#include "rqtQuadtreeControllerI.hpp"
#include "rwxObjectBrowser.hpp"
#include "rqtShapePropertyViewer.hpp"



class rqtQuadtreeController : public wxEvtHandler , public rqtQuadtreeControllerI{
public:
	rqtQuadtreeController(rqtShapePropertyViewer* propertyGrid, rwxObjectBrowser* objectBrowser , wxStatusBar* statusBar , rqtGameWorld* gameWorld, rqtGLView* worldView, rqtQueryViewWindow* queryView);
	~rqtQuadtreeController();

	wxString GetNextItemName();

	void LoadTree(const wxString& path);
	void InitQuadtree(const rRectangle2& r , size_t depth);
	void ClearQuadtree();

	virtual bool CreatePointObject(const wxString& name , const wxColor& color , const rVector2& p);
	virtual bool CreateRectangleObject(const wxString& name , const wxColor& color , const rRectangle2& r);
	virtual bool CreateCircleObject(const wxString& name , const wxColor& color , const rCircle2& circle);
	
	void RemoveObject(const wxString& name);
	bool RenameObject(const wxString& itemName , const wxString& newName);

	virtual void QueryPoint(const rVector2& p);
	virtual void QueryRectangle(const rRectangle2& r);
	virtual void QueryCircle(const rCircle2& c);
	virtual void SaveTree();

	virtual rqtShape* SelectItemByName(const wxString& name);
	virtual rqtShape* SelectItemByPoint(const rVector2& p);

	virtual rqtShape* GetSelectedItem();

	void ClearQueryResults();

	void UpdateStatusBar();
	virtual void UpdateObjectSelection();
	virtual bool ShapeFitsInWorld(const rqtShape* shape);

	void TrimQuadtree();
	bool IsModified() const;

public:

	void OnBrowserContextMenu(wxTreeEvent& event);
	void OnBrowserSelectItem(wxTreeEvent& event);

	void OnQueryViewContextMenu(wxListEvent& event);
	void OnQueryViewSelectItem(wxListEvent& event);

	void OnShapeItemPropertyChange(wxPropertyGridEvent& event);

	static int GetShapeImageIndex(const wxString& type);


private:

	void ResetUIElements(size_t depth);
	static void DisplayQuadtreeError(rQuadtreeError error);
	bool CheckItemName(const wxString& name);
	void ProcessItemInsert(int img , const wxString& name , const wxColor& c);

private:
	rwxObjectBrowser* m_objectBrowser;
	

	wxStatusBar* m_statusBar;

	rqtGameWorld* m_gameWorld;
	rqtGLView* m_worldView;
	rqtQueryViewWindow* m_queryView;
	rqtShapePropertyViewer* m_propertyGrid;

	bool m_isModified;
	wxString m_loadPath;
};

#endif