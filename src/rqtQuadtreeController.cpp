#include "rqtQuadtreeController.hpp"

rqtQuadtreeController::rqtQuadtreeController(rqtShapePropertyViewer* propertyGrid,rwxObjectBrowser* objectBrowser , wxStatusBar* statusBar ,rqtGameWorld* gameWorld, rqtGLView* worldView, rqtQueryViewWindow* queryView){
	m_objectBrowser = NULL;
	m_gameWorld = gameWorld;
	m_worldView = worldView;
	m_queryView=queryView;
	m_statusBar = statusBar;
	m_propertyGrid = propertyGrid;
	m_isModified = false;
	m_loadPath = wxEmptyString;

	m_objectBrowser = objectBrowser;
	m_objectBrowser->Bind(wxEVT_COMMAND_TREE_ITEM_MENU , &rqtQuadtreeController::OnBrowserContextMenu, this);
	m_objectBrowser->Bind(wxEVT_COMMAND_TREE_SEL_CHANGED , &rqtQuadtreeController::OnBrowserSelectItem, this);
	
	m_queryView->Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK , &rqtQuadtreeController::OnQueryViewContextMenu, this);
	m_queryView->Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED , &rqtQuadtreeController::OnQueryViewSelectItem, this);

	m_propertyGrid->Bind(wxEVT_PG_CHANGING , &rqtQuadtreeController::OnShapeItemPropertyChange, this);
}

rqtQuadtreeController::~rqtQuadtreeController(){
}

void rqtQuadtreeController::UpdateStatusBar(){
	size_t depth = m_gameWorld->TreeDepth();
	rRectangle2 r = m_gameWorld->WorldArea();

	wxString str = wxString::Format("%.2f,%.2f %.2fx%.2f : %i", r.x , r.y , r.width ,r.height, depth);
	m_statusBar->SetStatusText(str, 1);
}

void rqtQuadtreeController::ResetUIElements(size_t depth){
	m_objectBrowser->InitWithDepth(depth);
	m_queryView->Clear();
	m_propertyGrid->Clear();
	UpdateStatusBar();
	m_worldView->ResetView();
	ClearQueryResults();
}

void rqtQuadtreeController::LoadTree(const wxString& path){
	wxLogMessage("Load Quadtree from file: " + path);

	m_gameWorld->Load(path);
	ResetUIElements(m_gameWorld->TreeDepth());

	rqtShapeList objects;
	m_gameWorld->GetObjects(objects);
	wxString name;
	int img;

	for (size_t i = 0; i < objects.size(); i++){
		name =  objects[i]->Name();
		img = GetShapeImageIndex(objects[i]->GetType());
		m_objectBrowser->AddObject( name,img, m_gameWorld->GetShapeDepth(name));
	}

	
	m_worldView->Refresh();
	m_loadPath = path;
	m_isModified = false;
}

void rqtQuadtreeController::InitQuadtree(const rRectangle2& r , size_t depth){
	
	m_gameWorld->Init(r , depth);
	wxLogMessage("init quadtree: %.2f,%.2f %.2fx%.2f : %i", r.x , r.y , r.width ,r.height, depth);
	
	ResetUIElements(depth);
	m_loadPath = wxEmptyString;
	m_isModified = true;
}

