#include "rqtMainFrame.hpp"

BEGIN_EVENT_TABLE(rqtMainFrame , wxFrame)
	EVT_MENU(wxID_SAVE, rqtMainFrame::OnSave)
	EVT_MENU(wxID_OPEN, rqtMainFrame::OnLoad)
	EVT_MENU(wxID_EXIT, rqtMainFrame::OnExit)

	EVT_MENU(rqtINIT_TREE, rqtMainFrame::OnInitTree)
	EVT_MENU(rqtCLEAR_TREE, rqtMainFrame::OnClearTree)
	EVT_MENU(rqtTRIM_TREE, rqtMainFrame::OnTrimTree)

	EVT_MENU(rqtINSERT_POINT, rqtMainFrame::OnInsertPoint)
	EVT_MENU(rqtINSERT_RECT, rqtMainFrame::OnInsertRectangle)
	EVT_MENU(rqtINSERT_CIRCLE, rqtMainFrame::OnInsertCircle)

	EVT_MENU(rqtQUERY_POINT, rqtMainFrame::OnQueryPoint)
	EVT_MENU(rqtQUERY_RECT, rqtMainFrame::OnQueryRectangle)
	EVT_MENU(rqtQUERY_CIRCLE, rqtMainFrame::OnQueryCircle)

	EVT_MENU(rqtVIEW_LAST_QUERY, rqtMainFrame::OnViewLastQuery)
	EVT_MENU(rqtVIEW_RESET, rqtMainFrame::OnViewReset)

	EVT_MENU(wxID_ABOUT , rqtMainFrame::OnHelpAbout)

	EVT_MENU_RANGE(rqtVIEW_WIRE , rqtVIEW_SHADED , rqtMainFrame::OnViewRenderMode)
	EVT_MENU_RANGE(rqtTOOL_ITEM_INSERT , rqtTOOL_ITEM_TRANSLATE , rqtMainFrame::OnToolSelection)
	EVT_MENU_RANGE(rqtVIEW_ZOOM_IN , rqtVIEW_ZOOM_OUT , rqtMainFrame::OnViewZoom)

	EVT_CLOSE(rqtMainFrame::OnFrameClose)

END_EVENT_TABLE()

rqtMainFrame::rqtMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
	:wxFrame(parent , id , title , pos , size , style , name)
{
	m_logCtrl = new wxTextCtrl(this , wxID_ANY,wxEmptyString,wxDefaultPosition , wxDefaultSize , wxTE_MULTILINE);
	wxLog::SetActiveTarget(new wxLogTextCtrl(m_logCtrl));

	m_gameWorld = new rqtGameWorld();

	m_graphicsDevice = new rOpenGLGraphicsDevice();
	m_renderer = new rRenderer(m_graphicsDevice);

	m_auiManager.SetManagedWindow(this);

	CreateMenuBar();
	CreateToolBar();

	InitMainFrame();
	InitAuiIbjects();

	m_controller = new rqtQuadtreeController(m_propertyGrid,m_objectBrowser, m_statusBar, m_gameWorld, m_glView, m_queryView);

	m_toolManager = new rqtToolManager(m_controller, this);
	m_glView->SetToolManager(m_toolManager);

	m_activeToolBar = NULL;
	SetActiveTool(rqtTOOL_ITEM_SELECT);
	m_glView->SetFocus();
}

rqtMainFrame::~rqtMainFrame(){
	m_auiManager.UnInit();

        delete m_imgList;

	delete m_graphicsDevice;
	delete m_renderer;
	delete m_controller;
	delete m_gameWorld;
	delete m_toolManager;
}


