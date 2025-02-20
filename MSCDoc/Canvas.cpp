#include "Canvas.h"

Canvas::Canvas(wxWindow* parent, View* view, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size), view(view)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	//wxEVT_PAINT notifies when windows contents needs to be repainted, Bind it to OnPaint func
	this->Bind(wxEVT_PAINT, &Canvas::OnPaint, this);
}

wxRect Canvas::GetCanvasBounds() const
{
	return wxRect(FromDIP(50), FromDIP(50), FromDIP(500), FromDIP(300));
}

void Canvas::OnPaint(wxPaintEvent&)
{
	//When using autobuffered paint, the backgroundStyle must be set with wxBG_STYLE_PAINT
	//AutobufferedPaint won't double buffer on platforms with native double buffering.
	wxAutoBufferedPaintDC deviceContext(this);

	if (view)
	{
		//OnDraw overridden on view
		view->OnDraw(&deviceContext);
	}
}

void Canvas::OnMouseDown(wxMouseEvent&)
{

}
