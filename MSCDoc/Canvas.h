#pragma once

#include <wx/window.h>
#include <wx/dcbuffer.h>
#include "View.h"

class Canvas : public wxWindow
{
public:
	Canvas(wxWindow* parent, View* view, wxWindowID id, const wxPoint& pos, const wxSize& size);
	virtual ~Canvas() noexcept {};

    void SetView(View* view);
    wxRect GetCanvasBounds() const;

    // If zoom implemented at somepoint
    double GetCanvasScale() const { return std::pow(ZOOM_FACTOR, zoomLevel); }

private:

    View* view;
    bool isDragging{ false };

    //if zoom implemented at some point
    const double ZOOM_FACTOR = 1.1;
    int zoomLevel = 0;
    //wxMenu contextMenu;

    void OnPaint(wxPaintEvent&);
    // Not found in the cpp
   // void DrawOnContext(wxGraphicsContext* gc);  

    void OnMouseDown(wxMouseEvent&);
    void OnMouseMove(wxMouseEvent&);
    void OnMouseUp(wxMouseEvent&);
    void OnMouseLeave(wxMouseEvent&);


    void BuildContextMenu();
    void OnContextMenuEvent(wxContextMenuEvent&);
};