void rqtMainFrame::CreateMenuBar(){
	wxMenu* fileMenu = new wxMenu();
		fileMenu->Append(rqtINIT_TREE , "New Tree...\tCtrl+N");
		fileMenu->Append(wxID_OPEN , "&Open\tCtrl+O");
		fileMenu->Append(wxID_SAVE , "&Save\tCtrl+S");
		fileMenu->AppendSeparator();
		fileMenu->Append(wxID_EXIT , "&Exit\tCtrl+Q");

	wxMenu* insertMenu = new wxMenu();
		insertMenu->Append(rqtINSERT_POINT,"&Point...");
		insertMenu->Append(rqtINSERT_RECT,"&Rectangle...");
		insertMenu->Append(rqtINSERT_CIRCLE,"&Circle...");

	wxMenu* queryMenu = new wxMenu();
		queryMenu->Append(rqtQUERY_POINT,"&Point...");
		queryMenu->Append(rqtQUERY_RECT,"&Rectangle...");
		queryMenu->Append(rqtQUERY_CIRCLE,"&Circle...");

	wxMenu* treeMenu = new wxMenu();
	treeMenu->Append(rqtCLEAR_TREE , "Clear Tree...");
	treeMenu->Append(rqtTRIM_TREE , "Trim Tree");
	treeMenu->AppendSeparator();
		treeMenu->AppendSubMenu(insertMenu, "&Insert");
		treeMenu->AppendSubMenu(queryMenu, "&Query");

	wxMenu* renderMenu = new wxMenu();
		renderMenu->AppendRadioItem(rqtVIEW_WIRE, "Wireframe");
		renderMenu->AppendRadioItem(rqtVIEW_SHADED, "Shaded");
		renderMenu->Check(rqtVIEW_SHADED, true);

	wxMenu* helpMenu = new wxMenu();
		helpMenu->Append(wxID_ABOUT , "About...");

	wxMenu* viewMenu = new wxMenu();
	viewMenu->Append(rqtVIEW_RESET , "Reset View");
	viewMenu->Append(rqtVIEW_ZOOM_IN , "Zoom In");
	viewMenu->Append(rqtVIEW_ZOOM_OUT , "Zoom Out");
	viewMenu->AppendSeparator();
	viewMenu->AppendCheckItem(rqtVIEW_LAST_QUERY , "Last Query");
	viewMenu->Check(rqtVIEW_LAST_QUERY , true);
	viewMenu->AppendSeparator();
		viewMenu->AppendSubMenu(renderMenu , "&Render");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(treeMenu , "&Tree");
	menuBar->Append(viewMenu , "&View");
	menuBar->Append(helpMenu , "&Help");

	SetMenuBar(menuBar);

}

void rqtMainFrame::CreateToolBar(){
	m_toolBar = new wxAuiToolBar(this , wxID_ANY);

	m_toolBar->AddTool(rqtINIT_TREE, "New", wxBitmap("content/init22.png", wxBITMAP_TYPE_PNG), "Create a new Quadtree");
	m_toolBar->AddTool(wxID_SAVE, "Save", wxBitmap("content/save22.png", wxBITMAP_TYPE_PNG), "Save the Quadtree");
	m_toolBar->AddTool(wxID_OPEN, "Open", wxBitmap("content/open22.png", wxBITMAP_TYPE_PNG), "Load a Quadtree from file");
	m_toolBar->AddSeparator();
	m_toolBar->AddTool(rqtCLEAR_TREE, "Clear", wxBitmap("content/clear22.png", wxBITMAP_TYPE_PNG), "Clear the Quadtree");
	m_toolBar->AddTool(rqtTRIM_TREE, "Trim", wxBitmap("content/trim22.png", wxBITMAP_TYPE_PNG) , "Trim the Quadtree");
	m_toolBar->AddSeparator();
	m_toolBar->AddTool(rqtVIEW_ZOOM_IN, "Zoom In", wxBitmap("content/zoomin22.png", wxBITMAP_TYPE_PNG) , "Zoom In");
	m_toolBar->AddTool(rqtVIEW_ZOOM_OUT, "Zoom Out", wxBitmap("content/zoomout22.png", wxBITMAP_TYPE_PNG), "Zoom Out");
	m_toolBar->AddSeparator();
	m_toolBar->AddTool(rqtTOOL_ITEM_SELECT, wxT("Select"), wxBitmap("content/select22.png", wxBITMAP_TYPE_PNG), "Item Selection Tool" , wxITEM_RADIO);
	m_toolBar->AddTool(rqtTOOL_ITEM_QUERY, wxT("Query"), wxBitmap("content/query22.png", wxBITMAP_TYPE_PNG), "Query Quadtree Tool" , wxITEM_RADIO);
	m_toolBar->AddTool(rqtTOOL_ITEM_INSERT, wxT("Insert"), wxBitmap("content/insert22.png", wxBITMAP_TYPE_PNG), "Item Insertion Tool" , wxITEM_RADIO);
	m_toolBar->AddTool(rqtTOOL_ITEM_TRANSLATE, wxT("Translate"), wxBitmap("content/translate22.png", wxBITMAP_TYPE_PNG), "Item Translation Tool" , wxITEM_RADIO);
	m_toolBar->ToggleTool(rqtTOOL_ITEM_SELECT, true);
	
	m_toolBar->Realize();
}

