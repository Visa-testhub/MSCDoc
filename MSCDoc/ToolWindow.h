#pragma once

#include <array>

#include "SelectableWindow.h"
#include "ToolType.h"


class ToolWindow : public SelectableWindow
{
public:
    ToolWindow(wxWindow* parent, wxWindowID id = wxID_ANY, ToolType type = ToolType::Pen, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    ToolType toolType;

private:
    virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};

