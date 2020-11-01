// enable syslink in dialogs
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif

// use resources
#include "resource.h"

// Required Headers
#include "Octopath.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>
#include <ShObjIdl.h>
#include <winbase.h>
#include <processthreadsapi.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

// Create check buttons
HWND createCheckButton(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	HWND checkButton = CreateWindow(
		L"Button",
		name,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_LEFT | BS_FLAT,
		startx,
		starty,
		sizex,
		sizey,
		hwnd,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);
	return checkButton;
};

// Create tooltips
HWND createTooltips(int toolID, HWND hwnd, PTSTR pszText) {
	if (!toolID || !hwnd || !pszText) {
		return FALSE;
	}
	// Get the window of the tool
	HWND hwndTool = GetDlgItem(hwnd, toolID);

	// Create the tooltip
	HWND hwndTip = CreateWindowEx(
		NULL,
		TOOLTIPS_CLASS,
		NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwnd,
		NULL,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);
	if (!hwndTool || !hwndTip){
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hwnd;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	// set hovertime to 1 second
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_INITIAL, LOWORD(1000));
	// Set autotimeout to 16 seconds, essentially making it readable
	SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, LOWORD(16000));
	// Set tooltip length
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 450);

	return hwndTip;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	const size_t MAX_LOADSTRING = 256;
	const int bufferSize = 256;

	WCHAR className[MAX_LOADSTRING - 18];
	WCHAR title[MAX_LOADSTRING];
	WCHAR version[10]; // Version should not be longer than 6 bytes
	LoadString(hInstance, IDS_MYAPP_NAME, title, bufferSize);
	LoadString(hInstance, IDS_MYAPP_CLASS, className, (bufferSize - 18));
	LoadString(hInstance, IDV_VERSION, version, 10);

	// Combine the version and class name to show both in title
	WCHAR classVersion[MAX_LOADSTRING];
	swprintf_s(classVersion, L"%s %s", className, version);

	HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL_ICON));

	WNDCLASS wc = { };

	wc.lpfnWndProc		= WindowProc;
	wc.hIcon			= icon;
	wc.hInstance		= hInstance;
	wc.lpszClassName	= title;
	wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MAIN_MENU);

	RegisterClass(&wc);


	// Create the main window.
	HWND hwnd = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		title,
		classVersion,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		450,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Main message loop

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

BOOL CALLBACK UsageDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// Display error message for errors in randomization
void DisplayErrorMessageBox() {
	MessageBox(NULL, L"Something has gone wrong.\nCheck log.txt for details.", L"Error Randomizing", MB_ICONEXCLAMATION | MB_OK);
}

