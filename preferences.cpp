#include "stdafx.h"
#include "resource.h"

#include <Windows.h>

// GUID För att identifiera de olika inställningarna
static const GUID guid_cfg_COMPort = { 0xe90b8a05, 0x93cc, 0x40e3, { 0x8a, 0x69, 0x5d, 0x4e, 0xb2, 0x4, 0xec, 0xa9 } };
static const GUID guid_cfg_BaudRate = { 0xe90b8a05, 0x93cc, 0x40e3, { 0x8a, 0x69, 0x5d, 0x4e, 0xb2, 0x4, 0xec, 0xa9 } };
static const GUID guid_cfg_SignAdress = { 0xbea44a7b, 0x7efd, 0x43f8, { 0xa3, 0x35, 0x4f, 0x51, 0x56, 0xb, 0x77, 0x31 } };
static const GUID guid_cfg_PatternPlaying = { 0x6e18fba5, 0x2978, 0x4484, { 0xba, 0x56, 0x50, 0xb6, 0xc6, 0x0, 0xae, 0x78 } };
static const GUID guid_cfg_PatternPaused = { 0xc80b1b2a, 0x3712, 0x426f, { 0xba, 0x41, 0xc7, 0x2c, 0x5f, 0x49, 0xf4, 0x9f } };

static const GUID guid_cfg_ModePlaying = { 0xdb561b0a, 0xfd9, 0x4fac, { 0xb2, 0xc1, 0xba, 0xd5, 0x67, 0xd7, 0x1f, 0x4b } };
static const GUID guid_cfg_ModePaused = { 0x6261981b, 0x5cc9, 0x4428, { 0x87, 0x2b, 0xf5, 0x63, 0xde, 0x95, 0xe7, 0x7b } };

static const GUID guid_cfg_ModeStopped = { 0xc0b4b8e8, 0x9424, 0x469c, { 0x8c, 0xb3, 0x48, 0xa6, 0x42, 0xd0, 0x2, 0x56 } };
static const GUID guid_cfg_PatternStopped = { 0xa85f9540, 0x87e, 0x4a10, { 0x95, 0xf6, 0xd9, 0xd9, 0x87, 0x76, 0x41, 0x98 } };
static const GUID guid_cfg_UseClock = { 0x7f2bf02b, 0xfcf7, 0x4afc, { 0x84, 0x82, 0xc3, 0x14, 0x22, 0x78, 0xbc, 0x6b } };



//Orginalvärde för de olika instälningarna
enum {
	default_cfg_ComPort = 1,
	default_cfg_Baud = 9600,
	default_cfg_SignAdress = 128,
	default_cfg_ModePlaying = 'A',
	default_cfg_ModePaused = 'A',
	default_cfg_ModeStopped = 'A',
	default_cfg_Clock = true
};

//char * kan inte spars i enum
const char * default_cfg_PatternPlaying = "\\h %CODEC% %bitrate% %title% ";
const char * default_cfg_PatternPaused = "\\r Paused %title%";
const char * default_cfg_PatternStopped = "\\r Stopped";

//variabler för inställningarna länkat till guid
 cfg_uint cfg_COMPort(guid_cfg_COMPort, default_cfg_ComPort), cfg_BaudRate(guid_cfg_BaudRate, default_cfg_Baud), cfg_SignAdress(guid_cfg_SignAdress, default_cfg_SignAdress);

 cfg_string cfg_PatternPlaying(guid_cfg_PatternPlaying, default_cfg_PatternPlaying), cfg_PatternPaused(guid_cfg_PatternPaused, default_cfg_PatternPaused), cfg_PatternStopped(guid_cfg_PatternStopped, default_cfg_PatternStopped);

 cfg_uint cfg_ModePlaying(guid_cfg_ModePlaying, default_cfg_ModePlaying), cfg_ModePaused(guid_cfg_ModePaused, default_cfg_ModePaused), cfg_ModeStopped(guid_cfg_ModeStopped, default_cfg_ModeStopped);

 cfg_bool cfg_UseClock(guid_cfg_UseClock, default_cfg_Clock);



