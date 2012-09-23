#include "rqtGameWorld.hpp"

rqtGameWorld::rqtGameWorld(){
	m_lastQuery = NULL;
	m_selection = NULL;

	m_itemsCreated = 0;
	m_isInit = false;
}

rqtGameWorld::~rqtGameWorld(){
	Clear();
}

void rqtGameWorld::ClearLastQuery(){
	if (m_lastQuery)
		delete m_lastQuery;

	m_lastQuery = NULL;
}

void rqtGameWorld::Init(const rRectangle2& r , size_t depth){
	DeleteShapeObjects();
	ClearLastQuery();

	m_shapeTree.Init(r, depth);

	m_selection = NULL;
	m_isInit = true;
}

void rqtGameWorld::Clear(){
	m_selection = NULL;
	DeleteShapeObjects();
	ClearLastQuery();
	m_shapeTree.Clear();
}

rQuadtreeError rqtGameWorld::AddRectangle(rqtRectangle* r){
	rQuadtreeError error = m_shapeTree.InsertItemWithRect(r , r->Rectangle());
	
	if (!error){
		m_shapeMap[r->Name()] = r;
		m_itemsCreated++;
	}
	else
		delete r;

	return error;
}

rQuadtreeError rqtGameWorld::AddPoint(rqtPoint* p){
	rQuadtreeError error = m_shapeTree.InsertItemWithPoint(p , p->Point());
	
	if (!error){
		m_shapeMap[p->Name()] = p;
		m_itemsCreated++;
	}
	else 
		delete p;

	return error;
}

rQuadtreeError rqtGameWorld::AddCircle(rqtCircle* c){
	rQuadtreeError error = m_shapeTree.InsertItemWithCircle(c , c->Circle());

	if (!error){
		m_shapeMap[c->Name()] = c;
		m_itemsCreated++;
	}
	else
		delete c;

	return error;
}

void rqtGameWorld::DeleteShapeObjects(){
	for (rqtShapeMapItr it = m_shapeMap.begin(); it != m_shapeMap.end(); ++it)
		delete it->second;

	m_shapeMap.clear();
}

void rqtGameWorld::GetDrawablesInRect(const rRectangle2& r, rqtShapeResult& result){
	return m_shapeTree.QueryItemsWithRect(r , result);
}

void rqtGameWorld::QueryDrawablesInPoint(const rVector2& p , rqtShapeResult& result){
	return m_shapeTree.QueryItemsWithPoint(p , result);
}

void rqtGameWorld::GetDrawableNodes(const rRectangle2& r, rqtShapeNodeList& result){
	return m_shapeTree.QueryNodesWithRect(r, result);
}

void rqtGameWorld::QueryShapesInRect(const rRectangle2& r, rqtShapeResult& result){
	ClearLastQuery();
	m_lastQuery = new rqtRectangle("Search" , *wxYELLOW , r);
	m_shapeTree.QueryItemsWithRect(r , result);
}

void rqtGameWorld::QueryShapesInCircle(const rCircle2& c , rqtShapeResult& result){
	ClearLastQuery();
	m_lastQuery = new rqtCircle("Search" , *wxYELLOW , c);
	m_shapeTree.QueryItemsWithCircle(c , result);
}

void rqtGameWorld::QueryShapesInPoint(const rVector2& p , rqtShapeResult& result){
	ClearLastQuery();
	m_lastQuery = new rqtPoint("Search" , *wxYELLOW , p);
	m_shapeTree.QueryItemsWithPoint(p , result);
}

int rqtGameWorld::GetShapeDepth(const wxString& name){
	rqtShapeMapItr it = m_shapeMap.find(name);

	if (it == m_shapeMap.end())
		return -1;

	return m_shapeTree.ItemDepth(it->second);
}

bool rqtGameWorld::RemoveShape(const wxString& name){
	rqtShapeMapItr it = m_shapeMap.find(name);

	if (it == m_shapeMap.end())
		return false;

	m_shapeTree.RemoveItem(it->second);

	if (it->second == m_selection)
		m_selection = NULL;

	delete it->second;
	m_shapeMap.erase(it);

	return true;

}

wxString rqtGameWorld::GetNextItemName() const{
	wxString name;
	size_t i = m_itemsCreated;
	do{
		name =  wxString::Format("Item %i" , i);
		i++;
	}while(HasShapeNamed(name));

	return name;
}

