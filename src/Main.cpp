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
    Board *board;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    void mouseDown(wxMouseEvent& event);

    void loadBoard(const char* fileName);

    // some useful events
    /*
    void mouseMoved(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    */

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
// some useful events
/*
EVT_MOTION(wxImagePanel::mouseMoved)
EVT_LEFT_UP(wxImagePanel::mouseReleased)
EVT_RIGHT_DOWN(wxImagePanel::rightClick)
EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
EVT_KEY_DOWN(wxImagePanel::keyPressed)
EVT_KEY_UP(wxImagePanel::keyReleased)
EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
*/

// catch paint events
EVT_PAINT(wxImagePanel::paintEvent)
//Size event
EVT_SIZE(wxImagePanel::OnSize)
//TODO for now click to update
EVT_LEFT_DOWN(wxImagePanel::mouseDown)
END_EVENT_TABLE()


// some useful events
/*
void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
void wxImagePanel::rightClick(wxMouseEvent& event) {}
void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
void wxImagePanel::keyPressed(wxKeyEvent& event) {}
void wxImagePanel::keyReleased(wxKeyEvent& event) {}
*/

wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
wxPanel(parent)
{
    w = -1;
    h = -1;

    // make a test board
    board = new BasicBoard(100, 100);
    board->loadBoard("../../input/glider.txt");
    //board->loadBoard("../../input/nova.txt");
}

void wxImagePanel::mouseDown(wxMouseEvent& event)
{
    board->update();
    Refresh();
    Update();
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
    const char* bits = board->getBitmap(boardWidth, boardHeight);

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
 * A frame to collect all window elements together.
 */
class GOLFrame : public wxFrame
{
protected:
    wxImagePanel * drawPane;

public:
    // Button IDs
    enum
    {
        BUTTON_TICK = wxID_HIGHEST + 1
    };

    GOLFrame(wxWindow *parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, id, title, pos, size)
    {
    }

    bool OnInit()
    {
        // make sure to call this first
        wxInitAllImageHandlers();

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        
        drawPane = new wxImagePanel(this, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
        sizer->Add(drawPane, 1, wxEXPAND);

        // add buttons to control application
        //wxButton *tickButton =

        this->SetSizer(sizer);

        this->Show();
        return true;
    }
};

/**
 * Application to launch everything.
 */
class GOLApp : public wxApp
{
protected:
    GOLFrame *frame;

public:
    bool OnInit()
    {
        frame = new GOLFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));
        return frame->OnInit();
    }
};

IMPLEMENT_APP(GOLApp)