class CMyPreferences : public CDialogImpl<CMyPreferences>, public preferences_page_instance {
public:
	//Constructor - invoked by preferences_page_impl helpers - don't do Create() in here, preferences_page_impl does this for us
	CMyPreferences(preferences_page_callback::ptr callback) : m_callback(callback) {}

	//Note that we don't bother doing anything regarding destruction of our class.
	//The host ensures that our dialog is destroyed first, then the last reference to our preferences_page_instance object is released, causing our object to be deleted.


	//dialog resource ID
	enum {IDD = IDD_MYPREFERENCES};
	// preferences_page_instance methods (not all of them - get_wnd() is supplied by preferences_page_impl helpers)
	t_uint32 get_state();
	void apply();
	void reset();
	
	//REgistrerar events för fönstret
	BEGIN_MSG_MAP(CMyPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_COMPORT, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_BAUD, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_SignAdress, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_PatternPlaying, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_PatternPaused, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_PatternStopped, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_ModePlaying, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_ModePaused, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_ModeStopped, EN_CHANGE, OnEditChange)
		COMMAND_HANDLER_EX(IDC_Clock, BN_CLICKED, OnEditChange)
	END_MSG_MAP()
private:
	
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnEditChange(UINT, int, CWindow);
	bool HasChanged();
	void OnChanged();

	const preferences_page_callback::ptr m_callback;

};

//Sätter alla värden när iställningarna öppnas
BOOL CMyPreferences::OnInitDialog(CWindow, LPARAM) {
	
	//Sätter max läng på Text fälten 
	HWND EB = GetDlgItem(IDC_COMPORT); 
	SendMessage(EB, EM_LIMITTEXT, 3, 0);
	EB = GetDlgItem(IDC_BAUD); 
	SendMessage(EB, EM_LIMITTEXT, 5, 0);
	EB = GetDlgItem(IDC_SignAdress); 
	SendMessage(EB, EM_LIMITTEXT, 4, 0);
	EB = GetDlgItem(IDC_PatternPlaying); 
	SendMessage(EB, EM_LIMITTEXT, 100, 0);
	EB = GetDlgItem(IDC_PatternPlaying); 
	SendMessage(EB, EM_LIMITTEXT, 100, 0);
	EB = GetDlgItem(IDC_PatternStopped); 
	SendMessage(EB, EM_LIMITTEXT, 100, 0);

	//Ställer in dialogerna orginal värde
	SetDlgItemInt(IDC_COMPORT, cfg_COMPort, FALSE);
	SetDlgItemInt(IDC_BAUD, cfg_BaudRate, FALSE);
	SetDlgItemInt(IDC_SignAdress, cfg_SignAdress, FALSE);
	uSetDlgItemText(*this, IDC_PatternPlaying, cfg_PatternPlaying);
	uSetDlgItemText(*this, IDC_PatternPaused, cfg_PatternPaused);
	uSetDlgItemText(*this, IDC_PatternStopped, cfg_PatternStopped);

	//Sätter textboxen
	EB = GetDlgItem(IDC_Clock);
	SendMessage(EB, BM_SETCHECK, cfg_UseClock ? BST_CHECKED : BST_UNCHECKED, 0); //? = inline if

	HWND PAT = GetDlgItem(IDC_PatternStopped);
	HWND MODE = GetDlgItem(IDC_ModeStopped);
	if(cfg_UseClock)
	{
		::EnableWindow(PAT, FALSE);
		::EnableWindow(MODE, FALSE);
	}else
	{
		::EnableWindow(PAT, TRUE);
		::EnableWindow(MODE, TRUE);
	}



	//Lista över lägena 
	wchar_t * Modes[24] = {
		_T("Cyclic"),
		_T("Imidate"),
		 _T("Open From Right"),
		 _T("Open from Left"),
		 _T("Open From Center"),
		 _T("Open To Center"),
		 _T("Cover From Center"), 
		 _T("Cover From right"),
		 _T("Cover from left"),
		 _T("Cover to center"),
		 _T("Scroll up "),
		 _T("Scroll down"),
		 _T("Interlace to center"),
		 _T("Interlace cover "),
		 _T("Cover Up"),
		 _T("Cover Down"),
		 _T("Scanline"),
		 _T("Explode"),
		 _T("Pacman"),
		 _T("Fall & Stack"),
		 _T("Shoot"),
		 _T("Flash"),
		 _T("Random"),
		_T("Slide IN")
	};

	//Fyller de tre lägeslistorna
	HWND CB1 = GetDlgItem(IDC_ModePlaying); 
	HWND CB2 = GetDlgItem(IDC_ModePaused);
	HWND CB3 = GetDlgItem(IDC_ModeStopped);

	for(int i = 0; i<24; i++)
	{
		SendMessage(CB1, CB_ADDSTRING, 0,(LPARAM)Modes[i]);
		SendMessage(CB2, CB_ADDSTRING, 0,(LPARAM)Modes[i]);
		SendMessage(CB3, CB_ADDSTRING, 0,(LPARAM)Modes[i]);
	}

	//Lägena är gjorda som bokstäverna mellan A-X
	//Vi kan då göra om mellan index och läge genom att använda en bokstavs position
	SendMessage(CB1, CB_SETCURSEL, cfg_ModePlaying - 'A', 0); 
	SendMessage(CB2, CB_SETCURSEL, cfg_ModePaused - 'A', 0); 
	SendMessage(CB3, CB_SETCURSEL, cfg_ModeStopped - 'A', 0);
	
	return FALSE;
}