// Setting fonts
bool CALLBACK SetFont(HWND child, LPARAM font) {
	SendMessage(child, WM_SETFONT, font, true);
	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) {
	case WM_CREATE:
	{

		// File text box with input
		HWND hwndEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
			L"Edit",
			TEXT(""),
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			20,
			40,
			477,
			21,
			hwnd,
			(HMENU)IDE_EDIT,
			NULL,
			NULL
		);

		// Randomize button
		HWND fileButton = CreateWindow(
			L"Button",
			L"Browse",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER,
			500,
			40,
			60,
			21,
			hwnd,
			(HMENU)MAKEINTRESOURCE(IDB_FILE_BUTTON),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
		);
		// Randomize button
		HWND randomizeButton = CreateWindow(
			L"Button",
			L"Randomize",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER,
			255,
			337,
			90,
			33,
			hwnd,
			(HMENU)MAKEINTRESOURCE(IDB_RANDOMIZE_BUTTON),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
		);
		// Progress bar for randomization
		HWND randomizeBar = CreateWindowEx(
			NULL,
			PROGRESS_CLASS,
			NULL,
			WS_CHILD | WS_VISIBLE,
			40,
			307,
			500,
			25,
			hwnd,
			(HMENU)MAKEINTRESOURCE(IDP_RANDOMIZEBAR),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
		);
		// Hide progress bar untill needed
		ShowWindow(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), SW_HIDE);


		// Check Boxes for options
		HWND mixChapter24Bosses = createCheckButton(L"Mix Chapter 2-4 Bosses", 20, 89, 250, 23, hwnd, MAKEINTRESOURCE(IDB_MIXCHAPTER24));
		HWND mixChapter14Bosses = createCheckButton(L"Mix Chapter 1-4 Bosses", 30, 112, 250, 23, hwnd, MAKEINTRESOURCE(IDB_MIXCHAPTER14));
		HWND randomizeShrineBosses = createCheckButton(L"Randomize Shrine Bosses", 20, 135, 250, 23, hwnd, MAKEINTRESOURCE(IDB_RANDOMIZESHRINE));
		HWND includeShrineBosses = createCheckButton(L"Include Shrine Bosses", 30, 158, 250, 23, hwnd, MAKEINTRESOURCE(IDB_INCLUDESHRINE));
		HWND randomizeGateBosses = createCheckButton(L"Randomize Gate Bosses", 20, 181, 250, 23, hwnd, MAKEINTRESOURCE(IDB_RANDOMIZEGATE));
		HWND includeGateBosses = createCheckButton(L"Include Gate Bosses", 30, 204, 250, 23, hwnd, MAKEINTRESOURCE(IDB_INCLUDEGATE));
		HWND includeGaldera = createCheckButton(L"Include Galdera", 20, 227, 250, 23, hwnd, MAKEINTRESOURCE(IDB_INCLUDEGALDERA));
		HWND includeDuplicate = createCheckButton(L"Allow Duplicates", 20, 250, 250, 23, hwnd, MAKEINTRESOURCE(IDB_INCLUDEDUPLICATE));
		HWND trueRandom = createCheckButton(L"True Random", 300, 89, 250, 23, hwnd, MAKEINTRESOURCE(IDB_TRUERANDOM));

		// Tooltips for check boxes
		HWND mixChapter24BossesTooltip = createTooltips(IDB_MIXCHAPTER24, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER24));
		HWND mixChapter14BossesTooltip = createTooltips(IDB_MIXCHAPTER14, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER14));
		HWND randomizeShrineBossesTooltip = createTooltips(IDB_RANDOMIZESHRINE, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZESHRINE));
		HWND includeShineBossesTooltip = createTooltips(IDB_INCLUDESHRINE, hwnd, MAKEINTRESOURCE(IDS_INCLUDESHRINE));
		HWND randomizeGateBossesTooltip = createTooltips(IDB_RANDOMIZEGATE, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZEGATE));
		HWND includeGateBossesTooltip = createTooltips(IDB_INCLUDEGATE, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGATE));
		HWND includeGalderaTooltip = createTooltips(IDB_INCLUDEGALDERA, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGALDERA));
		HWND includeDuplicateTooltip = createTooltips(IDB_INCLUDEDUPLICATE, hwnd, MAKEINTRESOURCE(IDS_INCLUDEDUPLICATE));
		HWND trueRandomTooltip = createTooltips(IDB_TRUERANDOM, hwnd, MAKEINTRESOURCE(IDS_TRUERANDOM));
	}
	{
		// Read from config
		std::wstring pakPath;
		std::vector<bool> configs = configParser("Config.cfg", &pakPath);
		// Apply options from config file:
		configs[0] == true ? CheckDlgButton(hwnd, IDB_MIXCHAPTER24, BST_CHECKED) : CheckDlgButton(hwnd, IDB_MIXCHAPTER24, BST_UNCHECKED);
		configs[1] == true ? CheckDlgButton(hwnd, IDB_MIXCHAPTER14, BST_CHECKED) : CheckDlgButton(hwnd, IDB_MIXCHAPTER14, BST_UNCHECKED);
		configs[2] == true ? CheckDlgButton(hwnd, IDB_RANDOMIZESHRINE, BST_CHECKED) : CheckDlgButton(hwnd, IDB_RANDOMIZESHRINE, BST_UNCHECKED);
		configs[3] == true ? CheckDlgButton(hwnd, IDB_INCLUDESHRINE, BST_CHECKED) : CheckDlgButton(hwnd, IDB_INCLUDESHRINE, BST_UNCHECKED);
		configs[4] == true ? CheckDlgButton(hwnd, IDB_RANDOMIZEGATE, BST_CHECKED) : CheckDlgButton(hwnd, IDB_RANDOMIZEGATE, BST_UNCHECKED);
		configs[5] == true ? CheckDlgButton(hwnd, IDB_INCLUDEGATE, BST_CHECKED) : CheckDlgButton(hwnd, IDB_INCLUDEGATE, BST_UNCHECKED);
		configs[6] == true ? CheckDlgButton(hwnd, IDB_INCLUDEGALDERA, BST_CHECKED) : CheckDlgButton(hwnd, IDB_INCLUDEGALDERA, BST_UNCHECKED);
		configs[7] == true ? CheckDlgButton(hwnd, IDB_INCLUDEDUPLICATE, BST_CHECKED) : CheckDlgButton(hwnd, IDB_INCLUDEDUPLICATE, BST_UNCHECKED);
		configs[8] == true ? CheckDlgButton(hwnd, IDB_TRUERANDOM, BST_CHECKED) : CheckDlgButton(hwnd, IDB_TRUERANDOM, BST_UNCHECKED);
		// Set button state from config
		// Appy tree layout disablement first
		if (IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24) == false) {
			EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), FALSE);
			CheckDlgButton(hwnd, IDB_MIXCHAPTER14, BST_UNCHECKED);
		}
		if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE) == false) {
			EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), FALSE);
			CheckDlgButton(hwnd, IDB_INCLUDESHRINE, BST_UNCHECKED);
		}
		if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE) == false) {
			EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), FALSE);
			CheckDlgButton(hwnd, IDB_INCLUDEGATE, BST_UNCHECKED);
		}
		// Disable all buttons if true random is checked, enable if unchecked
		EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER24), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		EnableWindow(GetDlgItem(hwnd, IDB_RANDOMIZESHRINE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		EnableWindow(GetDlgItem(hwnd, IDB_RANDOMIZEGATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGALDERA), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEDUPLICATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		// Check if disabled already
		if (IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24)) {
			EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		}
		if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE)) {
			EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		}
		if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE)) {
			EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
		}
		// Apply Pak Path from Config File
		SendDlgItemMessage(hwnd, IDE_EDIT, WM_SETTEXT, 0, (LPARAM)pakPath.c_str());
		// Set font for child windows
		EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

	}
	break;

	case WM_DESTROY:
	{
		// redefine variables, as they will be different
		std::vector<bool> configs(9);
		// Save config options
		configs[0] = IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24);
		configs[1] = IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER14);
		configs[2] = IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE);
		configs[3] = IsDlgButtonChecked(hwnd, IDB_INCLUDESHRINE);
		configs[4] = IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE);
		configs[5] = IsDlgButtonChecked(hwnd, IDB_INCLUDEGATE);
		configs[6] = IsDlgButtonChecked(hwnd, IDB_INCLUDEGALDERA);
		configs[7] = IsDlgButtonChecked(hwnd, IDB_INCLUDEDUPLICATE);
		configs[8] = IsDlgButtonChecked(hwnd, IDB_TRUERANDOM);
		// Retrieve path for later use
		std::wstring pakPath;
		LRESULT len = SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXTLENGTH, 0, 0);
		WCHAR* buffer = new WCHAR[len];
		SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);

		// Write to config	
		configWriter("Config.cfg", configs[0], configs[1], configs[2], configs[3], configs[4], configs[5], configs[6], configs[7], configs[8], buffer);

	}
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
	{
		// check for update only once


		RECT rect;
		HFONT hFont;
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// Set fonts to Tahoma, make pak text bigger
		hFont = CreateFont(17, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
		SelectObject(hdc, hFont);
		SetRect(&rect, 20, 20, 477, 37);
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, TEXT("Octopath Traveler Pak Dir:"), -1, &rect, DT_NOCLIP);
		// Set font size to 15 Tahoma
		hFont = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
		SelectObject(hdc, hFont);
		// Boss randomizer options string
		SetRect(&rect, 20, 71, 86, 270);
		DrawText(hdc, TEXT("Boss Randomizer Options:"), -1, &rect, DT_NOCLIP);
		// Special options string
		SetRect(&rect, 300, 71, 86, 470);
		DrawText(hdc, TEXT("Special Options:"), -1, &rect, DT_NOCLIP);

		DeleteObject(hFont);
		EndPaint(hwnd, &ps);
	}
		break;
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		// Make button background same color as window
		HBRUSH hBrushBtn = (HBRUSH)(COLOR_WINDOW + 1);
		return ((LRESULT)hBrushBtn);
	}
		break;

	// Window Commands
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_HELP:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_HELP), hwnd, (DLGPROC)UsageDlgProc);
			break;
		case ID_HELP_ABOUT:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)UsageDlgProc);
			break;
		//Disable other buttons on true random selection
		case IDB_TRUERANDOM:
		{
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				// Disable all buttons if checked, enable when unchecked
				EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER24), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				EnableWindow(GetDlgItem(hwnd, IDB_RANDOMIZESHRINE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				EnableWindow(GetDlgItem(hwnd, IDB_RANDOMIZEGATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGALDERA), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEDUPLICATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);

				// Check if disabled already
				if (IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24)) {
					EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				}
				if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE)) {
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				}
				if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE)) {
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), IsDlgButtonChecked(hwnd, IDB_TRUERANDOM) ? FALSE : TRUE);
				}
				break;
			}
		}
		break;
		// follow tree structure for buttons
		case IDB_MIXCHAPTER24:
		{
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24) ? TRUE : FALSE);
				if (IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24) == false) {
					CheckDlgButton(hwnd, IDB_MIXCHAPTER14, BST_UNCHECKED);
				}
				break;
			}
		}
		break;
		case IDB_RANDOMIZESHRINE:
		{
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE) ? TRUE : FALSE);
				if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE) == false) {
					CheckDlgButton(hwnd, IDB_INCLUDESHRINE, BST_UNCHECKED);
				}
				break;
			}
		}
		break;
		case IDB_RANDOMIZEGATE:
		{
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE) ? TRUE : FALSE);
				if (IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE) == false) {
					CheckDlgButton(hwnd, IDB_INCLUDEGATE, BST_UNCHECKED);
				}
				break;
			}
		}
		break;
		// File Browser Button
		case IDB_FILE_BUTTON:
			HRESULT hr;
			hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr)) {
				IFileOpenDialog* pFileOpen;
				// Create the FileOpenDialog Object
				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
				if (SUCCEEDED(hr)) {

					// Show the open dialog box.
					pFileOpen->SetOptions(FOS_PICKFOLDERS);
					pFileOpen->SetTitle(L"Select Octopath Traveler Pak Dir");
					pFileOpen->SetFileNameLabel(L"Octopath Pak Dir:");
					hr = pFileOpen->Show(NULL);

					// Get the file name from the dialog box.
					if (SUCCEEDED(hr)) {
						IShellItem* pItem;
						hr = pFileOpen->GetFolder(&pItem);
						if (SUCCEEDED(hr)) {
							PWSTR pszFilePath;
							hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
							if (SUCCEEDED(hr)) {
								// Place the file path into the edit box
								SendDlgItemMessage(hwnd, IDE_EDIT, WM_SETTEXT, 0, (LPARAM)pszFilePath);
								CoTaskMemFree(pszFilePath);

							}
							pItem->Release();
						}
					}
					pFileOpen->Release();
				}
				CoUninitialize();
			}
		break;
		// Randomize Button
		case IDB_RANDOMIZE_BUTTON:
		{
			// Show progress bar
			ShowWindow(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), SW_SHOW);
			// Check pak path first
			// Retrieve pak path from edit control
			LRESULT len = SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXTLENGTH, 0, 0);
			WCHAR* buffer = new WCHAR[len];
			SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);

			// Verify that the pak path does indeed contain the Octopath pak
			DWORD pakAttrib = GetFileAttributes((std::wstring(buffer) + L"\\Octopath_Traveler-WindowsNoEditor.pak").c_str());
			if (pakAttrib == INVALID_FILE_ATTRIBUTES){
				MessageBox(hwnd, L"Octopath Pak file not found in pak path.\nView Usage for more details", L"Error Randomizing", MB_ICONEXCLAMATION | MB_OK);
			}
			else {
				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 10, 0);
				// Setup Random Number Generator
				std::random_device dev;
				std::mt19937 rng(dev());

				// Open log file 	
				std::wofstream logFile;
				logFile.open(L"log.txt");

				// Check for pak exe in bin folder, return error if not found
				DWORD unrealAttrib = GetFileAttributes(L".\\v4\\2\\3\\UnrealPak.exe");
				if (unrealAttrib != INVALID_FILE_ATTRIBUTES) {
					logFile << L"Found Unreal pak tool" << std::endl;
				}
				else {
					logFile << L"Unreal pak tool not found, did you place the executable as per the readme?" << std::endl << L"Exiting" << std::endl;
					DisplayErrorMessageBox();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}


				std::vector<bool> configs(9);
				// Check the options, and save them
				configs[0] = IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER24);
				configs[1] = IsDlgButtonChecked(hwnd, IDB_MIXCHAPTER14);
				configs[2] = IsDlgButtonChecked(hwnd, IDB_RANDOMIZESHRINE);
				configs[3] = IsDlgButtonChecked(hwnd, IDB_INCLUDESHRINE);
				configs[4] = IsDlgButtonChecked(hwnd, IDB_RANDOMIZEGATE);
				configs[5] = IsDlgButtonChecked(hwnd, IDB_INCLUDEGATE);
				configs[6] = IsDlgButtonChecked(hwnd, IDB_INCLUDEGALDERA);
				configs[7] = IsDlgButtonChecked(hwnd, IDB_INCLUDEDUPLICATE);
				configs[8] = IsDlgButtonChecked(hwnd, IDB_TRUERANDOM);

				// With all the setup, now start the main randomizing loop
				// Check which options are enabled, and choose an algorithm based on them
				// the Actual Randomization occures in seperate files

				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 20, 0);

				vectorvector randomizedLists;
				// True Chaos superseeds all options
				if (configs[8] == true) {
					randomizedLists = trueRandomFunction(rng);
				}
				// Next check for mix chapter options
				else if (configs[0] == true || configs[1] == true) {
					randomizedLists = mixRandomize(rng, configs[0], configs[1], configs[2], configs[3], configs[4], configs[5], configs[6], configs[7]);
				}
				// Next check for the include options
				else if (configs[3] == true || configs[5] == true || configs[6] == true) {
					randomizedLists = includeRandomize(rng, configs[2], configs[3], configs[4], configs[5], configs[6], configs[7]);
				}
				// last choice options, base options
				else {
					randomizedLists = baseRandomize(rng, configs[2], configs[4], configs[7]);
				}
				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 30, 0);

				int returnCheck = randomToFile(randomizedLists);
				// check if the randomization was successfull
				if (returnCheck == 1) {
					logFile << L"Randomization not successful, check above message for details" << std::endl;
					DisplayErrorMessageBox();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 40, 0);

				// try-catch statement for the execution, again to prevent errors
				try {
					logFile << "*********************************************" << std::endl;
					logFile << "Pak tool" << std::endl;
					logFile << "*********************************************" << std::endl;
					WCHAR fullFilename[MAX_PATH];
					GetFullPathName(L"randomizeboss.txt", MAX_PATH, fullFilename, nullptr);
					WCHAR command[MAX_PATH + 100];
					// Copy to char the path name
					swprintf_s(command, L".\\v4\\2\\3\\UnrealPak.exe ..\\..\\..\\RandomizedBosses_P.pak -Create=\"%s\" -compress", fullFilename);
					// for CreateProccess
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
 					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					ZeroMemory(&pi, sizeof(pi));

					// try using CreateProccess, without window created
					CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
					
					// Wait for the pak tool to complete
					WaitForSingleObject(pi.hProcess, INFINITE);

					// Close process and thread handles.
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
				catch (const std::exception& e) {
					std::cerr << e.what();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 60, 0);

				// Check if pak file was created, should always pass
				
				if (GetFileAttributes(L".\\RandomizedBosses_P.pak") == INVALID_FILE_ATTRIBUTES) {
					logFile << "Pak file not created, This shouldn't happen" << std::endl;
					logFile.close();
					DisplayErrorMessageBox();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}

				// now that we verified the pak exists, move patch to sub directory in pak directory
				try {
					if(GetFileAttributes((std::wstring(buffer) + L"\\BossRandomizer").c_str()) == INVALID_FILE_ATTRIBUTES){
						CreateDirectory((std::wstring(buffer) + L"\\BossRandomizer").c_str(), NULL);
					}
					std::wstring pakPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\RandomizedBosses_P.pak";
					std::wstring spoilerPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\Boss Randomizer Spoilers.txt";
					CopyFile(L".\\RandomizedBosses_P.pak", pakPathLPC.c_str(), FALSE);
					CopyFile(L".\\Boss Randomizer Spoilers.txt", spoilerPathLPC.c_str(), FALSE);
					logFile << "Patch File copied successfully" << std::endl;
				}
				catch (std::exception& e) {
					logFile << "Error Copying Patch File: " << e.what() << "\n";
					logFile.close();
					DisplayErrorMessageBox();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}

				// Filler for future music selection

				// Set progress
				SendMessage(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), PBM_SETPOS, 100, 0);

				// Output prompt
				MessageBox(hwnd, L"Randomization Complete, enjoy!", L"Randomizing Done",  MB_OK);

				// Hide progress bar
				ShowWindow(GetDlgItem(hwnd, IDP_RANDOMIZEBAR), SW_HIDE);
			}
		}
		break;
		}
		return 0L;
		break;
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}