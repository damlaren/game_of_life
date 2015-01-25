// wxWidgets GUI for game of life.
// Adapted from Image Panel example at: https://wiki.wxwidgets.org/An_image_panel

#include "BasicBoard.h"
#include "SparseBoard.h"
#include <algorithm>
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/dcbuffer.h>

/**
 * Image panel to draw a bitmap depicting current state of GOL.
 */
class wxImagePanel : public wxPanel
{
protected:
    wxBitmap mBitmap; /// bitmap to display the board
    int mPanelWidth, mPanelHeight; /// width and height of image panel on window
    Board *mBoard; /// Handle to GOL Board to draw
    int mDisplayWidth; /// Number of cells to draw, side to side
    int mDisplayHeight; /// Number of cells to draw, top to bottom
    int mRowOffset; /// Row at which to start display
    int mColumnOffset; /// Column at which to start display

public:
    static const int INIT_DISPLAY_WIDTH = 64; /// Initial width of display
    static const int INIT_DISPLAY_HEIGHT = 64; /// Initial height of display
    static const int MAX_DISPLAY_WIDTH = 1024; /// Max # cells to draw side-side
    static const int MIN_DISPLAY_WIDTH = 16; /// Min # cells to draw side-side

    wxImagePanel(Board *board, wxFrame* parent, wxString file, wxBitmapType format);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
	void sizeEvent(wxSizeEvent& evt);
	void eraseEvent(wxEraseEvent& evt);
    void render(wxDC& dc);

    void zoomOut();
    void zoomIn();
    void changeRow(int r);
    void changeColumn(int c);

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(wxImagePanel::paintEvent) // catch paint events
EVT_SIZE(wxImagePanel::sizeEvent)      // size event
EVT_ERASE_BACKGROUND(wxImagePanel::eraseEvent) // background-erase event
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(Board* board, wxFrame* parent, wxString file, wxBitmapType format) :
wxPanel(parent)
{
    mPanelWidth = -1;
    mPanelHeight = -1;

    mBoard = board;

    mDisplayWidth = INIT_DISPLAY_WIDTH;
    mDisplayHeight = INIT_DISPLAY_HEIGHT;
    mRowOffset = 0;
    mColumnOffset = 0;
}

/*
* Callback to redraw panel on paint events.
*/
void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
	wxBufferedPaintDC dc(this);
    render(dc);
}

/*
 * Force immediate redraw.
 */
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(wxBufferedDC(&dc));
}

/**
 * Callback to render current GOL state.
 */
void wxImagePanel::render(wxDC&  dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    // keep the cells square, not stretched out
    neww = std::min(neww, newh);
    newh = std::min(neww, newh);

    int boardWidth = mDisplayWidth, boardHeight = mDisplayHeight;
    const char* bits = (const char*)mBoard->getBitmap(mRowOffset, mColumnOffset, boardWidth, boardHeight);

    mBitmap = wxBitmap(wxBitmap(bits, boardWidth, boardHeight).ConvertToImage().Scale(neww, newh));
    mPanelWidth = neww;
    mPanelHeight = newh;
    dc.DrawBitmap(mBitmap, 0, 0, false);

    delete bits;
}

/**
 * Event handler for resizing event.
 */
void wxImagePanel::sizeEvent(wxSizeEvent& evt)
{
    Refresh();
    //skip the event.
    evt.Skip();
}

/**
 * Event handler for erase event. This event needs to caught and ignored to prevent flickering on draw calls.
 */
void wxImagePanel::eraseEvent(wxEraseEvent& evt)
{
}

void wxImagePanel::zoomOut()
{
    if (mDisplayWidth < MAX_DISPLAY_WIDTH)
    {
        mDisplayHeight *= 2;
        mDisplayWidth *= 2;
    }
}

void wxImagePanel::zoomIn()
{
    if (mDisplayWidth > MIN_DISPLAY_WIDTH)
    {
        mDisplayHeight /= 2;
        mDisplayWidth /= 2;
    }
}

void wxImagePanel::changeRow(int row)
{
    mRowOffset = row;
}