void rqtQuadtreeController::SaveTree(){
	if (!m_isModified)
		return;

	if (m_loadPath == wxEmptyString){
		wxFileDialog dialog(NULL , "Save Quadtree" , "", "", "Quadtree files (*.rqt)|*.rqt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

		if (dialog.ShowModal() != wxID_OK)
			return;

		m_loadPath =dialog.GetPath();
	}

	m_gameWorld->Save(m_loadPath);
	m_isModified = false;
}

bool  rqtQuadtreeController::CheckItemName(const wxString& name){
	if (m_gameWorld->HasShapeNamed(name)){
		wxMessageBox("There is already an item named: " + name);
		return false;
	}

	return true;
}

void rqtQuadtreeController::ProcessItemInsert(int img , const wxString& name , const wxColor& c){
	int depth = m_gameWorld->GetShapeDepth(name);

	if(m_objectBrowser && depth != -1)
		m_objectBrowser->AddObject(name , img , depth);
	
	UpdateStatusBar();
	SelectItemByName(name);
}

bool rqtQuadtreeController::CreatePointObject(const wxString& name , const wxColor& color , const rVector2& p){

	if (!CheckItemName(name))
		return false;

	rqtPoint* point = new rqtPoint(name , color , p);
	rQuadtreeError error = m_gameWorld->AddPoint(point);

	if (error){
		DisplayQuadtreeError(error);
		return false;
	}
	else{
		wxLogMessage("Created Point " + point->Name() + " " + point->GetInfo());
		ProcessItemInsert(0, name , color);
		m_isModified = true;
		return true;
	}
}

bool rqtQuadtreeController::CreateRectangleObject(const wxString& name , const wxColor& color , const rRectangle2& r){
	if (!CheckItemName(name))
		return false;

	rqtRectangle* rectangle = new rqtRectangle(name , color , r);
	rQuadtreeError error = m_gameWorld->AddRectangle(rectangle);

	if (error){
		DisplayQuadtreeError(error);
		return false;
	}
	else{
		wxLogMessage("Created Rectangle " + rectangle->Name() + " " + rectangle->GetInfo());
		ProcessItemInsert(1, name , color);
		m_isModified = true;
		return true;
	}
}

bool rqtQuadtreeController::CreateCircleObject(const wxString& name , const wxColor& color , const rCircle2& circle){
	if (!CheckItemName(name))
		return false;

	rqtCircle* cir = new rqtCircle(name , color , circle);
	rQuadtreeError error =m_gameWorld->AddCircle(cir);

	if (error){
		DisplayQuadtreeError(error);
		return false;
	}
	else{
		wxLogMessage("Created Circle " + cir->Name() + " " + cir->GetInfo());
		ProcessItemInsert(2, name , color);
		m_isModified = true;
		return true;
	}
}

void rqtQuadtreeController::QueryPoint(const rVector2& p){

	rqtShapeResult result;

	m_gameWorld->QueryShapesInPoint(p , result);
	wxLogMessage("Point Query: x%.2f , y: %.2f %i Items Returned", p.x , p.y, result.size());
	
	m_queryView->ShowPointQueryResults(p , result);
	m_queryView->SetItemSelection(m_gameWorld->GetSelectedItemName());
	m_worldView->Refresh();
}

void rqtQuadtreeController::QueryRectangle(const rRectangle2& r){
	rqtShapeResult result;

	m_gameWorld->QueryShapesInRect(r , result);
	wxLogMessage("Rectangle Query: x%.2f , y: %.2f , width: %.2f , height:%.2f %i Items Returned", r.x , r.y , r.width, r.height, result.size());
	m_queryView->ShowRectQueryResults(r , result);
	m_queryView->SetItemSelection(m_gameWorld->GetSelectedItemName());
	m_worldView->Refresh();
}

void rqtQuadtreeController::QueryCircle(const rCircle2& c){
	rqtShapeResult result;

	m_gameWorld->QueryShapesInCircle(c , result);

	wxLogMessage("Circle Query: center: %.2f , %.2f radius: %.2f %i Items Returned", c.center.x , c.center.y , c.radius, result.size());
	m_queryView->ShowCircleQueryResults(c , result);
	m_queryView->SetItemSelection(m_gameWorld->GetSelectedItemName());
	m_worldView->Refresh();
}

void rqtQuadtreeController::ClearQuadtree(){
	if (m_gameWorld->IsInit()){
		wxLogMessage ("Clearing quadtree contianing %i items" , m_gameWorld->NumShapes());

		m_gameWorld->Clear();
		ClearQueryResults();

		m_objectBrowser->ClearObjects();
		m_queryView->Clear();
		m_propertyGrid->Clear();

		UpdateStatusBar();
		m_worldView->Refresh();

		m_isModified = true;
	}
}

void rqtQuadtreeController::RemoveObject(const wxString& name){
	wxString selection  = m_gameWorld->GetSelectedItemName();

	if (m_gameWorld->RemoveShape(name)){
		wxLogMessage ("Removed item: " + name);
		m_objectBrowser->RemoveObject(name);
		m_queryView->RemoveItemFromView(name);
		UpdateStatusBar();
		m_worldView->Refresh();

		if (selection == name)
			m_propertyGrid->Clear();

		m_isModified = true;
	}
}

wxString rqtQuadtreeController::GetNextItemName(){
	return m_gameWorld->GetNextItemName();
}

void rqtQuadtreeController::OnBrowserContextMenu(wxTreeEvent& event){
	wxTreeItemId selection = m_objectBrowser->GetSelection();
	wxString name = m_objectBrowser->GetItemText(selection);

	wxMenu popupMenu;
	popupMenu.Append(99, "Delete " + name);

	int choice = m_objectBrowser->GetPopupMenuSelectionFromUser(popupMenu);

	if (choice == 99)
		RemoveObject(name);
}

void rqtQuadtreeController::OnQueryViewContextMenu(wxListEvent& event){
	wxString name = event.GetText();
	

	wxMenu popupMenu;
	popupMenu.Append(99, "Delete " + name);

	int choice = m_queryView->GetPopupMenuSelectionFromUser(popupMenu);

	if (choice == 99)
		RemoveObject(name);
}

void rqtQuadtreeController::ClearQueryResults(){
	m_queryView->Clear();
	m_gameWorld->ClearLastQuery();
}

void rqtQuadtreeController::OnBrowserSelectItem(wxTreeEvent& event){
	wxTreeItemId selection = m_objectBrowser->GetSelection();
	wxString name = m_objectBrowser->GetItemText(selection);
	SelectItemByName(name);

}

void rqtQuadtreeController::OnQueryViewSelectItem(wxListEvent& event){
	wxString name = m_queryView->GetSelectedShapeName();

	m_queryView->Unbind(wxEVT_COMMAND_LIST_ITEM_SELECTED , &rqtQuadtreeController::OnQueryViewSelectItem, this);
	SelectItemByName(name);
	m_queryView->Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED , &rqtQuadtreeController::OnQueryViewSelectItem, this);

}

