#include "ToolWindow.h"

ToolWindow::ToolWindow(wxWindow* parent, wxWindowID id, ToolType type, const wxPoint& pos, const wxSize& size)
	: SelectableWindow(parent, id, pos, size), toolType(type)
{
}

void ToolWindow::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
    gc->SetPen(*wxWHITE_PEN);
    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

    double itemWidth = rect.GetWidth() / 2.0;

    wxAffineMatrix2D transform{};
    transform.Translate(rect.GetX(), rect.GetY());
    transform.Translate(rect.GetWidth() / 2, rect.GetHeight() / 2);

    gc->PushState();
    gc->SetTransform(gc->CreateMatrix(transform));

    const auto color = wxColor(80, 80, 80);

    gc->SetPen(wxPen(color, FromDIP(3)));
    gc->SetBrush(wxBrush(color));


    //TODO: Update these to work on bitmaps. Instead of setting up points etc, we will be handling pictures bmp
    //      Bitmaps used here will be the icon bitmaps, but for the use in the program, we will need bit sharper bmp's
    switch (toolType)
    {
    case ToolType::Pen:
        // draw sine wave
    {
        std::vector<wxPoint2DDouble> points{};
        constexpr int Resolution = 50;

        for (int i = 0; i < Resolution; i++)
        {
            double x = (i / static_cast<double>(Resolution) - 0.5) * itemWidth;
            double y = sin(x / itemWidth * M_PI * 2.0) * itemWidth / 2.0;
            points.push_back(wxPoint2DDouble(x, y));
        }
        gc->Rotate(M_PI / 4.0);
        gc->StrokeLines(points.size(), points.data());
        break;
    }
    case ToolType::Rect:
        gc->DrawRectangle(-itemWidth / 2, -itemWidth / 2, itemWidth, itemWidth);
        break;
    case ToolType::Circle:
        gc->DrawEllipse(-itemWidth / 2, -itemWidth / 2, itemWidth, itemWidth);
        break;
    case ToolType::Transform:
    {
        std::array<wxPoint2DDouble, 8> points = { {{0, -itemWidth * 2 / 3},
                                                  {-itemWidth * 5 / 12, 0},
                                                  {-itemWidth / 8, -itemWidth / 12},
                                                  {-itemWidth / 8, itemWidth * 2 / 3},
                                                  {itemWidth / 8, itemWidth * 2 / 3},
                                                  {itemWidth / 8, -itemWidth / 12},
                                                  {itemWidth * 5 / 12, 0},
                                                  {0, -itemWidth * 2 / 3}} };

        gc->Rotate(-M_PI / 4.0);
        gc->StrokeLines(points.size(), points.data());
        break;
    }
    }

    gc->PopState();
}
