#pragma once

#include <wx/window.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class SelectableWindow :public wxWindow
{
public:
	SelectableWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	wxSize DoGetBestSize() const override;

	bool selected = false;

protected:
	virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const = 0;

private:
	void OnPaint(wxPaintEvent& event);

};

