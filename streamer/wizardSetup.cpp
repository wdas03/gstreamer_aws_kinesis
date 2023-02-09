/////////////////////////////////////////////////////////////////////////////
// Name:        wizarddialog.cpp
// Purpose:     
// Author:      Anthemion Software Ltd
// Modified by: 
// Created:     Sunday, 31 July 2022 at 22:58:08
// RCS-ID:      
// Copyright:   (c) Anthemion Software Ltd
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Sunday, 31 July 2022 at 22:58:08

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <gst/gst.h>
#include <gstStreamUtil.h>
#include <gstDeviceUtil.h>
#include <iostream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <charconv>


#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wizardSetup.h"

////@begin XPM images
////@end XPM images

using namespace std;

vector<int> cameraIndices;

class MyApp : public wxApp
{
public:
    // override base class virtuals
    virtual bool OnInit() wxOVERRIDE;
};


/*!
 * WizardDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardDialog, wxWizard )


/*!
 * WizardDialog event table definition
 */

BEGIN_EVENT_TABLE( WizardDialog, wxWizard )

////@begin WizardDialog event table entries
////@end WizardDialog event table entries

END_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    int argc = wxGetApp().argc;
    char **argv = wxGetApp().argv; 

    gst_init(&argc, &argv);

    wxLogDebug(to_string(argc).c_str());

    if ( !wxApp::OnInit() ) {
        wxLogError(wxT("Couldn't start app!"));
        return false;
    }

    wxLogDebug(wxT("Started app!"));

    //wxLogWindow *log_window = new wxLogWindow(NULL, "Log Window", TRUE, TRUE);

	// Doesn't work
	//log_window->LogText("Hello World\n");

    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    //wxXmlResource::Get()->Load("wizard.png");
    wxXmlResource::Get()->Load("wizardSetup.xrc");

    wxLogDebug(wxT("Loaded xrc resources."));

    wxWizard wizardDlg;
    if (wxXmlResource::Get()->LoadObject(&wizardDlg, NULL, "ID_WIZARDDIALOG", "wxWizard"))
        wxLogDebug("wizardDlg loaded.");

    wxWindow *parent = new wxWindow();
    WizardDialog wd = new WizardDialog(parent, 0, wxDefaultPosition);
    wd.Run();

    // we're done
    return true;
}

/*!
 * WizardDialog constructors
 */

WizardDialog::WizardDialog()
{
    Init();
}

WizardDialog::WizardDialog( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
    Init();
    Create(parent, id, pos);
}


/*!
 * WizardDialog creator
 */

bool WizardDialog::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin WizardDialog creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS|wxWIZARD_EX_HELPBUTTON);
    SetParent(parent);
    CreateControls();
////@end WizardDialog creation
    return true;
}


/*!
 * WizardDialog destructor
 */

WizardDialog::~WizardDialog()
{
////@begin WizardDialog destruction
////@end WizardDialog destruction
}


/*!
 * Member initialisation
 */

void WizardDialog::Init()
{
////@begin WizardDialog member initialisation
////@end WizardDialog member initialisation
}


/*!
 * Control creation for WizardDialog
 */

void WizardDialog::CreateControls()
{    
////@begin WizardDialog content construction
    if (!wxXmlResource::Get()->LoadObject(this, GetParent(), wxT("ID_WIZARDDIALOG"), wxT("wxWizard")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));

    for (wxWindowList::Node* node = GetChildren().GetFirst(); node; node = node->GetNext())
    {
        wxWizardPage* page = wxDynamicCast(node->GetData(), wxWizardPage);
        if (page)
            GetPageAreaSizer()->Add(page);
    }

////@end WizardDialog content construction

    // Create custom windows not generated automatically here.
////@begin WizardDialog content initialisation
////@end WizardDialog content initialisation
}


/*!
 * Runs the wizard.
 */

bool WizardDialog::Run()
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node)
    {
        wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
        if (startPage) return RunWizard(startPage);
        node = node->GetNext();
    }
    return false;
}


/*!
 * Should we show tooltips?
 */

bool WizardDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap WizardDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WizardDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WizardDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon WizardDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WizardDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WizardDialog icon retrieval
}


