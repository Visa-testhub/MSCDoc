#include <wx/wx.h>
#include "MSCDoc.h"
#include "Document.h"
#include "View.h"
#include "Frame.h"

wxIMPLEMENT_APP(MSCDocApp);

bool MSCDocApp::OnInit()
{
	wxInitAllImageHandlers();

	wxString appName("Machine stability control documentor");
	SetAppName(appName);
	SetAppDisplayName(appName);

	docManager = new wxDocManager();
	docManager->SetMaxDocsOpen(1);

	// The docTemplate handles creating the Document and View classes and calls onCreate on them when user
	// Opens existing or new document. The save and load objects in Document gets called too on user buttonclick
	new wxDocTemplate(docManager->GetDocumentManager(), "MSCDocument", "*.pxz", "", "pxz", "Document", "View", CLASSINFO(Document), CLASSINFO(View));

	frame = new Frame(docManager, nullptr, wxID_ANY, appName);
	frame->Show(true);
	
	return true;
}

int MSCDocApp::OnExit()
{
	//any application wide cleanup code here
	return 0;
}

void MSCDocApp::SetUpCanvasForView(View* view)
{
	wxGetApp().frame->SetUpCanvasForView(view);
}
