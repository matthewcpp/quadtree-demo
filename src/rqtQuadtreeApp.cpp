#include "rqtQuadtreeApp.hpp"

IMPLEMENT_APP(rqtQuadtreeApp)

bool rqtQuadtreeApp::OnInit(){
        SetAppName("Quadtree Demo");
    
	wxImage::AddHandler(new wxPNGHandler());

	rqtMainFrame* mainFrame = new rqtMainFrame(NULL , wxID_ANY , "Quadtree Demo", wxPoint(0,0) , wxSize(1024,768));
        mainFrame->SetIcon(wxIcon("content/tree16.png", wxBITMAP_TYPE_PNG));
	mainFrame->Show(true);

	return true;
}