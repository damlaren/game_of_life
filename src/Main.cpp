// wxWidgets GUI for game of life.
// Adapted from Image Panel example at: https://wiki.wxwidgets.org/An_image_panel

#include "BasicBoard.h"

#include <wx/wx.h>
#include <wx/sizer.h>

/**
 * Image panel to draw a bitmap depicting current state of GOL.
 */
class wxImagePanel : public wxPanel
{
    wxBitmap resized;
    int w, h;
    Board *mBoard;

public:
    wxImagePanel(Board *board, wxFrame* parent, wxString file, wxBitmapType format);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
    EVT_PAINT(wxImagePanel::paintEvent) // catch paint events
    EVT_SIZE(wxImagePanel::OnSize)      // size event
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(Board* board, wxFrame* parent, wxString file, wxBitmapType format) :
wxPanel(parent)
{
    w = -1;
    h = -1;

    mBoard = board;
}

/*
* Callback to redraw panel on paint events.
*/
void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Force immediate redraw.
 */
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/**
 * Callback to render current GOL state.
 */
void wxImagePanel::render(wxDC&  dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    int boardWidth, boardHeight;
    const char* bits = mBoard->getBitmap(boardWidth, boardHeight);

    //resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
    resized = wxBitmap(wxBitmap(bits, boardWidth, boardHeight).ConvertToImage().Scale(neww, newh));
    w = neww;
    h = newh;
    dc.DrawBitmap(resized, 0, 0, false);
}

/**
 * Event handler for resizing event.
 */
void wxImagePanel::OnSize(wxSizeEvent& event){
    Refresh();
    //skip the event.
    event.Skip();
}

/**
 * Button IDs
 */
enum
{
    BUTTON_TICK = wxID_HIGHEST + 1
};

/**
 * A frame to collect all window elements together.
 */
class GOLFrame : public wxFrame
{
protected:
    wxImagePanel* drawPane;
    Board* mBoard;

public:
    GOLFrame(Board *board, wxWindow *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, id, title, pos, size)
    {
        mBoard = board;
    }

    bool initialize()
    {
        // make sure to call this first
        wxInitAllImageHandlers();

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

        drawPane = new wxImagePanel(mBoard, this, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
        sizer->Add(drawPane, 1, wxEXPAND);

        // add buttons to control application
        wxButton *tickButton = new wxButton(this, BUTTON_TICK, _T("Tick"), wxDefaultPosition, wxDefaultSize, 0);
        sizer->Add(tickButton, 1, wxEXPAND);

        this->SetSizer(sizer);

        this->Show();
        return true;
    }

    /// Move to next step of simulation
    void tick()
    {
        mBoard->update();
        drawPane->Refresh();
        drawPane->Update();
    }

    void OnTickClick(wxCommandEvent& event)
    {
        tick();
    }

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(GOLFrame, wxFrame)
    EVT_BUTTON(BUTTON_TICK, GOLFrame::OnTickClick)
END_EVENT_TABLE()

/**
 * Application to launch everything.
 */
class GOLApp : public wxApp
{
protected:
    GOLFrame *frame;
    Board *board;

public:
    bool OnInit()
    {
        // make a test board
        board = new BasicBoard(100, 100);
        board->loadBoard("../../input/glider.txt");
        //board->loadBoard("../../input/nova.txt");

        frame = new GOLFrame(board, NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));
        return frame->initialize();
    }
};

IMPLEMENT_APP(GOLApp)