void CMyPreferences::OnEditChange(UINT, int, CWindow) {

	//Avaktiverar stoppat fältet när använd klocka checkas
	HWND EB = GetDlgItem(IDC_Clock);
	bool UseClock = (SendMessage(EB, BM_GETCHECK,0 , 0) == BST_CHECKED);

	HWND PAT = GetDlgItem(IDC_PatternStopped);
	HWND MODE = GetDlgItem(IDC_ModeStopped);

	if(UseClock)
	{
		::EnableWindow(PAT, FALSE);
		::EnableWindow(MODE, FALSE);
	}else
	{
		::EnableWindow(PAT, TRUE);
		::EnableWindow(MODE, TRUE);
	}

	OnChanged();
}

//Används för att veta att något har ändrat på sig
t_uint32 CMyPreferences::get_state() {
	t_uint32 state = preferences_state::resettable;
	if (HasChanged()) state |= preferences_state::changed;
	return state;
}

//Återställer dialogvärdena
void CMyPreferences::reset() {
	SetDlgItemInt(IDC_COMPORT, default_cfg_ComPort, FALSE);
	SetDlgItemInt(IDC_BAUD, default_cfg_Baud, FALSE);
	SetDlgItemInt(IDC_SignAdress, default_cfg_SignAdress, FALSE);
	uSetDlgItemText(*this, IDC_PatternPlaying, default_cfg_PatternPlaying);
	uSetDlgItemText(*this, IDC_PatternPaused, default_cfg_PatternPaused);
	uSetDlgItemText(*this, IDC_PatternStopped, default_cfg_PatternStopped);

	HWND EB = GetDlgItem(IDC_Clock);
	SendMessage(EB, BM_SETCHECK, default_cfg_Clock ? BST_CHECKED : BST_UNCHECKED, 0); //? = inline if

	HWND CB1 = GetDlgItem(IDC_ModePlaying); 
	HWND CB2 = GetDlgItem(IDC_ModePaused);
	HWND CB3 = GetDlgItem(IDC_ModeStopped);
	SendMessage(CB1, CB_SETCURSEL, default_cfg_ModePlaying - 'A', 0); 
	SendMessage(CB2, CB_SETCURSEL, default_cfg_ModePaused - 'A', 0); 
	SendMessage(CB3, CB_SETCURSEL, default_cfg_ModeStopped - 'A', 0); 
	
	OnChanged();
}