rqtShape* rqtGameWorld::SelectObject(const wxString& name){
	rqtShapeMapItr result = m_shapeMap.find(name);

	if (result == m_shapeMap.end())
		m_selection = NULL;
	else
		m_selection = result->second;

	return m_selection;
}

bool rqtGameWorld::ItemIsSelected(const rqtShape* item) const{
	return item == m_selection;
}

rqtShape* rqtGameWorld::GetSelection(){
	return m_selection;
}

wxString rqtGameWorld::GetSelectedItemName(){
	if (m_selection)
		return m_selection->Name();

	return wxEmptyString;
}

bool rqtGameWorld::HasShapeNamed(const wxString& name){
	return  m_shapeMap.find(name) != m_shapeMap.end();
}

bool rqtGameWorld::RenameShape(const wxString& name, const wxString& newName){
	rqtShapeMapItr result = m_shapeMap.find(name);

	if (result == m_shapeMap.end() || HasShapeNamed(newName))
		return false;

	rqtShape* shape = result->second;
	m_shapeMap.erase(result);

	shape->SetName(newName);
	m_shapeMap[newName] = shape;
	return true;
}

rRectangle2 rqtGameWorld::WorldArea() const{
	return m_shapeTree.GetArea();
}

bool rqtGameWorld::UpdateItem(rqtShape* shape , const rVector2& p){
	rQuadtreeError error = m_shapeTree.UpdateItemWithPoint(shape , p);

	return error == rQUADTREE_ERROR_NONE;
}

bool rqtGameWorld::UpdateItem(rqtShape* shape , const rRectangle2& r){
	rQuadtreeError error = m_shapeTree.UpdateItemWithRect(shape , r);

	return error == rQUADTREE_ERROR_NONE;
}

bool rqtGameWorld::UpdateItem(rqtShape* shape , const rCircle2& c){
	rQuadtreeError error = m_shapeTree.UpdateItemWithCircle(shape , c);

	return error == rQUADTREE_ERROR_NONE;
}

bool rqtGameWorld::UpdateItem(rqtShape* shape ){
	wxString type = shape->GetType();

	if (type == "Point"){
		rqtPoint* p= static_cast<rqtPoint*>(shape);
		return UpdateItem(p , p->Point());
	}
	else if (type == "Circle"){
		rqtCircle* c = static_cast<rqtCircle*>(shape);
		return UpdateItem(c , c->Circle());
	}
	else if (type == "Rectangle"){
		rqtRectangle* r = static_cast<rqtRectangle*>(shape);
		return UpdateItem(r , r->Rectangle());
	}

	return false;
}

bool rqtGameWorld::ShapeFitsInWorld(const rqtShape* shape){
	 rRectangle2 r = WorldArea();
	wxString type = shape->GetType();

	if (type == "Point"){
		const rqtPoint* p= static_cast<const rqtPoint*>(shape);
		return r.ContainsPoint( p->Point());
	}
	else if (type == "Circle"){
		const rqtCircle* c = static_cast<const rqtCircle*>(shape);
		return rCollision2::RectangleContainsCircle(r , c->Circle());
	}
	else if (type == "Rectangle"){
		const rqtRectangle* rect = static_cast<const rqtRectangle*>(shape);
		return r.ContainsRectangle( rect->Rectangle());
	}

	return false;

}

void rqtGameWorld::TrimTree(){
	m_shapeTree.Trim();
}

bool rqtGameWorld::Save(const wxString& path){

	if (!IsInit())
		return false;

	wxXmlDocument doc;
	wxXmlNode* root = new wxXmlNode( wxXML_ELEMENT_NODE , "Quadtree");
	doc.SetRoot(root);

	wxXmlNode* worldInfo = new wxXmlNode(wxXML_ELEMENT_NODE , "Area");
	SaveWorldInfo(worldInfo);

	wxXmlNode* shapeInfo = new wxXmlNode(wxXML_ELEMENT_NODE , "Shapes");
	SaveShapeInfo(shapeInfo);

	root->AddChild(worldInfo);
	root->AddChild(shapeInfo);

	return doc.Save(path);
}