void rqtMainFrame::InitMainFrame(){
    	m_imgList = new wxImageList(16 ,16);
	m_imgList->Add(wxIcon("content/point16.png", wxBITMAP_TYPE_PNG));
	m_imgList->Add(wxIcon("content/rect16.png", wxBITMAP_TYPE_PNG));
	m_imgList->Add(wxIcon("content/circle16.png", wxBITMAP_TYPE_PNG));
	m_imgList->Add(wxIcon("content/level16.png", wxBITMAP_TYPE_PNG));


	m_statusBar = CreateStatusBar(4);
	m_glView = new rqtGLView(m_gameWorld , m_renderer, m_graphicsDevice, m_statusBar, this , rqtGL_VIEW);
	m_queryView = new rqtQueryViewWindow(this);

	m_objectBrowser = new rwxObjectBrowser(this , rqtOBJ_BROWSER);
        m_objectBrowser->SetImageList(m_imgList);
	m_propertyGrid= new rqtShapePropertyViewer(this , wxID_ANY);
}

void rqtMainFrame::InitAuiIbjects(){
	m_auiManager.AddPane(m_glView , wxAuiPaneInfo().Caption("GL View").Floatable(false).CenterPane().CloseButton(false));
	m_auiManager.AddPane(m_toolBar, wxAuiPaneInfo().Caption(wxT("Tool Bar")).Floatable(true).ToolbarPane().Top().GripperTop().CloseButton(false));
	m_auiManager.AddPane(m_propertyGrid , wxAuiPaneInfo().Caption("Item Properties").Floatable(true).Dockable().Right().Dock().Position(0).BestSize(wxSize(200,200)).CloseButton(false));
	m_auiManager.AddPane(m_objectBrowser , wxAuiPaneInfo().Caption("Object Browser").Floatable(true).Dockable().Right().Dock().Position(1).BestSize(wxSize(200,200)).CloseButton(false));
	m_auiManager.AddPane(m_queryView , wxAuiPaneInfo().Caption("Query Viewer").Floatable(true).Dockable().Bottom().Dock().Position(0).BestSize(wxSize(100,100)).CloseButton(false));
	m_auiManager.AddPane(m_logCtrl , wxAuiPaneInfo().Caption("Log").Floatable(true).Dockable().Bottom().Dock().Position(1).BestSize(wxSize(100,100)).CloseButton(false));
	
	//normally would need this but we will be setting the active tool shortly after so that will call the inital update
	//m_auiManager.Update();
}

void rqtMainFrame::OnInsertPoint(wxCommandEvent& event){
	rqtPointDialog dialog(this, wxID_ANY, "Insert Point");

	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString n = dialog.GetItemName();
	wxColor c = dialog.GetItemColor();
	rVector2 p = dialog.GetPoint();

	m_controller->CreatePointObject(n ,c, p);
}

void rqtMainFrame::OnInsertRectangle(wxCommandEvent& event){
	rqtRectangleDialog dialog(this, wxID_ANY, "Insert Rectangle");

	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString n = dialog.GetItemName();
	wxColor c = dialog.GetItemColor();
	rRectangle2 r = dialog.GetArea();

	m_controller->CreateRectangleObject(n,c,r);
}

void rqtMainFrame::OnInitTree(wxCommandEvent& event){
	CheckForUnsavedChanges();

	rqtInitTreeDialog dialog(this ,wxID_ANY, "Init Quadtree");

	if (dialog.ShowModal() != wxID_OK)
		return;

	rRectangle2 r = dialog.GetArea();
	int d = dialog.GetDepth();

	m_controller->InitQuadtree(r, d);
}

