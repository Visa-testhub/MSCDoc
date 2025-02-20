#include "View.h"

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
    MSCDocApp::SetUpCanvasForView(this);

    return true;
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
        Document* doc = wxStaticCast(wxView::GetDocument(), Document);
        
        std::vector<std::unique_ptr<Element>> vector = doc->elements;
        
        for (const auto& element : vector)
        {

        }
    }
}