//Värkställer dialogvärdena
void CMyPreferences::apply() {
	cfg_COMPort = GetDlgItemInt(IDC_COMPORT, NULL, FALSE);
	cfg_BaudRate = GetDlgItemInt(IDC_BAUD, NULL, FALSE);
	cfg_SignAdress =  GetDlgItemInt(IDC_SignAdress, NULL, FALSE);
	uGetDlgItemText(*this, IDC_PatternPlaying, cfg_PatternPlaying);
	uGetDlgItemText(*this, IDC_PatternPaused, cfg_PatternPaused);
	uGetDlgItemText(*this, IDC_PatternStopped, cfg_PatternStopped);

	HWND EB = GetDlgItem(IDC_Clock);
	cfg_UseClock = (SendMessage(EB, BM_GETCHECK,0 , 0) == BST_CHECKED);

	HWND CB1 = GetDlgItem(IDC_ModePlaying); 
	HWND CB2 = GetDlgItem(IDC_ModePaused);
	HWND CB3 = GetDlgItem(IDC_ModeStopped);
	cfg_ModePlaying = 'A' + SendMessage(CB1, CB_GETCURSEL, 0, 0); 
	cfg_ModePaused = 'A' + SendMessage(CB2, CB_GETCURSEL, 0, 0); 
	cfg_ModeStopped = 'A' + SendMessage(CB3, CB_GETCURSEL, 0, 0); 
	

	OnChanged(); //our dialog content has not changed but the flags have - our currently shown values now match the settings so the apply button can be disabled
}

bool CMyPreferences::HasChanged() {
	//returns whether our dialog content is different from the current configuration (whether the apply button should be enabled or not)

	HWND EB = GetDlgItem(IDC_Clock);

	HWND CB1 = GetDlgItem(IDC_ModePlaying); 
	HWND CB2 = GetDlgItem(IDC_ModePaused);
	HWND CB3 = GetDlgItem(IDC_ModeStopped);
	return GetDlgItemInt(IDC_COMPORT, NULL, FALSE) != cfg_COMPort || GetDlgItemInt(IDC_BAUD, NULL, FALSE) != cfg_BaudRate || GetDlgItemInt(IDC_SignAdress, NULL, FALSE) != cfg_SignAdress
		|| uGetDlgItemText(*this, IDC_PatternPlaying) != cfg_PatternPlaying || uGetDlgItemText(*this, IDC_PatternPaused) != cfg_PatternPaused || uGetDlgItemText(*this, IDC_PatternStopped) != cfg_PatternStopped
		|| SendMessage(CB1, CB_GETCURSEL, 0, 0) != cfg_ModePlaying - 'A' || SendMessage(CB2, CB_GETCURSEL, 0, 0) != cfg_ModePaused - 'A' ||  SendMessage(CB3, CB_GETCURSEL, 0, 0) != cfg_ModeStopped - 'A'
		|| cfg_UseClock != (SendMessage(EB, BM_GETCHECK,0 , 0) == BST_CHECKED);
}
void CMyPreferences::OnChanged() {
	//tell the host that our state has changed to enable/disable the apply button appropriately.
	m_callback->on_state_changed();
}

class preferences_page_myimpl : public preferences_page_impl<CMyPreferences> {
	// preferences_page_impl<> helper deals with instantiation of our dialog; inherits from preferences_page_v3.
public:
	const char * get_name() {return "Serial Display plugin";}
	GUID get_guid() {
		// This is our GUID. Replace with your own when reusing the code.
		static const GUID guid = {  0x58d59ea9, 0xef18, 0x4432, { 0x93, 0x10, 0x79, 0x2d, 0x8d, 0xc5, 0x60, 0x1b} };
		return guid;
	}
	GUID get_parent_guid() {return guid_tools;}
};

static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;