void wxImagePanel::changeColumn(int column)
{
    mColumnOffset = column;
}

/**
 * Button IDs
 */
enum
{
    BUTTON_TICK = wxID_HIGHEST + 1,
    BUTTON_PLAY = wxID_HIGHEST + 2,
    BUTTON_OUTPUT = wxID_HIGHEST + 3,
    BUTTON_ZOOM_OUT = wxID_HIGHEST + 4,
    BUTTON_ZOOM_IN = wxID_HIGHEST + 5,
    TEXT_ROW = wxID_HIGHEST + 6,
    TEXT_COLUMN = wxID_HIGHEST + 7
};

/**
 * This frame basically runs the app and ties everything together.
 */
class GOLFrame : public wxFrame
{
protected:
    /**
    * Timer that fires once every second when play button is clicked
    * to tick simulation forward.
    */
    class PlayTimer : public wxTimer
    {
    private:
        GOLFrame *mFrame;

    public:
        PlayTimer(GOLFrame *mFrame);
        void Notify();
    };

    static const int TICK_TIME = 25; /// Time between ticks when playing, in ms.

    wxImagePanel *mDrawPane; /// Panel for drawing cells
    PlayTimer *mTimer; /// Timer for play button
    Board *mBoard; /// Handle to GOL Board

	// GUI elements
	wxBoxSizer *mButtonsSizer; /// Sizer to hold all other sizers for buttons
	wxBoxSizer *mPositionControlSizer; /// Sizer for position controls
	wxTextCtrl *mRowTextEntry; /// Text entry for row to view in simulation
	wxTextCtrl *mColumnTextEntry; /// Text entry for column to view
	wxButton *mZoomOutButton; /// Button to zoom out simulation view
	wxButton *mZoomInButton; /// Button to zoom in simulation view
	wxBoxSizer *mSimControlSizer; /// Sizer for simulation controls (tick/play/etc.)

	wxButton *mOutputButton; /// Button to output file
	wxButton *mTickButton; /// Button to tick simulation forward one step
	wxButton *mPlayButton; /// Button to play simulation continously

public:
    GOLFrame(Board *board, wxWindow *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, id, title, pos, size)
    {
        mBoard = board;
        mTimer = new PlayTimer(this);
    }

	~GOLFrame()
	{
		mTimer->Stop();
		delete mTimer;
	}

    bool initialize()
    {
        wxInitAllImageHandlers();

		mButtonsSizer = new wxBoxSizer(wxVERTICAL);
        mDrawPane = new wxImagePanel(mBoard, this, wxT("image.jpg"), wxBITMAP_TYPE_JPEG);
		mButtonsSizer->Add(mDrawPane, 20, wxEXPAND);

        // Add position controls
        mPositionControlSizer = new wxBoxSizer(wxHORIZONTAL);
        mRowTextEntry = new wxTextCtrl(this, TEXT_ROW, "0",
            wxDefaultPosition, wxDefaultSize, 0);
        mColumnTextEntry = new wxTextCtrl(this, TEXT_COLUMN, "0",
            wxDefaultPosition, wxDefaultSize, 0);
        mZoomOutButton = new wxButton(this, BUTTON_ZOOM_OUT, _T("Zoom out"),
            wxDefaultPosition, wxDefaultSize, 0);
        mZoomInButton = new wxButton(this, BUTTON_ZOOM_IN, _T("Zoom in"),
            wxDefaultPosition, wxDefaultSize, 0);
		mPositionControlSizer->Add(mRowTextEntry, 1, wxEXPAND);
		mPositionControlSizer->Add(mColumnTextEntry, 1, wxEXPAND);
		mPositionControlSizer->Add(mZoomOutButton, 1, wxEXPAND);
		mPositionControlSizer->Add(mZoomInButton, 1, wxEXPAND);

        // Add buttons to control simulation
        mSimControlSizer = new wxBoxSizer(wxHORIZONTAL);
        mTickButton = new wxButton(this, BUTTON_TICK, _T("Tick"),
            wxDefaultPosition, wxDefaultSize, 0);
        mOutputButton = new wxButton(this, BUTTON_OUTPUT, _T("Output"),
            wxDefaultPosition, wxDefaultSize, 0);
        mPlayButton = new wxButton(this, BUTTON_PLAY, _T("Play/Stop"),
            wxDefaultPosition, wxDefaultSize, 0);
		mSimControlSizer->Add(mTickButton, 1, wxEXPAND);
		mSimControlSizer->Add(mPlayButton, 1, wxEXPAND);
		mSimControlSizer->Add(mOutputButton, 1, wxEXPAND);

		mButtonsSizer->Add(mPositionControlSizer, 1, wxEXPAND);
		mButtonsSizer->Add(mSimControlSizer, 1, wxEXPAND);
		this->SetSizer(mButtonsSizer);

        this->Show();
        return true;
    }

