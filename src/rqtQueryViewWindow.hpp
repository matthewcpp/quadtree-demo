#ifndef RQT_QUERYVIEWWINDOW_HPP
#define RQT_QUERYVIEWWINDOW_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>

#include <map>

#include "rqtTypes.hpp"
#include "rqtShapes.hpp"

#include "rqtGameWorld.hpp"

typedef std::map<wxString  , int> rwxStringIntMap;
typedef rwxStringIntMap::iterator rwxStringIntItr;

class rqtQueryViewWindow : public wxPanel{
public:
	rqtQueryViewWindow(wxWindow *parent, wxWindowID id=wxID_ANY);

	void Clear();
	void ShowRectQueryResults(const rRectangle2& r , rqtShapeResult& result);
	void ShowPointQueryResults(const rVector2& p , rqtShapeResult& result);
	void ShowCircleQueryResults(const rCircle2& c , rqtShapeResult& result);

	void RemoveItemFromView(const wxString& name);
	bool RenameItem(const wxString& name , const wxString& newName);

	wxString GetSelectedShapeName() const;
	bool SetItemSelection(const wxString& name);

private:
	void AddShapeResult(rqtShape* shape);
	void InitQueryViewWindow();

private:
	wxStaticText* m_query_desc;

	wxListView* m_result_view;

	rwxStringIntMap m_items;
};

#endif