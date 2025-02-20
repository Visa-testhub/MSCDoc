#pragma once
#include <wx/wx.h>
#include <wx/docview.h>
#include "Frame.h"
#include "View.h"

class MSCDocApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();

    static void SetUpCanvasForView(View* view);
private:
    //This prolly handles the saving loading functionality
    wxDocManager* docManager;
    Frame* frame = nullptr;
};