    void refreshDisplay()
    {
        mDrawPane->Refresh();
        mDrawPane->Update();
    }

    void tick()
    {
        mBoard->update();
        refreshDisplay();
    }

    void OnTickClick(wxCommandEvent& event)
    {
        if (!mTimer->IsRunning())
        {
            tick();
        }
    }

    void OnPlayClick(wxCommandEvent& event)
    {
        if (mTimer->IsRunning())
        {
            mTimer->Stop();
        }
        else
        {
            mTimer->Start(TICK_TIME);
        }
    }

    void OnOutputClick(wxCommandEvent& event)
    {
        mBoard->writeBoard("../../output.txt");
    }

    void OnZoomOut(wxCommandEvent& event)
    {
        mDrawPane->zoomOut();
        if (!mTimer->IsRunning())
        {
            refreshDisplay();
        }
    }

    void OnZoomIn(wxCommandEvent& event)
    {
        mDrawPane->zoomIn();
        if (!mTimer->IsRunning())
        {
            refreshDisplay();
        }
    }

    void OnRowEntry(wxCommandEvent& event)
    {
        wxString textContent = event.GetString();
        mDrawPane->changeRow(atoi(textContent.c_str()));
        if (!mTimer->IsRunning())
        {
            refreshDisplay();
        }
    }

    void OnColumnEntry(wxCommandEvent& event)
    {
        wxString textContent = event.GetString();
        mDrawPane->changeColumn(atoi(textContent.c_str()));
        if (!mTimer->IsRunning())
        {
            refreshDisplay();
        }
    }

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(GOLFrame, wxFrame)
EVT_BUTTON(BUTTON_TICK, GOLFrame::OnTickClick)
EVT_BUTTON(BUTTON_PLAY, GOLFrame::OnPlayClick)
EVT_BUTTON(BUTTON_OUTPUT, GOLFrame::OnOutputClick)
EVT_BUTTON(BUTTON_ZOOM_OUT, GOLFrame::OnZoomOut)
EVT_BUTTON(BUTTON_ZOOM_IN, GOLFrame::OnZoomIn)
EVT_TEXT_ENTER(TEXT_ROW, GOLFrame::OnRowEntry)
EVT_TEXT_ENTER(TEXT_COLUMN, GOLFrame::OnColumnEntry)
END_EVENT_TABLE()

GOLFrame::PlayTimer::PlayTimer(GOLFrame *frame)
: wxTimer()
{
    mFrame = frame;
}

void GOLFrame::PlayTimer::Notify()
{
    mFrame->tick();
}

/**
 * Application launcher.
 */
class GOLApp : public wxApp
{
protected:
    GOLFrame *mFrame;
    Board *mBoard;

public:
    bool OnInit()
    {
        // make a test board
		mBoard = new SparseBoard();
        //board->loadBoard("../../input/glider.txt");
		mBoard->loadBoard("../../input/glider_gun.txt");
        //board->loadBoard("../../input/nova.txt");

		mFrame = new GOLFrame(mBoard, NULL, wxID_ANY, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));
		return mFrame->initialize();
	}

	~GOLApp()
	{
		//delete mFrame;
		delete mBoard;
	}
};

IMPLEMENT_APP(GOLApp)