/*!
 * FirstPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FirstPage, wxWizardPageSimple )


/*!
 * FirstPage event table definition
 */

BEGIN_EVENT_TABLE( FirstPage, wxWizardPageSimple )

////@begin FirstPage event table entries
////@end FirstPage event table entries

END_EVENT_TABLE()


/*!
 * FirstPage constructors
 */

FirstPage::FirstPage()
{
    Init();
}

FirstPage::FirstPage( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*!
 * FirstPage creator
 */

bool FirstPage::Create( wxWizard* parent )
{
////@begin FirstPage creation
    CreateControls();
////@end FirstPage creation
    return true;
}


/*!
 * FirstPage destructor
 */

FirstPage::~FirstPage()
{
////@begin FirstPage destruction
////@end FirstPage destruction
}


/*!
 * Member initialisation
 */

void FirstPage::Init()
{
////@begin FirstPage member initialisation
////@end FirstPage member initialisation
}


/*!
 * Control creation for FirstPage
 */

void FirstPage::CreateControls()
{    
////@begin FirstPage content construction
////@end FirstPage content construction

    // Create custom windows not generated automatically here.
////@begin FirstPage content initialisation
////@end FirstPage content initialisation
}


/*!
 * Should we show tooltips?
 */

bool FirstPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FirstPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FirstPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FirstPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FirstPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FirstPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FirstPage icon retrieval
}


/*!
 * SecondPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SecondPage, wxWizardPageSimple )


/*!
 * SecondPage event table definition
 */

BEGIN_EVENT_TABLE( SecondPage, wxWizardPageSimple )

////@begin SecondPage event table entries
////@end SecondPage event table entries

END_EVENT_TABLE()


/*!
 * SecondPage constructors
 */

SecondPage::SecondPage()
{
    Init();
}

SecondPage::SecondPage( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*!
 * SecondPage creator
 */

bool SecondPage::Create( wxWizard* parent )
{
////@begin SecondPage creation
    CreateControls();
////@end SecondPage creation
    return true;
}


/*!
 * SecondPage destructor
 */

SecondPage::~SecondPage()
{
////@begin SecondPage destruction
////@end SecondPage destruction
}


/*!
 * Member initialisation
 */

void SecondPage::Init()
{
////@begin SecondPage member initialisation
////@end SecondPage member initialisation
}


/*!
 * Control creation for SecondPage
 */

void SecondPage::CreateControls()
{    
////@begin SecondPage content construction
////@end SecondPage content construction

    // Create custom windows not generated automatically here.
////@begin SecondPage content initialisation
////@end SecondPage content initialisation
}


/*!
 * Should we show tooltips?
 */

bool SecondPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SecondPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SecondPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SecondPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SecondPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SecondPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SecondPage icon retrieval
}

/*!
 * FourthPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FourthPage, wxWizardPageSimple )


/*!
 * FourthPage event table definition
 */

BEGIN_EVENT_TABLE( FourthPage, wxWizardPageSimple )

////@begin FourthPage event table entries
    EVT_BUTTON( XRCID("ID_BUTTON"), FourthPage::OnButtonClick )
////@end FourthPage event table entries


END_EVENT_TABLE()


/*!
 * FourthPage constructors
 */

FourthPage::FourthPage()
{
    Init();
}

FourthPage::FourthPage( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*!
 * WizardPage creator
 */

bool FourthPage::Create( wxWizard* parent )
{
////@begin FourthPage creation
    CreateControls();
////@end FourthPage creation
    return true;
}


/*!
 * FourthPage destructor
 */

FourthPage::~FourthPage()
{
////@begin FourthPage destruction
////@end FourthPage destruction
}


/*!
 * Member initialisation
 */

void FourthPage::Init()
{
////@begin FourthPage member initialisation
////@end FourthPage member initialisation
}


/*!
 * Control creation for WizardPage
 */

void FourthPage::CreateControls()
{    
////@begin FourthPage content construction
////@end FourthPage content construction

    // Create custom windows not generated automatically here.
////@begin FourthPage content initialisation
////@end FourthPage content initialisation
}


/*!
 * Should we show tooltips?
 */

bool FourthPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FourthPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FourthPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FourthPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FourthPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FourthPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FourthPage icon retrieval
}

