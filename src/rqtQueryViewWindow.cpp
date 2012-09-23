#include "rqtQueryViewWindow.hpp"

rqtQueryViewWindow::rqtQueryViewWindow(wxWindow *parent, wxWindowID id)
	:wxPanel(parent , id)
{
	InitQueryViewWindow();
}

void rqtQueryViewWindow::InitQueryViewWindow(){
	m_query_desc = new wxStaticText(this , wxID_ANY , wxEmptyString);

wxImageList* objImages = new wxImageList(16 ,16);
	objImages->Add(wxBitmap("content/point16.png", wxBITMAP_TYPE_PNG));
	objImages->Add(wxBitmap("content/rect16.png", wxBITMAP_TYPE_PNG));
	objImages->Add(wxBitmap("content/circle16.png", wxBITMAP_TYPE_PNG));

	m_result_view = new wxListView(this , wxID_ANY, wxDefaultPosition , wxDefaultSize , wxLC_REPORT| wxLC_SINGLE_SEL);
	m_result_view->AssignImageList(objImages,wxIMAGE_LIST_SMALL);
	m_result_view->InsertColumn(0, "Name");
	m_result_view->InsertColumn(1, "Type");
	m_result_view->InsertColumn(2, "Description");

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(m_query_desc , 0 , wxRIGHT , 5);
	mainSizer->Add(m_result_view , 1 , wxALL| wxEXPAND , 5);
	SetSizer(mainSizer);
}

void rqtQueryViewWindow::Clear(){
	m_query_desc->SetLabel(wxEmptyString);
	m_items.clear();
	m_result_view->DeleteAllItems();
}

void rqtQueryViewWindow::ShowRectQueryResults(const rRectangle2& r , rqtShapeResult& result){
	m_query_desc->SetLabel(wxString::Format("Rectangle Query: x%.2f , y: %.2f , width: %.2f , height:%.2f %i Items Returned", r.x , r.y , r.width, r.height, result.size()));
	m_result_view->DeleteAllItems();

	for (rqtShapeTreeItr it = result.begin(); it != result.end(); ++it)
		AddShapeResult(*it);
}

void rqtQueryViewWindow::ShowPointQueryResults(const rVector2& p , rqtShapeResult& result){
	m_query_desc->SetLabel(wxString::Format("Point Query: x%.2f , y: %.2f %i Items Returned", p.x , p.y , result.size()));
	m_result_view->DeleteAllItems();

	for (rqtShapeTreeItr it = result.begin(); it != result.end(); ++it)
		AddShapeResult(*it);
}

void rqtQueryViewWindow::ShowCircleQueryResults(const rCircle2& c , rqtShapeResult& result){
	m_query_desc->SetLabel(wxString::Format("Circle Query: center: %.2f , %.2f radius: %.2f %i Items Returned", c.center.x , c.center.y , c.radius, result.size()));
	m_result_view->DeleteAllItems();

	for (rqtShapeTreeItr it = result.begin(); it != result.end(); ++it)
		AddShapeResult(*it);
}

void rqtQueryViewWindow::RemoveItemFromView(const wxString& name){
	rwxStringIntItr it = m_items.find(name);

	if (it == m_items.end())
		return;

	m_result_view->DeleteItem(it->second);
	m_items.erase(it);
}


void rqtQueryViewWindow::AddShapeResult(rqtShape* shape){
	int index = m_result_view->GetItemCount();
	wxString name = shape->Name();
	int image;

	wxString type = shape->GetType();

	if (type == "Point")
		image = 0;
	else if (type == "Rectangle")
		image = 1;
	else if (type == "Circle")
		image = 2;

	wxListItem item;
	item.SetId(index);
	item.SetImage(image);
	item.SetText(name);
	m_result_view->InsertItem(item);

	item.SetImage(-1);
	item.SetText(type);
	item.SetColumn(1);
	m_result_view->SetItem(item);

	item.SetColumn(2);
	item.SetText(shape->GetInfo());
	m_result_view->SetItem(item);

	m_items[name] = index;
}

wxString rqtQueryViewWindow::GetSelectedShapeName() const{
	long id = m_result_view->GetFirstSelected();
	return m_result_view->GetItemText(id , 0);
}

bool rqtQueryViewWindow::SetItemSelection(const wxString& name){
	rwxStringIntItr result = m_items.find(name);

	if (result == m_items.end() ){

		if (m_result_view->GetSelectedItemCount() > 0)
			m_result_view->Select(m_result_view->GetFirstSelected() , false);

		return false;

	}

	m_result_view->Select(result->second);
	return true;
}

bool rqtQueryViewWindow::RenameItem(const wxString& name , const wxString& newName){
	rwxStringIntItr result = m_items.find(name);

	if (result == m_items.end() )
		return false;

	int id = result->second;
	m_items.erase(result);

	m_items[newName] = id;
	m_result_view->SetItemText(id , newName);
	return true;
}