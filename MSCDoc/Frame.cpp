#include "Frame.h"
#include <wx/persist/toplevel.h>

Frame::Frame(wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxDocParentFrame(manager, frame, id, title, pos, size)
{
    // Splitter window will hold the view and panel.
    splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
    splitter->SetMinimumPaneSize(FromDIP(220));

    // Set up toolpanel for the left side of the splitter.
    SetUpToolPanel(splitter);
    
    // Panel for the right side of splitterwindow
    docPanel = new wxPanel(splitter, wxID_ANY);
    docPanel->SetSizer(new wxBoxSizer(wxVERTICAL));

    // Split the window
    splitter->SplitVertically(toolPanel, docPanel);
    splitter->SetSashPosition(FromDIP(240));

    // Handles going back to the size the frame was when closed
    if (!wxPersistentRegisterAndRestore(this, title))
    {
        SetClientSize(FromDIP(wxSize(800, 600)));
    }

    // Set up the menubar at the top. Info on what this does inside the function.
    SetUpMenuBar();
}

void Frame::CreateDocument()
{
}

void Frame::SetUpMenuBar()
{
    // Create the actual instance of menubar. 
    // Note: wxWidgets handles deleting its variables on program closing
    wxMenuBar* menuBar = new wxMenuBar;

    // Create file menu and append it to menubar.
    // Menubar is horizontal(default) and the things we append to it will
    // be one after one 
    wxMenu* fileMenu = new wxMenu;
    menuBar->Append(fileMenu, "&File");

    // The new an open automatically makes new and open menu which have menu picker to
    // open the wanted menu. These are important since on the new or open command the
    // docManager creates (onCreate) the dynamically implemented classes wxWiev and wxDocument
    fileMenu->Append(wxID_NEW, "New Document");
    fileMenu->Append(wxID_OPEN, "Open document");
    fileMenu->Append(wxID_SAVE);
    fileMenu->Append(wxID_SAVEAS);
    fileMenu->Append(wxID_CLOSE);
    fileMenu->Append(wxID_EXIT);
    
    // If wanted, wx has ready made functionality for these wxID_UNDO wxID_REDO
    // They have to be derived from the base class and implemented to work how wanted.
    // There is also wxID_COPY wxID_PASTE, wxID_CUT, wxID_DELETE
    // Also those have to be implemented. There exists some functionality already but its very basic
    // 
    // Functionality exist for recent menu and it can be implemented like this.
    // Make new wxMenu, give it a menuID that you save in i.e Frame
    // Append the menu to i.e. filemenu
    // Tell document manager that the menu you made is the recent files menu like this.
    // GetDocumentManager()->FileHistoryUseMenu(theMenuYouMade)
    // GetDocumentManager()->FileHistoryLoad(*wxConfig::Get());
    // You can empty the history with
    // GetDocumentManager()->GetFileHistory()->RemoveFileFromHistory(TheFileOfChoosing i.e. 0);

    //This shows the menubar.
    SetMenuBar(menuBar);
}

void Frame::SetUpToolPanel(wxWindow* parent)
{
    toolPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
    toolPanel->SetScrollRate(0, FromDIP(8));
    toolPanel->SetBackgroundColour(wxColour(darkBackgroundColour));


    // Add a main sizer. This will be vertically and for other content like selection panes
    // we throw them inside horizontal sizer which then goes inside this main vertical sizer.
    mainSizer = new wxBoxSizer(wxVERTICAL);
    

    //Create title for the colour
    wxStaticText* colourLabel = new wxStaticText(toolPanel, wxID_ANY, "Colour");
    colourLabel->SetForegroundColour(wxColour("#ffffff"));
    //Add it to the main sizer.
    mainSizer->Add(colourLabel, 0, wxALIGN_LEFT, FromDIP(4));

    //Create colour picker.
    colourPicker = new wxColourPickerCtrl(toolPanel,wxID_ANY);
    //Bind the onColourChanged event to it.
    this->Bind(wxEVT_COLOURPICKER_CHANGED, &Frame::OnColourChanged, this);
    //Add this picker in to the main sizer.
    mainSizer->Add(colourPicker, wxALIGN_LEFT, FromDIP(5));
    //Set minimum size for the picker so we can always see it.
    colourPicker->SetMinSize(wxSize(FromDIP(80), FromDIP(40)));

    
    // Add title for the different tools
    wxStaticText * toolLabel = new wxStaticText(toolPanel, wxID_ANY, "Tool");
    toolLabel->SetForegroundColour("#ffffff");
    mainSizer->Add(toolLabel, 0, wxALL, FromDIP(5));
    
    // Horizontal sizer for tools so that they expand sideways.
    wxWrapSizer* toolSizer = new wxWrapSizer(wxHORIZONTAL);
    mainSizer->Add(toolSizer, 0, wxALL, FromDIP(5));
    
    // Create window for every tooltype. Create vector that can be used to roll through them if something is clicked later.
    // Selecting the tool is implemented here on the OnToolWindowClicked and the icons are done in the toolWindow class.
    // TODO: change for loop to automatically go through every tooltype without needing to add the tooltype here.
    for (const ToolType toolType : { ToolType::Pen, ToolType::Rect, ToolType::Circle, ToolType::Text, ToolType::Transform })
    {
        ToolWindow* toolWindow = new ToolWindow(toolPanel, wxID_ANY, toolType);
        toolWindow->Bind(wxEVT_LEFT_DOWN, &Frame::OnToolWindowClicked, this);
        toolWindows.push_back(toolWindow);
        toolSizer->Add(toolWindow, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
    }



    // Add title for the pen width slider
    wxStaticText* penWidthLabel = new wxStaticText(toolPanel, wxID_ANY, "Penwidth");
    penWidthLabel->SetForegroundColour("#ffffff");
    mainSizer->Add(penWidthLabel, 0, wxALL, FromDIP(5));

    // Create pen width slider and box sizer. Box sizer is added so that it can be hidden if pen is not chosen.
    wxSlider* penWidthSlider = new wxSlider(toolPanel, wxID_ANY, 1, 1, 20, wxDefaultPosition, wxDefaultSize, wxSL_MIN_MAX_LABELS | wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_VALUE_LABEL | wxSL_TOP, wxDefaultValidator, "Pen size");
    penWidthSlider->SetMinSize(wxSize(FromDIP(180), FromDIP(40)));
    wxBoxSizer* penWidthSizer = new wxBoxSizer(wxVERTICAL);
    penWidthSizer->Add(penWidthSlider, 0, wxALL, FromDIP(5));
    mainSizer->Add(penWidthSizer, 0, wxALL, FromDIP(5));
    
    //TODO: find a way to change the colour of the texts of the slider.
    //penWidthSlider->UseBackgroundColour();
    
    //TODO: bind the changing of the pen width to change the pen width in tool settings.
    toolPanel->Bind(wxEVT_SCROLL_CHANGED, &Frame::OnPenWidthChanged, this);

    toolPanel->SetSizer(mainSizer);
    return ;
}

void Frame::SetUpCanvasForView(View* view)
{
    if (docPanel->GetChildren().size() > 0)
    {
        docPanel->GetSizer()->Clear(true);
    }

    if (view != nullptr)
    {
        //in here we add new canvas and the size will be default size
        Canvas* canvas = new Canvas(docPanel, view, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(1000), FromDIP(1000)));

        //This part makes the sizer in docPanel to change the size of the canvas according to the limits of the window
        //DocPanel was of type wxScrolled<wxPanel> (underlying wxPanel wrapped in the wxScrolled)
        // TEST docPanel->GetScrollHelper()->add

        //docPanel->GetSizer()->Add(canvas, 1, wxEXPAND);

    //    view->SetFrame(this);
    }
    else
    {
    //    this->SetTitle(wxGetApp().GetAppDisplayName());
    }

    docPanel->Layout();
}


void Frame::OnToolWindowClicked(wxMouseEvent& evt)
{
    ToolWindow* toolWindow = dynamic_cast<ToolWindow*>(evt.GetEventObject());
    
    for (ToolWindow* currentTool : toolWindows)
    {
        currentTool->selected = currentTool == toolWindow;
        currentTool->Refresh();
    }

    if (toolWindow->toolType == ToolType::Pen)
    {
        //TODO: If tooltype is Pen, show the sizer which has the pen sizes in it
        //Else, hide it. 
    }
}

// Called when the user has picked a new colour with colourpicker.
void Frame::OnColourChanged(wxColourPickerEvent& evt)
{
    selectedColour = evt.GetColour();
}

void Frame::OnPenWidthChanged(wxScrollEvent& evt)
{
    evt.GetInt();
}
