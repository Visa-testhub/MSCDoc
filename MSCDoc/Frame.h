#pragma once

#include <wx/wx.h>
#include <wx/docview.h>
#include <wx/bitmap.h>
#include <wx/splitter.h>
#include <wx/wrapsizer.h>
#include <wx/clrpicker.h>
#include <random>

#include "View.h"
#include "ToolType.h"
#include "ToolWindow.h"
#include "Canvas.h"

class Frame : public wxDocParentFrame
{
public:
    //Constructor that sets up the frame
    Frame(wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void SetUpCanvasForView(View* view);

private:

    /* 
    ** This window (splitter) will split the frame in two and will be on top of the A4canvas and tool panel
    ** Under the splitter will be the (toolPanel) which will hold the windows for tools.
    ** Also the canvas that can be drawn upon will be under the (splitter)
    */
    wxSplitterWindow*       splitter = nullptr;

    wxScrolled<wxPanel>*        toolPanel = nullptr;
    wxBoxSizer*                 mainSizer = nullptr;
    wxColourPickerCtrl*         colourPicker = nullptr;
    std::vector<ToolWindow*>    toolWindows;

    wxPanel* docPanel = nullptr;

    // For the rectangles and their random generation. If the randomGen gets removed, the <random> can go too.
    int rectCount = 0;
    std::mt19937 randomGen;

    // Context menu on the top
    wxMenuBar* topMenuBar = nullptr;
    wxMenu* actionsMenu = nullptr;


    wxString darkBackgroundColour = "#2c2828";
    wxColour selectedColour = "#000000";

    //Coming soon. These are the elements that can be moved around.


    //Setting stuff up

    void CreateDocument(); // Initializes the document. Controlled calls only.
    void SetUpMenuBar();//No additional calls
    void SetUpToolPanel(wxWindow* parent);


    // On event happening

    void OnToolWindowClicked(wxMouseEvent& evt);
    void OnColourChanged(wxColourPickerEvent& evt);
    void OnPenWidthChanged(wxScrollEvent& evt);
    //void NewDocumentButtonClick(wxCommandEvent&);
    //void LoadDocumentButtonClick(wxCommandEvent& evt);
    //void OnAddButtonClick(wxCommandEvent& event);
    //void OnRemoveButtonClick(wxCommandEvent& event);
};

enum MenuButtonID
{
    NewDocument = 10,
    LoadDocument
};