/*!
 * ThirdPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ThirdPage, wxWizardPageSimple )


/*!
 * ThirdPage event table definition
 */

BEGIN_EVENT_TABLE( ThirdPage, wxWizardPageSimple )

////@begin ThirdPage event table entries
    EVT_BUTTON( XRCID("ID_BUTTON_SCAN_CAMERAS"), ThirdPage::OnButtonScanCamerasClick )
    EVT_LISTBOX( XRCID("ID_CHECKLISTBOX_CAMERA_LIST"), ThirdPage::OnChecklistboxCameraListSelected )
    EVT_UPDATE_UI( XRCID("ID_CHECKLISTBOX_CAMERA_LIST"), ThirdPage::OnChecklistboxCameraListUpdate )
////@end ThirdPage event table entries

END_EVENT_TABLE()


/*!
 * ThirdPage constructors
 */

ThirdPage::ThirdPage()
{
    Init();
}

ThirdPage::ThirdPage( wxWizard* parent )
{
    Init();
    Create( parent );
}


/*!
 * ThirdPage creator
 */

bool ThirdPage::Create( wxWizard* parent )
{
////@begin ThirdPage creation
    CreateControls();
////@end ThirdPage creation
    return true;
}


/*!
 * ThirdPage destructor
 */

ThirdPage::~ThirdPage()
{
////@begin ThirdPage destruction
////@end ThirdPage destruction
}


/*!
 * Member initialisation
 */

void ThirdPage::Init()
{
////@begin ThirdPage member initialisation
////@end ThirdPage member initialisation
}


/*!
 * Control creation for ThirdPage
 */

void ThirdPage::CreateControls()
{    
////@begin ThirdPage content construction
    // Connect events and objects
    FindWindow(XRCID("ID_BUTTON_SCAN_CAMERAS"))->Connect(XRCID("ID_BUTTON_SCAN_CAMERAS"), wxEVT_LEFT_DCLICK, wxMouseEventHandler(ThirdPage::OnLeftDClick), NULL, this);
    FindWindow(XRCID("ID_BUTTON_SCAN_CAMERAS"))->Connect(XRCID("ID_BUTTON_SCAN_CAMERAS"), wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(ThirdPage::OnMiddleDClick), NULL, this);
    FindWindow(XRCID("ID_BUTTON_SCAN_CAMERAS"))->Connect(XRCID("ID_BUTTON_SCAN_CAMERAS"), wxEVT_RIGHT_DCLICK, wxMouseEventHandler(ThirdPage::OnRightDClick), NULL, this);
////@end ThirdPage content construction

    // Create custom windows not generated automatically here.
////@begin ThirdPage content initialisation
////@end ThirdPage content initialisation
}


/*!
 * Should we show tooltips?
 */

bool ThirdPage::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ThirdPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ThirdPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ThirdPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ThirdPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ThirdPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ThirdPage icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
 */

void ThirdPage::OnButtonScanCamerasClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in ThirdPage.
    // Before editing this code, remove the block markers.
    wxLogDebug(wxT("Scan camera button clicked!"));
    GList *devices = getDevices(TRUE);
    //wxButton *btn = wxDynamicCast(event.GetEventObject(), wxButton);

    wxWizard wizardDlg;
    if (wxXmlResource::Get()->LoadObject(&wizardDlg, NULL, "ID_WIZARDDIALOG", "wxWizard"))
        wxLogDebug("wizardDlg loaded.");

    wxCheckListBox *w = XRCCTRL(*this, "ID_CHECKLISTBOX_CAMERA_LIST", wxCheckListBox);
    //w->~wxCheckListBox(); // Clear checklist box
    wxLogDebug(to_string(w->GetCount()).c_str());
    for (int i=0; i < w->GetCount(); i++) {
        //wxLogDebug(to_string(i).c_str());
        w->Delete(i);
    }

    w->SetFirstItem(0);
    //w->Check(0);

    //wxString items[] = {"test", "what"};
    //w->InsertItems(2, items, 1);

    for (int i = 0; i < g_list_length(devices); i++) {
        GstDevice *device = static_cast<GstDevice *>(g_list_nth_data(devices, i));
        char *deviceName = static_cast<char *>(gst_device_get_display_name(device));

        wxString items[] = {deviceName};
        w->InsertItems(1, items, 0);

        printf("Name: %s\n", deviceName);
        //print_device(device, FALSE);
        //printf("device: %s\n", gst_device_get_display_name(device));
    }
    
    
    //if (!w->IsChecked(1)) 
    //    wxLogDebug("Yes");
    //event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in ThirdPage. 
}


