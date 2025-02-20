#include "SelectableWindow.h"

SelectableWindow::SelectableWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &SelectableWindow::OnPaint, this);
}

wxSize SelectableWindow::DoGetBestSize() const
{
    return FromDIP(wxSize(45, 45));
}

void SelectableWindow::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(this->GetParent()->GetBackgroundColour()));
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc)
    {
        wxRect selectionRect{ 0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight() };
        selectionRect.Deflate(FromDIP(1));

        wxRect contentRect = selectionRect;
        contentRect.Deflate(FromDIP(2));

        const int roundness = FromDIP(4);

        // Here we draw the individual content in each window.
        DrawContent(gc, contentRect, roundness);

        if (selected)
        {
            gc->SetPen(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE_PEN : *wxBLACK_PEN);
            gc->SetBrush(*wxTRANSPARENT_BRUSH);

            gc->DrawRoundedRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), selectionRect.GetHeight(), roundness);
        }

        delete gc;
    }
}