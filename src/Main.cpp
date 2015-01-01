// Based on Image Panel example at: https://wiki.wxwidgets.org/An_image_panel

#include "BasicBoard.h"

#include <wx/wx.h>
#include <wx/sizer.h>

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
    //board->loadBoard("../../input/glider.txt");
    board->loadBoard("../../input/nova.txt");
}

void wxImagePanel::mouseDown(wxMouseEvent& event)
{
    board->update();
    Refresh();
    Update();
}

/*
* Called by the system of by wxWidgets when the panel needs
* to be redrawn. You can also trigger this call by
* calling Refresh()/Update().
*/

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
* Alternatively, you can use a clientDC to paint on the panel
* at any time. Using this generally does not free you from
* catching paint events, since it is possible that e.g. the window
* manager throws away your drawing when the window comes to the
* background, and expects you will redraw it when the window comes
* back (by sending a paint event).
*/
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
* Here we do the actual rendering. I put it in a separate
* method so that it can work no matter what type of DC
* (e.g. wxPaintDC or wxClientDC) is used.
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

/*
* Here we call refresh to tell the panel to draw itself again.
* So when the user resizes the image panel the image should be resized too.
*/
void wxImagePanel::OnSize(wxSizeEvent& event){
    Refresh();
    //skip the event.
    event.Skip();
}

// ----------------------------------------
// how-to-use example

class MyApp : public wxApp
{

    wxFrame *frame;
    wxImagePanel * drawPane;
public:
    bool OnInit()
    {
        // make sure to call this first
        wxInitAllImageHandlers();

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        frame = new wxFrame(NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));

        // then simply create like this
        drawPane = new wxImagePanel(frame, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
        sizer->Add(drawPane, 1, wxEXPAND);

        frame->SetSizer(sizer);

        frame->Show();
        return true;
    }

};

IMPLEMENT_APP(MyApp)