/*!
 * wxEVT_LEFT_DCLICK event handler for ID_BUTTON
 */

void ThirdPage::OnLeftDClick( wxMouseEvent& event )
{
////@begin wxEVT_LEFT_DCLICK event handler for ID_BUTTON in ThirdPage.
    // Before editing this code, remove the block markers.
    printf("Scan Camera Button Clicked (Left click)");
    event.Skip();
////@end wxEVT_LEFT_DCLICK event handler for ID_BUTTON in ThirdPage. 
}


/*!
 * wxEVT_MIDDLE_DCLICK event handler for ID_BUTTON
 */

void ThirdPage::OnMiddleDClick( wxMouseEvent& event )
{
////@begin wxEVT_MIDDLE_DCLICK event handler for ID_BUTTON in ThirdPage.
    // Before editing this code, remove the block markers.
    printf("Scan Camera Button Clicked (Middle Click)");
    event.Skip();
////@end wxEVT_MIDDLE_DCLICK event handler for ID_BUTTON in ThirdPage. 
}


/*!
 * wxEVT_RIGHT_DCLICK event handler for ID_BUTTON
 */

void ThirdPage::OnRightDClick( wxMouseEvent& event )
{
////@begin wxEVT_RIGHT_DCLICK event handler for ID_BUTTON in ThirdPage.
    // Before editing this code, remove the block markers.
    printf("Scan Camera Button Clicked (Right Click)");
    event.Skip();
////@end wxEVT_RIGHT_DCLICK event handler for ID_BUTTON in ThirdPage. 
}

void ThirdPage::OnChecklistboxCameraListSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_CAMERA_LIST in ThirdPage.
    // Before editing this code, remove the block markers.
    wxLogDebug(wxT("Item Checked!"));

    wxWizard wizardDlg;
    if (!wxXmlResource::Get()->LoadObject(&wizardDlg, NULL, "ID_WIZARDDIALOG", "wxWizard"))
        wxLogDebug("Couldn't load wizard dialog.");

    wxCheckListBox *cameraChecklist = XRCCTRL(*this, "ID_CHECKLISTBOX_CAMERA_LIST", wxCheckListBox);
    g_print("%d boxes present.\n", cameraChecklist->GetCount());

    wxArrayInt checklistIndices;
    int numChecked = cameraChecklist->GetCheckedItems(checklistIndices);

    g_print("%d items checked\n", cameraChecklist->GetCount());

    cameraIndices.clear();

    for (int idx : checklistIndices) {
        cameraIndices.push_back(idx);
        g_print("Index %d checked.\n", idx);
    }

    
    
    //event.Skip();
////@end wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_CAMERA_LIST in ThirdPage. 
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX_CAMERA_LIST
 */

void ThirdPage::OnChecklistboxCameraListUpdate( wxUpdateUIEvent& event )
{
////@begin wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX_CAMERA_LIST in ThirdPage.
    // Before editing this code, remove the block markers.
    //wxLogDebug(wxT("Checklistbox updated!"));
    //event.Skip();
////@end wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX_CAMERA_LIST in ThirdPage. 
}

void FourthPage::OnButtonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in FourthPage.
    // Before editing this code, remove the block markers.

    for (int idx : cameraIndices) {
        g_print("Camera index from third page: %d\n", idx);
    }

    //int test = pushStreamToKVS("test", cameraIndices[0], NULL, 1920, 1080, 30, 1);

    wxLogDebug("Test Camera Selected!");
    //event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON in FourthPage. 
}