bool rqtGameWorld::Load(const wxString& path){
	wxXmlDocument doc(path);

	wxXmlNode* root = doc.GetRoot();

	wxXmlNode* node = root->GetChildren();
	LoadWorldInfo(node);

	node = node->GetNext();
	LoadShapeInfo(node);

	return true;
}

void rqtGameWorld::SaveWorldInfo(wxXmlNode* parent){
	rRectangle2 r = m_shapeTree.GetArea();
	
	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE ,"x");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "x" , wxString::Format("%f", r.x)));
	parent->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"y");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "y" , wxString::Format("%f", r.y)));
	parent->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"width");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "width" , wxString::Format("%f", r.width)));
	parent->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"height");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "height" , wxString::Format("%f", r.height)));
	parent->AddChild(node);

	node = new wxXmlNode(wxXML_ELEMENT_NODE ,"depth");
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "depth" , wxString::Format("%d", m_shapeTree.MaxDepth())));
	parent->AddChild(node);
}

void rqtGameWorld::SaveShapeInfo(wxXmlNode* parent){
	for (rqtShapeMapItr it = m_shapeMap.begin(); it != m_shapeMap.end(); it++)
		it->second->SerializeShape(parent);
}

void rqtGameWorld::LoadWorldInfo(wxXmlNode* parent){
	rRectangle2 area;
	unsigned long depth;
	double d;
	wxString content;

	wxXmlNode* node = parent->GetChildren();

	content = node->GetNodeContent();
	content.ToDouble(&d);
	area.x = (float)d;
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToDouble(&d);
	area.y = (float)d;
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToDouble(&d);
	area.width = (float)d;
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToDouble(&d);
	area.height = (float)d;
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToULong(&depth);

	Init(area , depth);
}

void rqtGameWorld::LoadShapeInfo(wxXmlNode* parent){
	wxXmlNode* shape = parent->GetChildren();
	wxString type;

	while (shape){
		type = shape->GetName();

		if (type == "Point")
			LoadPoint(shape);
		else if (type == "Rectangle")
			LoadRectangle(shape);
		else if (type == "Circle")
			LoadCircle(shape);

		shape = shape->GetNext();
	}

}

wxColor rqtGameWorld::LoadColor(wxXmlNode* parent){
	unsigned long r,g,b,a;
	wxString content;

	wxXmlNode* node = parent->GetChildren();

	content = node->GetNodeContent();
	content.ToULong(&r);
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToULong(&g);
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToULong(&b);
	node = node->GetNext();

	content = node->GetNodeContent();
	content.ToULong(&a);
	node = node->GetNext();

	return wxColor(r,g,b,a);
}

void rqtGameWorld::LoadPoint(wxXmlNode* parent){
	double d;
	rVector2 p;
	wxColor c;
	wxString name, value;

	wxXmlNode* node = parent->GetChildren();
	name = node->GetNodeContent();
	node = node->GetNext();

	c = LoadColor(node);
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	p.x = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	p.y = (float)d;

	AddPoint(new rqtPoint(name , c , p));
}

void rqtGameWorld::LoadCircle(wxXmlNode* parent){
	double d;
	rCircle2 cir;
	wxColor c;
	wxString name, value;

	wxXmlNode* node = parent->GetChildren();
	name = node->GetNodeContent();
	node = node->GetNext();

	c = LoadColor(node);
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	cir.center.x = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	cir.center.y = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	cir.radius = (float)d;

	AddCircle(new rqtCircle(name , c , cir));
}

void rqtGameWorld::LoadRectangle(wxXmlNode* parent){
	double d;
	rRectangle2 r;
	wxColor c;
	wxString name, value;

	wxXmlNode* node = parent->GetChildren();
	name = node->GetNodeContent();
	node = node->GetNext();

	c = LoadColor(node);
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	r.x = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	r.y = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	r.width = (float)d;
	node = node->GetNext();

	value = node->GetNodeContent();
	value.ToDouble(&d);
	r.height = (float)d;

	AddRectangle(new rqtRectangle(name , c, r));
}

void rqtGameWorld::GetObjects(rqtShapeList& shapeList){
	shapeList.resize (m_shapeMap.size());
	size_t i = 0;

	for (rqtShapeMapItr it = m_shapeMap.begin(); it != m_shapeMap.end(); ++it)
		shapeList[i++] = it->second;
	
}
