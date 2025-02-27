#pragma once

#include <wx/docview.h>
#include <vector>
//#include "MSCDoc.h"
#include "Document.h"



class View : public wxView
{
public:
	bool OnCreate(wxDocument* document, long flags) override;
	bool OnClose(bool deleteWindow = true) override;
	void OnDraw(wxDC* deviceContext) override;

	
	Document* GetDocument() const;

	//wxWidgets internally expects document and view classes to have dynamic class capabilities.
	wxDECLARE_DYNAMIC_CLASS(View);
};