rqtShape* rqtQuadtreeController::SelectItemByPoint(const rVector2& p){
	rqtShapeResult result;

	m_gameWorld->QueryDrawablesInPoint(p , result);

	rqtShape* shape;
	for (rqtShapeTreeItr it = result.begin(); it != result.end(); ++it){
		shape = *it;

		if (shape->ContainsPoint(p)){
			SelectItemByName(shape->Name());
			return shape;
		}
	}

	return NULL;
}

rqtShape* rqtQuadtreeController::SelectItemByName(const wxString& name){
	rqtShape* selection = m_gameWorld->SelectObject(name);
	
	if (selection)
		wxLogMessage("Selecting item: " + name);
	
	m_propertyGrid->DisplayShape(selection);

	m_objectBrowser->SetItemSelection(name);
	m_queryView->SetItemSelection(name);
	m_worldView->Refresh();

	return selection;
}

void rqtQuadtreeController::OnShapeItemPropertyChange(wxPropertyGridEvent& event){
	rqtShape* shape = m_gameWorld->GetSelection();

	if (!shape)
		return;

	wxString label = event.GetPropertyName();
	wxAny value= event.GetPropertyValue();
	wxString itemName = shape->Name();
        int depth = m_gameWorld->GetShapeDepth(itemName);

	if (label == "Name"){
		if (!RenameObject(shape->Name() , value.As<wxString>()))
			event.Veto();
	}

	if (shape->SetProperty(label , value, m_gameWorld)){
		wxLogMessage("Edit Property: " + label + " for " + itemName + " --> "+ shape->GetInfo());

		wxString type = shape->GetType();
		int index;

		if (type == "Point")
			index = 0;
		else if (type == "Rectangle")
			index = 1;
		else
			index = 2;

                int newDepth = m_gameWorld->GetShapeDepth(itemName);
                if (depth != newDepth){
                    m_objectBrowser->RemoveObject(itemName);
                    m_objectBrowser->AddObject(itemName, index , newDepth);
                }

		m_isModified = true;

		m_worldView->Refresh();
	}
	else
		event.Veto();
}

bool rqtQuadtreeController::RenameObject(const wxString& itemName , const wxString& newName){
	if (m_gameWorld->RenameShape(itemName, newName)){
		m_objectBrowser->RenameObject(itemName, newName);
		m_queryView->RenameItem(itemName,newName);
		m_isModified = true;
		return true;
	}

	else{
		wxMessageBox("There is already an item named: "+newName );
		return false;
	}
}

void rqtQuadtreeController::DisplayQuadtreeError(rQuadtreeError error){
	switch (error){
	case rQUADTREE_ERROR_NOT_INIT:
		wxMessageBox("Quadtree not initialized");
		break;

	case rQUADTREE_ERROR_ITEM_NOT_FOUND:
		wxMessageBox("Item not found in quadtree");
		break;

	case rQUADTREE_ERROR_ITEM_ALREADY_IN_TREE:
		wxMessageBox("Item already exists in quadtree");
		break;

	case rQUADTREE_ERROR_ITEM_DOES_NOT_FIT:
		wxMessageBox("Item does not fit in quadtree");
		break;
	};
}

rqtShape* rqtQuadtreeController::GetSelectedItem(){
	return m_gameWorld->GetSelection();
}

void rqtQuadtreeController::UpdateObjectSelection(){
	rqtShape* selection = m_gameWorld->GetSelection();
	wxString name = selection->Name();

	int oldDepth = m_gameWorld->GetShapeDepth(name);
	m_gameWorld->UpdateItem(selection);
	int newDepth = m_gameWorld->GetShapeDepth(name);

	if (oldDepth != newDepth){
		m_objectBrowser->RemoveObject(name);
		m_objectBrowser->AddObject(name , GetShapeImageIndex(selection->GetType()) , newDepth);
                SelectItemByName(name);
	}


	m_propertyGrid->DisplayShape(selection);
	m_isModified = true;

}

int rqtQuadtreeController::GetShapeImageIndex(const wxString& type){
	if (type == "Point")
		return 0;
	else if (type == "Rectangle")
		return 1;
	else //if (type == "Circle")
		return 2;
}

bool rqtQuadtreeController::ShapeFitsInWorld(const rqtShape* shape){
	return m_gameWorld->ShapeFitsInWorld(shape);
}

void rqtQuadtreeController::TrimQuadtree(){
	if (m_gameWorld->IsInit()){
		size_t oldSize = m_gameWorld->TreeSize();
		m_gameWorld->TrimTree();
		size_t newSize = m_gameWorld->TreeSize();

		wxLogMessage("Trimmed Quadtree removing %i empty nodes" , oldSize - newSize);
		m_worldView->Refresh();

		m_isModified = true;
	}
}

bool rqtQuadtreeController::IsModified() const{
	return m_isModified;
}