void rqtMainFrame::OnClearTree(wxCommandEvent& event){
	wxMessageDialog dialog(NULL , "Really Clear the tree?" , "Clear Quadtree", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

	if (dialog.ShowModal() != wxID_YES)
		return;

	m_controller->ClearQuadtree();
}

void rqtMainFrame::OnInsertCircle(wxCommandEvent& event){
	rqtCircleDialog dialog(this, wxID_ANY, "Insert Circle");

	if (dialog.ShowModal() != wxID_OK)
		return;

	wxString n = dialog.GetItemName();
	wxColor c = dialog.GetItemColor();
	rCircle2 circle = dialog.GetCircle();

	m_controller->CreateCircleObject(n,c,circle);
}

void rqtMainFrame::OnViewRenderMode(wxCommandEvent& event){
	switch (event.GetId()){
		case rqtVIEW_WIRE: 
			m_glView->SetRenderMode(rRENDER_WIRE);
			break;

		case rqtVIEW_SHADED: 
			m_glView->SetRenderMode(rRENDER_SHADED);
			break;
	};
	
}

void rqtMainFrame::OnQueryPoint(wxCommandEvent& event){
		rqtQueryPointDialog dialog(this , wxID_ANY , "Point Query");

	if (dialog.ShowModal() != wxID_OK)
		return;

	rVector2 p = dialog.GetPoint();
	m_controller->QueryPoint(p);
}

void rqtMainFrame::OnQueryRectangle(wxCommandEvent& event){
	rqtQueryRectangleDialog dialog(this , wxID_ANY , "Rectangle Query");

	if (dialog.ShowModal() != wxID_OK)
		return;

	rRectangle2 r = dialog.GetArea();
	m_controller->QueryRectangle(r);
}

void rqtMainFrame::OnQueryCircle(wxCommandEvent& event){
	rqtQueryCircleDialog dialog(this , wxID_ANY , "Circle Query");

	if (dialog.ShowModal() != wxID_OK)
		return;

	rCircle2 c = dialog.GetCircle();
	m_controller->QueryCircle(c);
}

void rqtMainFrame::CheckForUnsavedChanges(){
	if (!m_controller->IsModified())
		return;

	wxMessageDialog dialog(NULL ,  "Do you want to save your changes?", "Save Changes",wxYES_NO);

	if (dialog.ShowModal() == wxID_YES)
		m_controller->SaveTree();
}

void rqtMainFrame::OnExit(wxCommandEvent& event){
	Close(true);
}

void rqtMainFrame::OnFrameClose(wxCloseEvent& event){
	CheckForUnsavedChanges();
	event.Skip();
}

void rqtMainFrame::OnToolSelection(wxCommandEvent& event){
	SetActiveTool(event.GetId());
}

void rqtMainFrame::OnViewLastQuery(wxCommandEvent& event){
	m_glView->ShouldRenderQuery(event.IsChecked());
	m_glView->Refresh();
}

void rqtMainFrame::OnViewReset(wxCommandEvent& event){
	m_glView->ResetView();

}

void rqtMainFrame::OnViewZoom(wxCommandEvent& event){
	if (!m_gameWorld->IsInit())
		return;

	if (event.GetId() == rqtVIEW_ZOOM_IN)
		m_glView->IncrementWorldScale(-rqtGLView::rqtZOOM_STEP);
	else
		m_glView->IncrementWorldScale(rqtGLView::rqtZOOM_STEP);
}

void rqtMainFrame::OnHelpAbout(wxCommandEvent& event){
        wxAboutDialogInfo aboutInfo;
        aboutInfo.SetName("Quadtree Demo");
        aboutInfo.SetVersion("1.0");
        aboutInfo.SetDescription("Demo Showing quadtree class functionality");
        aboutInfo.SetCopyright("(C) 2011 Matthew LaRocca <mlarocca.cpp@gmail.com>");
        aboutInfo.SetWebSite("http://matthewcpp.wordpress.com/");

        wxAboutBox(aboutInfo);
}

void rqtMainFrame::SetActiveTool(int toolId){

	if (m_activeToolBar){
		m_activeToolBar->Hide();
		m_auiManager.DetachPane(m_activeToolBar);
	}

	m_toolManager->SetActiveTool(toolId);
	m_activeToolBar = m_toolManager->GetToolbar();
	m_activeToolBar->Show();
	m_auiManager.AddPane(m_activeToolBar, wxAuiPaneInfo().Caption(wxT("Tool Bar")).Floatable(true).ToolbarPane().Top().GripperTop().CloseButton(false).Position(1));
	m_controller->ClearQueryResults();
	m_glView->Refresh();

	m_auiManager.Update();

	wxLogMessage(m_toolManager->ActiveToolName() + " active.");
}

void rqtMainFrame::OnTrimTree(wxCommandEvent& event){
	m_controller->TrimQuadtree();
}

void rqtMainFrame::OnSave(wxCommandEvent& event){
	if (!m_gameWorld->IsInit())
		return;

	m_controller->SaveTree();
}

void rqtMainFrame::OnLoad(wxCommandEvent& event){

	wxFileDialog dialog(NULL, "Load quadtree", "", "", "Quadtree files (*.rqt)|*.rqt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if (dialog.ShowModal() != wxID_OK)
		return;

	CheckForUnsavedChanges();
	m_controller->LoadTree(dialog.GetPath());
}