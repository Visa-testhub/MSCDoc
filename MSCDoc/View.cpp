#include "View.h"
#include "MSCDoc.h"

// wxWidgets internally expects document and view classes to have dynamic class capabilities.
wxIMPLEMENT_DYNAMIC_CLASS(View, wxView);

bool View::OnCreate(wxDocument* doc, long flags)
{
    if (!wxView::OnCreate(doc, flags))
    {
        return false;
    }

    // Little loop here. We go through the App because it has pointer to the frame.
    // In frame we have everything else.
    MSCDocApp::InitializeCanvas(this);

    return true;
}

bool View::OnClose(bool deleteWindow)
{
    if (deleteWindow)
    {
        MSCDocApp::InitializeCanvas(nullptr);
    }
    return wxView::OnClose(deleteWindow);
}

void View::OnDraw(wxDC* dc)
{
    dc->SetBackground(*wxWHITE_BRUSH);
    dc->Clear();


    std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::CreateFromUnknownDC(*dc));


    // Here we draw all the elements to the canvas.
    if (gc)
    {
        //Cast should be pretty safe since the wxDocument this creates is created in the Document class.
        Document* doc = wxStaticCast(this->GetDocument(), Document);
        
        std::vector<PaintableElement>::iterator it = doc->paintableElements.begin();
		while (it != doc->paintableElements.end())
		{
			it->Draw(gc.get());
			it++;
		}
    }
}

Document* View::GetDocument() const
{
    return wxStaticCast(wxView::GetDocument(), Document);
}