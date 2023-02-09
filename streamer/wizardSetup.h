/////////////////////////////////////////////////////////////////////////////
// Name:        wizarddialog.h
// Purpose:     
// Author:      William Das
// Modified by: 
// Created:     Monday, 01 August 2022 at 00:03:23
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Monday, 01 August 2022 at 00:03:23

#ifndef _WIZARDDIALOG_H_
#define _WIZARDDIALOG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/wizard.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class FirstPage;
class SecondPage;
class ThirdPage;
class FourthPage;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_WIZARDDIALOG 13000
#define ID_WIZARD_DIALOG_FIRSTPAGE 13011
#define ID_WIZARD_DIALOG_SECONDPAGE 13014
#define ID_WIZARD_DIALOG_THIRDPAGE 13019
#define ID_WIZARD_DIALOG_FOURTHPAGE 13004
#define SYMBOL_WIZARDDIALOG_IDNAME ID_WIZARDDIALOG
////@end control identifiers


/*!
 * WizardDialog class declaration
 */

class WizardDialog: public wxWizard
{    
    DECLARE_DYNAMIC_CLASS( WizardDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WizardDialog();
    WizardDialog( wxWindow* parent, wxWindowID id = SYMBOL_WIZARDDIALOG_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WIZARDDIALOG_IDNAME, const wxPoint& pos = wxDefaultPosition );

    /// Destructor
    ~WizardDialog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin WizardDialog event handler declarations

////@end WizardDialog event handler declarations

////@begin WizardDialog member function declarations

    /// Runs the wizard
    bool Run();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WizardDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin WizardDialog member variables
////@end WizardDialog member variables
};

/*!
 * FirstPage class declaration
 */

class FirstPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( FirstPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FirstPage();

    FirstPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~FirstPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin FirstPage event handler declarations

////@end FirstPage event handler declarations

////@begin FirstPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FirstPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FirstPage member variables
////@end FirstPage member variables
};

/*!
 * SecondPage class declaration
 */

class SecondPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( SecondPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SecondPage();

    SecondPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~SecondPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin SecondPage event handler declarations

////@end SecondPage event handler declarations

////@begin SecondPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SecondPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SecondPage member variables
////@end SecondPage member variables
};

/*!
 * FourthPage class declaration
 */

class FourthPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( FourthPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FourthPage();

    FourthPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~FourthPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin FourthPage event handler declarations
    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON
    void OnButtonClick( wxCommandEvent& event );

////@end FourthPage event handler declarations

////@begin FourthPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FourthPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FourthPage member variables
////@end FourthPage member variables
};

/*!
 * ThirdPage class declaration
 */

class ThirdPage: public wxWizardPageSimple
{    
    DECLARE_DYNAMIC_CLASS( ThirdPage )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ThirdPage();

    ThirdPage( wxWizard* parent );

    /// Creation
    bool Create( wxWizard* parent );

    /// Destructor
    ~ThirdPage();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ThirdPage event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_SCAN_CAMERAS
    void OnButtonScanCamerasClick( wxCommandEvent& event );

    /// wxEVT_LEFT_DCLICK event handler for ID_BUTTON_SCAN_CAMERAS
    void OnLeftDClick( wxMouseEvent& event );

    /// wxEVT_MIDDLE_DCLICK event handler for ID_BUTTON_SCAN_CAMERAS
    void OnMiddleDClick( wxMouseEvent& event );

    /// wxEVT_RIGHT_DCLICK event handler for ID_BUTTON_SCAN_CAMERAS
    void OnRightDClick( wxMouseEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_CHECKLISTBOX_CAMERA_LIST
    void OnChecklistboxCameraListSelected( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_CHECKLISTBOX_CAMERA_LIST
    void OnChecklistboxCameraListUpdate( wxUpdateUIEvent& event );

////@end ThirdPage event handler declarations

////@begin ThirdPage member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ThirdPage member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ThirdPage member variables
////@end ThirdPage member variables
};

#endif
    // _WIZARDDIALOG_H_