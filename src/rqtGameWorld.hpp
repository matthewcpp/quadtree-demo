#ifndef RQT_GAMEWORLD_HPP
#define RQT_GAMEWORLD_HPP

#include <map>
#include <vector>

#include <wx/wx.h>
#include <wx/xml/xml.h>

#include "rqtTypes.hpp"

#include "recondite.hpp"
#include "container/rQuadtree.hpp"

#include "rqtGameWorldBase.hpp"
#include "rqtShapes.hpp"

typedef rQuadtree<rqtShape*> rqtShapeTree;
typedef rqtShapeTree::result_type rqtShapeResult;
typedef rqtShapeResult::iterator rqtShapeTreeItr;
typedef rqtShapeTree::node_list rqtShapeNodeList;

typedef std::map < wxString , rqtShape*> rqtShapeMap;
typedef std::pair < wxString , rqtShape*> rqtShapeEntry;
typedef rqtShapeMap::iterator rqtShapeMapItr;

typedef std::vector<rqtShape*> rqtShapeList;

class rqtGameWorld : public rqtGameWorldBase{
public:

	rqtGameWorld();
	~rqtGameWorld();

	void Init(const rRectangle2& r , size_t depth);
	void Clear();

	inline bool HasShapeNamed(const wxString& name) const;

	rQuadtreeError AddRectangle(rqtRectangle* r);
	rQuadtreeError AddPoint(rqtPoint* p);
	rQuadtreeError AddCircle(rqtCircle* c);

	virtual bool UpdateItem(rqtShape* shape , const rVector2& p);
	virtual bool UpdateItem(rqtShape* shape , const rRectangle2& r);
	virtual bool UpdateItem(rqtShape* shape , const rCircle2& c);
	virtual bool UpdateItem(rqtShape* shape );

	bool RemoveShape(const wxString& name);
	bool RenameShape(const wxString& name, const wxString& newname);
	bool HasShapeNamed(const wxString& name);
	bool ShapeFitsInWorld(const rqtShape* shape);

	void QueryShapesInRect(const rRectangle2& r, rqtShapeResult& result);
	void QueryShapesInCircle(const rCircle2& c , rqtShapeResult& result);
	void QueryShapesInPoint(const rVector2& p , rqtShapeResult& result);

	void GetDrawablesInRect(const rRectangle2& r, rqtShapeResult& result);
	void QueryDrawablesInPoint(const rVector2& p , rqtShapeResult& result);
	void GetDrawableNodes(const rRectangle2& r, rqtShapeNodeList& result);

	void ClearLastQuery();
	inline rqtShape* GetLastQuery();

	inline size_t NumShapes() const;
	inline size_t TreeSize() const;
	inline size_t TreeDepth() const;
	virtual rRectangle2 WorldArea() const;
	inline bool IsInit();

	rqtShape* SelectObject(const wxString& name);
	bool ItemIsSelected(const rqtShape* item) const;
	rqtShape* GetSelection();
	wxString GetSelectedItemName();

	int GetShapeDepth(const wxString& name);
	wxString GetNextItemName() const;

	void TrimTree();

	bool Save(const wxString& path);
	bool Load(const wxString& path);

	void GetObjects(rqtShapeList& shapeList);

private:

	void DeleteShapeObjects();
	void SaveWorldInfo(wxXmlNode* parent);
	void SaveShapeInfo(wxXmlNode* parent);
	void LoadWorldInfo(wxXmlNode* parent);
	void LoadShapeInfo(wxXmlNode* parent);

	wxColor LoadColor(wxXmlNode* parent);
	void LoadPoint(wxXmlNode* parent);
	void LoadCircle(wxXmlNode* parent);
	void LoadRectangle(wxXmlNode* parent);
private:

	rqtShapeTree m_shapeTree;
	rqtShapeMap m_shapeMap;

	rqtShape* m_lastQuery;
	rqtShape* m_selection;
	size_t m_itemsCreated;
	bool m_isInit;
};

bool rqtGameWorld::IsInit(){
	return m_isInit;
}

rqtShape* rqtGameWorld::GetLastQuery(){
	return m_lastQuery;
}

bool rqtGameWorld::HasShapeNamed(const wxString& name) const{
	return m_shapeMap.find(name) != m_shapeMap.end();
}

size_t rqtGameWorld::NumShapes() const{
	return m_shapeTree.NumItems();
}

size_t rqtGameWorld::TreeSize() const{
	return m_shapeTree.NumNodes();
}

size_t rqtGameWorld::TreeDepth() const{
	return m_shapeTree.MaxDepth();
}

#endif