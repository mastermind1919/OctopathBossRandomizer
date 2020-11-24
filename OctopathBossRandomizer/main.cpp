// enable syslink in dialogs
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif

// use resources
#include "resource1.h"

// Prevent all the extra stuff in windows.h
#define WIN32_LEAN_AND_MEAN

// Required Headers
#include "Octopath.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <strsafe.h>
#include <ShObjIdl.h>
#include <winbase.h>
#include <processthreadsapi.h>

// Global Variables for dialog boxes
intvector configs(18);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

// For tristate radial buttons
void triStateCheck(HWND hwnd, int checkedResource, int uncheckedResource1, int uncheckedResource2, HWND checkedString, HWND uncheckedString1, HWND uncheckedString2) {
	CheckDlgButton(hwnd, checkedResource, BST_CHECKED);
	CheckDlgButton(hwnd, uncheckedResource1, BST_UNCHECKED);
	CheckDlgButton(hwnd, uncheckedResource2, BST_UNCHECKED);
	ShowWindow(checkedString, SW_SHOW);
	ShowWindow(uncheckedString1, SW_HIDE);
	ShowWindow(uncheckedString2, SW_HIDE);
}
// For duostate radial buttons
void duoStateCheck(HWND hwnd, int checkedResource, int uncheckedResource, HWND checkedString, HWND uncheckedString) {
	CheckDlgButton(hwnd, checkedResource, BST_CHECKED);
	CheckDlgButton(hwnd, uncheckedResource, BST_UNCHECKED);
	ShowWindow(checkedString, SW_SHOW);
	ShowWindow(uncheckedString, SW_HIDE);
}

// Create text strings
HWND createTextString(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	// Create the text strings for the interface
	HWND hwndString = CreateWindow(
		L"Static",
		name,
		WS_VISIBLE | WS_CHILD,
		startx,
		starty,
		sizex,
		sizey,
		hwnd,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);
	return hwndString;
}

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
// Create Radial buttons
HWND createRadioButton(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	HWND checkButton = CreateWindow(
		L"Button",
		name,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON | BS_LEFT,
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
	if (!hwndTool || !hwndTip) {
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
	WCHAR version[10]; // Version should never be longer than 6 bytes
	LoadString(hInstance, IDS_APP_NAME, title, bufferSize);
	LoadString(hInstance, IDS_APP_CLASS, className, (bufferSize - 18));
	LoadString(hInstance, IDV_VERSION, version, 10);

	// Combine the version and class name to show both in title
	WCHAR classVersion[MAX_LOADSTRING];
	swprintf_s(classVersion, L"%s %s", className, version);

	HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALLICON));

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hIcon = icon;
	wc.hInstance = hInstance;
	wc.lpszClassName = title;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);

	RegisterClass(&wc);


	// Create the main window.
	HWND hwnd = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		title,
		classVersion,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		560,
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

// Typical dialog callback
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

// Dialog callback for config syncronizer
INT_PTR CALLBACK ConfigDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_INITDIALOG:
	{
		// string for storing config
		std::wstring configout;
		// String for hashing for config
		std::wstring hashing = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::wstring digits = L"0123456789";
		// Set config string based on current config;
		// Mixing Options
		switch (configs[0]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(17)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(15)));
			break;
		case 2:
			configout.append(std::wstring(1, hashing.at(14)));
			break;
		}
		// Randomize Shrine Options
		switch (configs[1]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(1)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(7)));
			break;
		case 2:
			configout.append(std::wstring(1, hashing.at(4)));
			break;
		}

		// Randomize Gate Options
		switch (configs[2]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(7)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(20)));
			break;
		case 2:
			configout.append(std::wstring(1, hashing.at(23)));
			break;
		}

		// Galdera Options
		switch (configs[3]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(21)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(6)));
			break;
		}

		// PC win condition 
		switch (configs[4]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(3)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(0)));
			break;
		}

		// Full Random
		configout.append(std::wstring(1, digits.at(configs[5])));

		// Solo Random
		configout.append(std::wstring(1, digits.at(configs[6])));

		// Force Bosses 
		configout.append(std::wstring(1, digits.at(configs[7])));

		// Include Duplicates
		switch (configs[8]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(12)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(4)));
			break;
		}

		// Forcing PC option
		switch (configs[9]) {
		case 0:
			configout.append(std::wstring(1, hashing.at(8)));
			break;
		case 1:
			configout.append(std::wstring(1, hashing.at(13)));
			break;
		case 2:
			configout.append(std::wstring(1, hashing.at(19)));
			break;
		}
		// Force PC and Force Boss Options
		configout.append(std::wstring(1, digits.at(configs[10])));
		configout.append(std::wstring(1, digits.at(configs[11])));
		configout.append(std::wstring(1, digits.at(configs[12])));
		configout.append(std::wstring(1, digits.at(configs[13])));
		configout.append(std::wstring(1, digits.at(configs[14])));
		configout.append(std::wstring(1, digits.at(configs[15])));
		configout.append(std::wstring(1, digits.at(configs[16])));
		configout.append(std::wstring(1, digits.at(configs[17])));

		// Set the edit control in window using the pseudohashing of the config
		SendMessage(GetDlgItem(hwnd, IDE_CONFIGEDIT), WM_SETTEXT, 0, (LPARAM)configout.c_str());
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			std::wstring hashing = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			// Set the config based on the input, with default on unspecified character
			// Get string from the edit control
			LRESULT len = SendMessage(GetDlgItem(hwnd, IDE_CONFIGEDIT), WM_GETTEXTLENGTH, 0, 0);
			WCHAR* buffer = new WCHAR[len];
			SendMessage(GetDlgItem(hwnd, IDE_CONFIGEDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
			// Get size of buffer, if less than 18, set defaults
			// Give allowance for spaces as long as they are at the end
			if (len >= 18) {
				// Mixing Options
				if (buffer[0] == hashing.at(17)) {
					configs[0] = 0;
				}
				else if (buffer[0] == hashing.at(15)) {
					configs[0] = 1;
				}
				else if (buffer[0] == hashing.at(14)) {
					configs[0] = 2;
				}
				else {
					configs[0] = 0;
				}

				// Randomize Shrine Options
				if (buffer[1] == hashing.at(1)) {
					configs[1] = 0;
				}
				else if (buffer[1] == hashing.at(7)) {
					configs[1] = 1;
				}
				else if (buffer[1] == hashing.at(4)) {
					configs[1] = 2;
				}
				else {
					configs[1] = 1;
				}

				// Randomize Gate Options
				if (buffer[2] == hashing.at(7)) {
					configs[2] = 0;
				}
				else if (buffer[2] == hashing.at(20)) {
					configs[2] = 1;
				}
				else if (buffer[2] == hashing.at(23)) {
					configs[2] = 2;
				}
				else {
					configs[2] = 1;
				}

				// Galdera Options
				if (buffer[3] == hashing.at(21)) {
					configs[3] = 0;
				}
				else if (buffer[3] == hashing.at(6)) {
					configs[3] = 1;
				}
				else {
					configs[3] = 0;
				}

				// PC win condition 
				if (buffer[4] == hashing.at(3)) {
					configs[4] = 0;
				}
				else if (buffer[4] == hashing.at(0)) {
					configs[4] = 1;
				}
				else {
					configs[4] = 0;
				}

				// Full Random
				configs[5] = _wtoi(std::wstring(1, buffer[5]).c_str());

				// Solo Random
				configs[6] = _wtoi(std::wstring(1, buffer[6]).c_str());

				// Force Bosses 
				configs[7] = _wtoi(std::wstring(1, buffer[7]).c_str());

				// Include Duplicates
				if (buffer[8] == hashing.at(12)) {
					configs[8] = 0;
				}
				else if (buffer[8] == hashing.at(4)) {
					configs[8] = 1;
				}
				else {
					configs[8] = 0;
				}

				// Forcing PC option
				if (buffer[9] == hashing.at(8)) {
					configs[9] = 0;
				}
				else if (buffer[9] == hashing.at(13)) {
					configs[9] = 1;
				}
				else if (buffer[9] == hashing.at(19)) {
					configs[9] = 2;
				}
				else {
					configs[9] = 1;
				}

				// Force PC character
				configs[10] = _wtoi(std::wstring(1, buffer[10]).c_str());

				// Force Boss Options
				configs[11] = _wtoi(std::wstring(1, buffer[11]).c_str());
				configs[12] = _wtoi(std::wstring(1, buffer[12]).c_str());
				configs[13] = _wtoi(std::wstring(1, buffer[13]).c_str());
				configs[14] = _wtoi(std::wstring(1, buffer[14]).c_str());
				configs[15] = _wtoi(std::wstring(1, buffer[15]).c_str());
				configs[16] = _wtoi(std::wstring(1, buffer[16]).c_str());
				configs[17] = _wtoi(std::wstring(1, buffer[17]).c_str());
			}
			else {
				configs[0] = 0;
				configs[1] = 1;
				configs[2] = 1;
				configs[3] = 0;
				configs[4] = 0;
				configs[5] = 0;
				configs[6] = 0;
				configs[7] = 0;
				configs[8] = 0;
				configs[9] = 1;
				configs[10] = 0;
				configs[11] = 0;
				configs[12] = 0;
				configs[13] = 0;
				configs[14] = 0;
				configs[15] = 0;
				configs[16] = 0;
				configs[17] = 0;
			}
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	}
	return DefWindowProc(hwnd, Message, wParam, lParam);
}

// Dialog box for Force Boss Options
BOOL CALLBACK OptionDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_INITDIALOG:
	{
		// Set up combo boxes
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_SETCURSEL, configs[11], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_SETCURSEL, configs[12], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_SETCURSEL, configs[13], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_SETCURSEL, configs[14], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_SETCURSEL, configs[15], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_SETCURSEL, configs[16], 0);
		configs[17] == 1 ? CheckDlgButton(hwnd, IDC_FORCEGALDERA, BST_CHECKED) : CheckDlgButton(hwnd, IDC_FORCEGALDERA, BST_UNCHECKED);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			// Post changes to config
			configs[11] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_GETCURSEL, 0, 0);
			configs[12] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_GETCURSEL, 0, 0);
			configs[13] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_GETCURSEL, 0, 0);
			configs[14] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_GETCURSEL, 0, 0);
			configs[15] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_GETCURSEL, 0, 0);
			configs[16] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_GETCURSEL, 0, 0);
			(bool)IsDlgButtonChecked(hwnd, IDC_FORCEGALDERA) == true ? configs[17] = 1 : configs[17] = 0;
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

// Main window callback, all the important stuff happens here
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
	{
		// Create the text strings for the interface
		HWND pakDIR = CreateWindow(
			L"Static",
			L"Octopath Traveler Pak Directory:",
			WS_VISIBLE | WS_CHILD,
			10,
			10,
			200,
			15,
			hwnd,
			NULL,
			NULL,
			NULL
		);

		HWND seedText = CreateWindow(
			L"Static",
			L"Seed (Leave blank for random seed):",
			WS_VISIBLE | WS_CHILD,
			560,
			10,
			200,
			15,
			hwnd,
			NULL,
			NULL,
			NULL
		);

		// Create all other necessary elements
		// File text box with input
		HWND pakEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES,
			L"Edit",
			TEXT(""),
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			10,
			28,
			447,
			21,
			hwnd,
			(HMENU)IDE_EDIT,
			NULL,
			NULL
		);

		// File Browser Button
		HWND fileButton = CreateWindow(
			L"Button",
			L"Browse",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER,
			460,
			28,
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
			330,
			447,
			90,
			33,
			hwnd,
			(HMENU)MAKEINTRESOURCE(IDB_RANDOMIZE_BUTTON),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
		);
		// Seed edit control
		HWND seedEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"Edit",
			TEXT(""),
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
			560,
			28,
			200,
			21,
			hwnd,
			(HMENU)IDE_SEED,
			NULL,
			NULL
		);
		// For some reason, entering above 9 characters causes an error
		// set 9 character limit for now
		SendMessage(seedEdit, EM_SETLIMITTEXT, (WPARAM)9, 0);
		// Config Sync Button
		HWND configSync = CreateWindow(
			L"Button",
			L"Config Sync",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER,
			660,
			447,
			90,
			33,
			hwnd,
			(HMENU)MAKEINTRESOURCE(IDB_CONFIGSYNC),
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL
		);

		// Buttons and text for the options
		HWND MixText = createTextString(L"Chapter Mixing Options:", 395, 65, 150, 15, hwnd, NULL);
		HWND defaultMix = createRadioButton(L"No Chapter Boss Pool Mixing", 400, 83, 170, 15, hwnd, MAKEINTRESOURCE(IDB_DEFAULTMIX));
		HWND mixChapter24 = createRadioButton(L"Mix Chapters 2 - 4 Boss Pools", 400, 101, 170, 15, hwnd, MAKEINTRESOURCE(IDB_MIXCHAPTER24));
		HWND mixChapter14 = createRadioButton(L"Mix Chapters 1 - 4 Boss Pools", 400, 119, 170, 15, hwnd, MAKEINTRESOURCE(IDB_MIXCHAPTER14));
		HWND ShrineText = createTextString(L"Shrine Boss Options:", 395, 140, 150, 15, hwnd, NULL);
		HWND noShrineRandomization = createRadioButton(L"No Shrine Boss Randomization", 400, 158, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NOSHRINE));
		HWND randomizeShrine = createRadioButton(L"Randomize Shrine Bosses", 400, 176, 170, 15, hwnd, MAKEINTRESOURCE(IDB_RANDOMSHRINE));
		HWND includeShrine = createRadioButton(L"Include Shrine Bosses", 400, 194, 170, 15, hwnd, MAKEINTRESOURCE(IDB_INCLUDESHRINE));
		HWND GateText = createTextString(L"Gate of Finis Boss Options:", 395, 215, 170, 15, hwnd, NULL);
		HWND noGateRandomization = createRadioButton(L"No Gate Boss Randomization", 400, 233, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NOGATE));
		HWND randomizeGate = createRadioButton(L"Randomize Gate Bosses", 400, 251, 170, 15, hwnd, MAKEINTRESOURCE(IDB_RANDOMGATE));
		HWND includeGate = createRadioButton(L"Include Gate Bosses", 400, 269, 170, 15, hwnd, MAKEINTRESOURCE(IDB_INCLUDEGATE));
		HWND GalderaText = createTextString(L"Galdera Options:", 395, 290, 170, 15, hwnd, NULL);
		HWND noGalderaRandomization = createRadioButton(L"No Galdera Randomization", 400, 308, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NOGALDERA));
		HWND includeGaldera = createRadioButton(L"Include Galdera", 400, 326, 170, 15, hwnd, MAKEINTRESOURCE(IDB_INCLUDEGALDERA));
		HWND DuplicateText = createTextString(L"Duplicate Boss Options:", 395, 347, 170, 15, hwnd, NULL);
		HWND noDupliates = createRadioButton(L"No Duplicates", 400, 365, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NODUPLICATE));
		HWND includeDupliates = createRadioButton(L"Allow Duplicates", 400, 383, 170, 15, hwnd, MAKEINTRESOURCE(IDB_INCLUDEDUPLICATE));
		HWND WinText = createTextString(L"Win condition:", 590, 65, 150, 15, hwnd, NULL);
		HWND PCWin = createRadioButton(L"Main PC Completion", 595, 83, 170, 15, hwnd, MAKEINTRESOURCE(IDB_PCWIN));
		HWND GalderaWin = createRadioButton(L"Gate of Finis Completion", 595, 101, 170, 15, hwnd, MAKEINTRESOURCE(IDB_GALDERAWIN));
		HWND SpecialOptionsText = createTextString(L"Special Options:", 590, 132, 150, 15, hwnd, NULL);
		HWND fullRandom = createCheckButton(L"Full Random", 595, 150, 170, 15, hwnd, MAKEINTRESOURCE(IDB_FULLRANDOM));
		HWND soloRandom = createCheckButton(L"Solo Traveler Randomizer", 595, 168, 170, 15, hwnd, MAKEINTRESOURCE(IDB_SOLORANDOM));
		HWND forceBoss = createCheckButton(L"Force Boss Tier", 595, 186, 100, 15, hwnd, MAKEINTRESOURCE(IDB_FORCEBOSS));
		HWND forceBossOptions = CreateWindow(L"Button", L"Options", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, 695, 186, 50, 19, hwnd, (HMENU)MAKEINTRESOURCE(IDB_FORCEBOSSOPTION), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		HWND ForcingOptionsText = createTextString(L"PC Forcing Options:", 590, 215, 170, 15, hwnd, NULL);
		HWND noForcingPC = createRadioButton(L"No PC Forcing", 595, 233, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NOFORCEPC));
		HWND forcingPC = createRadioButton(L"Force Random PC", 595, 251, 170, 15, hwnd, MAKEINTRESOURCE(IDB_FORCEPC));
		HWND specificPC = createRadioButton(L"Force Specific PC:", 595, 269, 110, 15, hwnd, MAKEINTRESOURCE(IDB_SPECIFICPC));
		HWND specificPCOption = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 610, 287, 70, 15, hwnd, (HMENU)MAKEINTRESOURCE(IDB_SPECIFICPCOPTION), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

		// Fill the combo box
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Ophilia"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Cyrus"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Tressa"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Olberic"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Primrose"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Alfyn"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Therion"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"H'annit"));
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_SETCURSEL, 0, 0);

		// Text strings for description box
		HWND descriptionString = createTextString(L"These are descriptions of the currently selected options.", 15, 67, 350, 30, hwnd, MAKEINTRESOURCE(IDS_DESCRIPTIONSTRING));
		HWND dashString1 = createTextString(L"*", 15, 103, 4, 15, hwnd, NULL);
		HWND dashString2 = createTextString(L"*", 15, 151, 4, 15, hwnd, NULL);
		HWND dashString3 = createTextString(L"*", 15, 169, 4, 15, hwnd, NULL);
		HWND dashString4 = createTextString(L"*", 15, 187, 4, 15, hwnd, NULL);
		HWND dashString5 = createTextString(L"*", 15, 205, 4, 15, hwnd, NULL);
		HWND dashString6 = createTextString(L"*", 15, 223, 4, 15, hwnd, NULL);
		HWND dashString7 = createTextString(L"*", 15, 256, 4, 15, hwnd, NULL);
		HWND dashString8 = createTextString(L"*", 15, 289, 4, 15, hwnd, NULL);
		HWND dashString9 = createTextString(L"*", 15, 326, 4, 15, hwnd, NULL);
		HWND dashString10 = createTextString(L"*", 15, 359, 4, 15, hwnd, NULL);
		HWND DefaultMixString = createTextString(L"Do not allow for Chapter 1 - 4 bosses pools to be mixed, so a Chapter 4 boss can never appear in a Chapter 2 area.", 20, 100, 350, 30, hwnd, MAKEINTRESOURCE(IDS_DEFAULTMIX));
		HWND MixChapter24String = createTextString(L"Allows for any boss in the Chapter 4 pool and include options to be mixed with Chapter 2 and Chapter 3 bosses, so you can get a Chapter 4 boss for a Chapter 2 area.", 20, 100, 350, 45, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER24));
		HWND MixChapter14String = createTextString(L"Allows for any boss in the Chapter 4 pool and include options to be mixed with Chapters 1 - 3, so yes, you can get a Chapter 4 or nastier boss for a first character.", 20, 100, 350, 45, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER14));
		HWND NoShrineString = createTextString(L"Do not randomize the Shrine Bosses in any way.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOSHRINE));
		HWND RandomizeShrineString = createTextString(L"Randomize the Shrine Bosses among themselves.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZESHRINE));
		HWND IncludeShrineString = createTextString(L"Include the Shrine Bosses in the Chapter 4 boss pool.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDESHRINE));
		HWND NoGateString = createTextString(L"Do not randomize the Gate of Finis Bosses in any way.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOGATE));
		HWND RandomizeGateString = createTextString(L"Randomize the Gate of Finis Bosses among themselves.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZEGATE));
		HWND IncludeGateString = createTextString(L"Include the Gate of Finis Bosses in the Chapter 4 boss pool.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGATE));
		HWND NoGalderaString = createTextString(L"Do not randomize Galdera.", 20, 184, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOGALDERA));
		HWND IncludeGalderaString = createTextString(L"Include Galdera in the Chapter 4 boss pool.", 20, 184, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGALDERA));
		HWND NoDuplicateString = createTextString(L"Do not allow for duplicate bosses, each one is unique.", 20, 202, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NODUPLICATE));
		HWND IncludeDuplicateString = createTextString(L"Allows for duplicate bosses, so a boss can show up multiple times.", 20, 202, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEDUPLICATE));
		HWND NoFullRandomString = createTextString(L"Full Random is Disabled", 20, 220, 350, 30, hwnd, MAKEINTRESOURCE(IDS_NOFULLRANDOM));
		HWND FullRandomString = createTextString(L"Ignores all normal randomization options and make each boss have an equal chance of showing up. Obeys forcing rules.", 20, 220, 345, 30, hwnd, MAKEINTRESOURCE(IDS_FULLRANDOM));
		HWND NoSoloRandomString = createTextString(L"Solo Traveler Randomization is Disabled", 20, 253, 350, 30, hwnd, MAKEINTRESOURCE(IDS_NOSOLORANDOM));
		HWND SoloRandomString = createTextString(L"Forces a random Solo Traveler to fight any one boss. It is recommended you get all travelers before continuing past chapter 2.", 20, 253, 345, 30, hwnd, MAKEINTRESOURCE(IDS_SOLORANDOM));
		HWND NoForcingString = createTextString(L"No boss forcing is done.", 20, 286, 345, 15, hwnd, MAKEINTRESOURCE(IDS_NOFORCEBOSS));
		HWND ForcingString = createTextString(L"Boss forcing is enabled, Check options for details.", 20, 286, 345, 15, hwnd, MAKEINTRESOURCE(IDS_FORCEBOSS));
		HWND NoForcingPCString = createTextString(L"Do not force a random PC", 20, 323, 345, 30, hwnd, MAKEINTRESOURCE(IDS_NOFORCEPC));
		HWND ForcingPCString = createTextString(L"Forces a random PC for a new game. This PC will always\nhave a Chapter 1 boss", 20, 323, 345, 30, hwnd, MAKEINTRESOURCE(IDS_FORCEPC));
		HWND SpecificPCString = createTextString(L"Forces the specified PC for a new game. This PC will always have a Chapter 1 boss", 20, 323, 345, 30, hwnd, MAKEINTRESOURCE(IDS_SPECIFICPC));
		HWND PCWinConditionString = createTextString(L"Credits roll upon completion of the main PC Story.", 20, 356, 345, 15, hwnd, MAKEINTRESOURCE(IDS_PCWIN));
		HWND GalderaWinConditionString = createTextString(L"Credits roll upon completing the Gate of Finis and defeating \"Galdera\".", 20, 356, 345, 15, hwnd, MAKEINTRESOURCE(IDS_GALDERAWIN));

		// Disable all the strings
		ShowWindow(DefaultMixString, SW_HIDE);
		ShowWindow(MixChapter24String, SW_HIDE);
		ShowWindow(MixChapter14String, SW_HIDE);
		ShowWindow(NoShrineString, SW_HIDE);
		ShowWindow(RandomizeShrineString, SW_HIDE);
		ShowWindow(IncludeShrineString, SW_HIDE);
		ShowWindow(NoGateString, SW_HIDE);
		ShowWindow(RandomizeGateString, SW_HIDE);
		ShowWindow(IncludeGateString, SW_HIDE);
		ShowWindow(NoGalderaString, SW_HIDE);
		ShowWindow(IncludeGalderaString, SW_HIDE);
		ShowWindow(NoDuplicateString, SW_HIDE);
		ShowWindow(IncludeDuplicateString, SW_HIDE);
		ShowWindow(NoFullRandomString, SW_HIDE);
		ShowWindow(FullRandomString, SW_HIDE);
		ShowWindow(NoSoloRandomString, SW_HIDE);
		ShowWindow(SoloRandomString, SW_HIDE);
		ShowWindow(NoForcingString, SW_HIDE);
		ShowWindow(ForcingString, SW_HIDE);
		ShowWindow(NoForcingPCString, SW_HIDE);
		ShowWindow(ForcingPCString, SW_HIDE);
		ShowWindow(SpecificPCString, SW_HIDE);
		ShowWindow(PCWinConditionString, SW_HIDE);
		ShowWindow(GalderaWinConditionString, SW_HIDE);


		// With all options and windows created, load from config
		std::wstring pakDir;
		configs = configParser(".\\working\\config.txt", &pakDir);
		// Set the options and the respective text entry
		// Mixing Options
		switch (configs[0]) {
		case 0:
			CheckDlgButton(hwnd, IDB_DEFAULTMIX, BST_CHECKED);
			ShowWindow(DefaultMixString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_MIXCHAPTER24, BST_CHECKED);
			ShowWindow(MixChapter24String, SW_SHOW);
			break;
		case 2:
			CheckDlgButton(hwnd, IDB_MIXCHAPTER14, BST_CHECKED);
			ShowWindow(MixChapter14String, SW_SHOW);
			break;
		}
		// Randomize Shrine Options
		switch (configs[1]) {
		case 0:
			CheckDlgButton(hwnd, IDB_NOSHRINE, BST_CHECKED);
			ShowWindow(NoShrineString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_RANDOMSHRINE, BST_CHECKED);
			ShowWindow(RandomizeShrineString, SW_SHOW);
			break;
		case 2:
			CheckDlgButton(hwnd, IDB_INCLUDESHRINE, BST_CHECKED);
			ShowWindow(IncludeShrineString, SW_SHOW);
			break;
		}

		// Randomize Gate Options
		switch (configs[2]) {
		case 0:
			CheckDlgButton(hwnd, IDB_NOGATE, BST_CHECKED);
			ShowWindow(NoGateString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_RANDOMGATE, BST_CHECKED);
			ShowWindow(RandomizeGateString, SW_SHOW);
			break;
		case 2:
			CheckDlgButton(hwnd, IDB_INCLUDEGATE, BST_CHECKED);
			ShowWindow(IncludeGateString, SW_SHOW);
			break;
		}

		// Galdera Options
		switch (configs[3]) {
		case 0:
			CheckDlgButton(hwnd, IDB_NOGALDERA, BST_CHECKED);
			ShowWindow(NoGalderaString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_INCLUDEGALDERA, BST_CHECKED);
			ShowWindow(IncludeGalderaString, SW_SHOW);
			break;
		}

		// PC win condition 
		switch (configs[4]) {
		case 0:
			CheckDlgButton(hwnd, IDB_PCWIN, BST_CHECKED);
			ShowWindow(PCWinConditionString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_GALDERAWIN, BST_CHECKED);
			ShowWindow(GalderaWinConditionString, SW_SHOW);
			break;
		}

		// Full Random
		configs[5] == 1 ? CheckDlgButton(hwnd, IDB_FULLRANDOM, BST_CHECKED) : CheckDlgButton(hwnd, IDB_FULLRANDOM, BST_UNCHECKED);
		configs[5] == 1 ? ShowWindow(FullRandomString, SW_SHOW) : ShowWindow(NoFullRandomString, SW_SHOW);

		// Solo Random
		configs[6] == 1 ? CheckDlgButton(hwnd, IDB_SOLORANDOM, BST_CHECKED) : CheckDlgButton(hwnd, IDB_SOLORANDOM, BST_UNCHECKED);
		configs[6] == 1 ? ShowWindow(SoloRandomString, SW_SHOW) : ShowWindow(NoSoloRandomString, SW_SHOW);

		// Force Bosses 
		configs[7] == 1 ? CheckDlgButton(hwnd, IDB_FORCEBOSS, BST_CHECKED) : CheckDlgButton(hwnd, IDB_FORCEBOSS, BST_UNCHECKED);
		configs[7] == 1 ? ShowWindow(ForcingString, SW_SHOW) : ShowWindow(NoForcingString, SW_SHOW);

		// Duplicates 
		switch (configs[8]) {
		case 0:
			CheckDlgButton(hwnd, IDB_NODUPLICATE, BST_CHECKED);
			ShowWindow(NoDuplicateString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_INCLUDEDUPLICATE, BST_CHECKED);
			ShowWindow(IncludeDuplicateString, SW_SHOW);
			break;
		}

		// Forcing PC option
		switch (configs[9]) {
		case 0:
			CheckDlgButton(hwnd, IDB_NOFORCEPC, BST_CHECKED);
			ShowWindow(NoForcingPCString, SW_SHOW);
			break;
		case 1:
			CheckDlgButton(hwnd, IDB_FORCEPC, BST_CHECKED);
			ShowWindow(ForcingPCString, SW_SHOW);
			break;
		case 2:
			CheckDlgButton(hwnd, IDB_SPECIFICPC, BST_CHECKED);
			ShowWindow(SpecificPCString, SW_SHOW);
			break;
		}
		// Force PC character options
		SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_SETCURSEL, configs[10], 0);
		// Forcing bosses is done in the dialog

		// Enforce option box greying out if certain items are selected
		// full random config
		if (configs[5] == 1) {
			EnableWindow(defaultMix, false);
			EnableWindow(mixChapter24, false);
			EnableWindow(mixChapter14, false);
			EnableWindow(noShrineRandomization, false);
			EnableWindow(randomizeShrine, false);
			EnableWindow(includeShrine, false);
			EnableWindow(noGateRandomization, false);
			EnableWindow(randomizeGate, false);
			EnableWindow(includeGate, false);
			EnableWindow(noGalderaRandomization, false);
			EnableWindow(includeGaldera, false);
			EnableWindow(noDupliates, false);
			EnableWindow(includeDupliates, false);
			EnableWindow(DefaultMixString, false);
			EnableWindow(MixChapter24String, false);
			EnableWindow(MixChapter14String, false);
			EnableWindow(NoShrineString, false);
			EnableWindow(RandomizeShrineString, false);
			EnableWindow(IncludeShrineString, false);
			EnableWindow(NoGateString, false);
			EnableWindow(RandomizeGateString, false);
			EnableWindow(IncludeGateString, false);
			EnableWindow(NoGalderaString, false);
			EnableWindow(IncludeGalderaString, false);
			EnableWindow(NoDuplicateString, false);
			EnableWindow(IncludeDuplicateString, false);
		}
		// Force Boss Options
		if (configs[7] != 1) {
			EnableWindow(forceBossOptions, false);
		}
		if (configs[9] != 2) {
			EnableWindow(specificPCOption, false);
		}

		SendDlgItemMessage(hwnd, IDE_EDIT, WM_SETTEXT, 0, (LPARAM)pakDir.c_str());
	}
	// Set font for child windows
	EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

	break;
	case WM_DESTROY:
	{
		// Get current selection from force specific PC
		configs[10] = (int)SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_GETCURSEL, 0, 0);
		// Save configs to file
		LRESULT len = SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXTLENGTH, 0, 0);
		WCHAR* buffer = new WCHAR[len];
		SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
		configWriter(".\\working\\config.txt", configs, buffer);
		PostQuitMessage(0);
		return 0;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rectange;
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// Draw borders for the description box
		SetRect(&rectange, 10, 62, 375, 440);
		DrawEdge(hdc, &rectange, EDGE_SUNKEN, BF_RECT | BF_FLAT);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == GetDlgItem(hwnd, IDS_DESCRIPTIONSTRING)) {
			// Make text a sight gray color
			//SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(100, 100, 100));
		}
		[[fallthrough]];
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
			// Main button, does all the randomization
		case IDB_RANDOMIZE_BUTTON:
		{
			// Check the pak path first
			// Retrieve pak path from edit control
			LRESULT len = SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXTLENGTH, 0, 0);
			WCHAR* buffer = new WCHAR[len];
			SendMessage(GetDlgItem(hwnd, IDE_EDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
			// Verify that the pak path does indeed contain the Octopath pak
			// Also allow for not inputing anything, and place folder in exe direcotry
			DWORD pakAttrib = GetFileAttributes((std::wstring(buffer) + L"\\Octopath_Traveler-WindowsNoEditor.pak").c_str());
			if (pakAttrib == INVALID_FILE_ATTRIBUTES && len != 0) {
				MessageBox(hwnd, L"Octopath Pak file not found in pak path.\nView usage for more details", L"Error Randomizing", MB_ICONEXCLAMATION | MB_OK);
			}
			else {
				// Get on with the randomization
				// Setup random number generator

				// Take input from seed edit box, use random one from device otherwise
				LRESULT seedLen = SendMessage(GetDlgItem(hwnd, IDE_SEED), WM_GETTEXTLENGTH, 0, 0);
				std::random_device dev;
				std::mt19937 rng;
				unsigned int seed;
				if (seedLen != 0) {
					WCHAR* seedBuffer = new WCHAR[seedLen];
					SendMessage(GetDlgItem(hwnd, IDE_SEED), WM_GETTEXT, (WPARAM)seedLen + 1, (LPARAM)seedBuffer);
					seed = std::stoi(seedBuffer);
					rng.seed(seed);
				}
				else {
					seed = dev();
					rng.seed(seed);
				}
				// Open log file
				std::wofstream logFile;
				logFile.open(L".\\working\\log.txt");

				// Check for pak exe in bin folder, return error if not found
				DWORD unrealAttrib = GetFileAttributes(L".\\working\\v4\\2\\3\\UnrealPak.exe");
				if (unrealAttrib != INVALID_FILE_ATTRIBUTES) {
					logFile << L"Found Unreal pak tool" << std::endl;
				}
				else {
					logFile << L"Unreal Pak tool not found, Exiting" << std::endl;
					DisplayErrorMessageBox();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}

				// Get the Character from the Specific Character Box
				configs[10] = (int)SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_GETCURSEL, 0, 0);
				// Add one to the config for specific character, 0 if specific character is disabled
				int specificCharacter = 0;
				if (configs[9] == 2) {
					specificCharacter = configs[10] + 1;
				}
				else if (configs[9] == 1) {
					// Generate Random Character for forcing
					std::uniform_int_distribution <std::mt19937::result_type> characters(1, 8);
					specificCharacter = characters(rng);
				}
				else {
					specificCharacter = 0;
				}

				// If checked, start with force randomization first
				vectorvector fixedBosses(7);
				if (configs[7] == 1) {
					fixedBosses = fixedTier(rng, configs);
				}
				// Randomize the bosses
				vectorvector randomizedLists(7);
				// Set bool options based on config
				bool mixChapter24;
				bool mixChapter14;
				bool randomizeShrine;
				bool includeShrine;
				bool randomizeGate;
				bool includeGate;
				bool includeGaldera;
				bool includeDuplicate;
				bool fullRandom;
				configs[0] == 1 ? mixChapter24 = true : mixChapter24 = false;
				configs[0] == 2 ? mixChapter14 = true : mixChapter14 = false;
				configs[1] == 1 ? randomizeShrine = true : randomizeShrine = false;
				configs[1] == 2 ? includeShrine = true : includeShrine = false;
				configs[2] == 1 ? randomizeGate = true : randomizeGate = false;
				configs[2] == 2 ? includeGate = true : includeGate = false;
				configs[3] == 1 ? includeGaldera = true : includeGaldera = false;
				configs[8] == 1 ? includeDuplicate = true : includeDuplicate = false;
				configs[5] == 1 ? fullRandom = true : fullRandom = false;
				logFile << "Randomzing bosses..." << std::endl;
				randomizedLists = randomizeBosses(rng, fixedBosses, specificCharacter, mixChapter14, mixChapter24, randomizeShrine, includeShrine, randomizeGate, includeGate, includeGaldera, includeDuplicate, fullRandom);

				// Now that the bosses are randomized, put them into the files
				// Ensure that the file structure is there
				if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\Event\\json") == INVALID_FILE_ATTRIBUTES) {
					if (GetFileAttributes(L".\\working\\Octopath_Traveler") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\Event") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\Event", NULL);
					}
					CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\Event\\json", NULL);
				}
				// Write random bosses to files
				bool soloTraveler;
				configs[6] == 1 ? soloTraveler = true : soloTraveler = false;
				logFile << "Writing random bosses to files" << std::endl;
				int errorCheck = randomToFile(rng, randomizedLists, soloTraveler, seed, configs[4]);
				if (errorCheck == 1) {
					// Error out
					logFile << L"Error in writing to files." << std::endl;
					DisplayErrorMessageBox();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// If force pc is set, main menu files to directory
				// Get current character, and translate it to a file
				// Make sure that directory is there, create otherwise
				if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts") == INVALID_FILE_ATTRIBUTES) {
					if (GetFileAttributes(L".\\working\\Octopath_Traveler") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\UI") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\UI", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\UI\\Title") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\UI\\Title", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP", NULL);
					}
					if (GetFileAttributes(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene") == INVALID_FILE_ATTRIBUTES) {
						CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene", NULL);
					}
					CreateDirectory(L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts", NULL);
				}
				std::map<int, std::wstring> characterNames;
				characterNames[0] = L"All";
				characterNames[1] = L"Ophilia";
				characterNames[2] = L"Cyrus";
				characterNames[3] = L"Tressa";
				characterNames[4] = L"Olberic";
				characterNames[5] = L"Primrose";
				characterNames[6] = L"Alfyn";
				characterNames[7] = L"Therion";
				characterNames[8] = L"H'annit";

				logFile << "Setting main menu to " << characterNames[specificCharacter] << std::endl;
				std::wstring characterPath = L".\\working\\UI\\" + characterNames[specificCharacter] + L"\\TitlePlayerSelect.uasset";
				std::wstring charactersinglePath = L".\\working\\UI\\" + characterNames[specificCharacter] + L"\\ItemPlayerSelect.uasset";
				// Move the character UI to the files
				CopyFile(characterPath.c_str(), L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\TitlePlayerSelect.uasset", FALSE);
				CopyFile(charactersinglePath.c_str(), L".\\working\\Octopath_Traveler\\Content\\UI\\Title\\BP\\Scene\\Parts\\ItemPlayerSelect.uasset", FALSE);

				// try-catch statement for the execution, again to prevent errors
				try {
					logFile << "************************************" << std::endl;
					logFile << "Pak tool" << std::endl;
					logFile << "************************************" << std::endl;
					WCHAR fullFilename[MAX_PATH];
					GetFullPathName(L".\\working\\randomizeboss.txt", MAX_PATH, fullFilename, nullptr);
					WCHAR command[MAX_PATH + 100];
					// Copy to char the path name
					swprintf_s(command, L".\\working\\v4\\2\\3\\UnrealPak.exe ..\\..\\..\\RandomizedBosses_P.pak -Create=\"%s\" -compress", fullFilename);
					// for CreateProccess
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					ZeroMemory(&pi, sizeof(pi));

					// try using CreateProcess for pak tool, without cmd window created
					CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

					// Wait for the pak tool to complete
					WaitForSingleObject(pi.hProcess, INFINITE);

					// Close process and thread handles
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
				catch (const std::exception& e) {
					std::cerr << e.what();
					logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Check if pak file was created, should always pass
				if (GetFileAttributes(L".\\working\\RandomizedBosses_P.pak") == INVALID_FILE_ATTRIBUTES) {
					logFile << "Pak file not created, This shouldn't happen" << std::endl;
					logFile.close();
					DisplayErrorMessageBox();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Place pak folder in root
				logFile << "Moving pak files to BossRandomizer" << std::endl;
				if (len == 0) {
					try {
						if (GetFileAttributes(L".\\BossRandomizer") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\BossRandomizer", NULL);
						}
						if (GetFileAttributes(L".\\Spoilers") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\Spoilers", NULL);
						}
						if (GetFileAttributes(L".\\BossRandomizer\\Spoilers") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\BossRandomizer\\Spoilers", NULL);
						}
						CopyFile(L".\\working\\RandomizedBosses_P.pak", L".\\BossRandomizer\\RandomizedBosses_P.pak", FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", L".\\BossRandomizer\\Spoilers\\Boss Randomizer Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", L".\\Spoilers\\Boss Randomizer Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", L".\\BossRandomizer\\Spoilers\\Chapter 1 Boss Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", L".\\Spoilers\\Chapter 1 Boss Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Seed.txt", L".\\BossRandomizer\\Spoilers\\Seed.txt", FALSE);
						CopyFile(L".\\working\\Seed.txt", L".\\Spoilers\\Seed.txt", FALSE);
						// Clean up root directory
						DeleteFile(L".\\working\\RandomizedBosses_P.pak");
						DeleteFile(L".\\working\\Boss Randomizer Spoilers.txt");
						DeleteFile(L".\\working\\Chapter 1 Boss Spoilers.txt");
						DeleteFile(L".\\working\\Seed.txt");
						logFile << "Patch Files Left in Program Directory" << std::endl;
					}
					catch (std::exception& e) {
						logFile << "Error Copying Patch File: " << e.what() << "\n";
						logFile.close();
						DisplayErrorMessageBox();
						SendMessage(hwnd, WM_DESTROY, 0, 0);
					}
				}
				else {
					// now that we verified the pak exists, move patch to sub directory in pak directory
					try {
						if (GetFileAttributes((std::wstring(buffer) + L"\\BossRandomizer").c_str()) == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory((std::wstring(buffer) + L"\\BossRandomizer").c_str(), NULL);
						}
						if (GetFileAttributes((std::wstring(buffer) + L"\\BossRandomizer\\Spoilers").c_str()) == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory((std::wstring(buffer) + L"\\BossRandomizer\\Spoilers").c_str(), NULL);
						}
						if (GetFileAttributes(L".\\working\\BossRandomizer") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\working\\BossRandomizer", NULL);
						}
						if (GetFileAttributes(L".\\Spoilers") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\Spoilers", NULL);
						}
						if (GetFileAttributes(L".\\working\\BossRandomizer\\Spoilers") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\working\\BossRandomizer\\Spoilers", NULL);
						}
						std::wstring pakPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\RandomizedBosses_P.pak";
						std::wstring spoilerPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\Spoilers\\Boss Randomizer Spoilers.txt";
						std::wstring chapter1SpoilerPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\Spoilers\\Chapter 1 Boss Spoilers.txt";
						std::wstring seedPathLPC = std::wstring(buffer) + L"\\BossRandomizer\\Spoilers\\Seed.txt";
						// Copy to local directory first, as a backup
						CopyFile(L".\\working\\RandomizedBosses_P.pak", L".\\working\\BossRandomizer\\RandomizedBosses_P.pak", FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", L".\\working\\BossRandomizer\\Spoilers\\Boss Randomizer Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", L".\\Spoilers\\Boss Randomizer Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", L".\\working\\BossRandomizer\\Spoilers\\Chapter 1 Boss Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", L".\\Spoilers\\Chapter 1 Boss Spoilers Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Seed.txt", L".\\working\\BossRandomizer\\Spoilers\\Seed.txt", FALSE);
						CopyFile(L".\\working\\Seed.txt", L".\\Spoilers\\Seed.txt", FALSE);
						// Copy to final destination
						CopyFile(L".\\working\\RandomizedBosses_P.pak", pakPathLPC.c_str(), FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", spoilerPathLPC.c_str(), FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", chapter1SpoilerPathLPC.c_str(), FALSE);
						CopyFile(L".\\working\\Seed.txt", seedPathLPC.c_str(), FALSE);
						// Clean up root directory
						DeleteFile(L".\\working\\RandomizedBosses_P.pak");
						DeleteFile(L".\\working\\Boss Randomizer Spoilers.txt");
						DeleteFile(L".\\working\\Chapter 1 Boss Spoilers.txt");
						DeleteFile(L".\\working\\Seed.txt");
						logFile << "Patch File copied successfully" << std::endl;
					}
					catch (std::exception& e) {
						logFile << "Error Copying Patch File: " << e.what() << "\n";
						logFile.close();
						DisplayErrorMessageBox();
						SendMessage(hwnd, WM_DESTROY, 0, 0);
					}
				}
				logFile << "Randomization completed without error." << std::endl;
				// Display message box for completion
				if (len == 0) {
					// Special output prompt for exe direcotry pak file
					MessageBox(hwnd, L"Randomization Complete, Move \"BossRandomizer\"\nto the pak dir to enjoy the randomized bosses.", L"Randomization Complete", MB_OK);
				}
				else {
					// Normal output prompt
					MessageBox(hwnd, L"Randomization Complete, enjoy!", L"Randomizing Done", MB_OK);
				}

			}
		}
		break;
		case IDM_ABOUT:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)UsageDlgProc);
			break;
		case IDM_HELP:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_HELP), hwnd, (DLGPROC)UsageDlgProc);
			break;
			// Force Boss Options
		case IDB_FORCEBOSSOPTION:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_FORCEOPTIONS), hwnd, (DLGPROC)OptionDlgProc);
			break;
			// Config Sync Button
		case IDB_CONFIGSYNC:
			// Draw config option from Specific PC combo box
			configs[10] = (int)SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_GETCURSEL, 0, 0);
			// Use a modal dialog box to prevent execution of the next lines until after closing
			DialogBox(NULL, MAKEINTRESOURCE(IDD_CONFIG), hwnd, (DLGPROC)ConfigDlgProc);
			// Make the UI reflect the config changes
			// Mixing Options
			switch (configs[0]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_DEFAULTMIX, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_MIXCHAPTER24, BN_CLICKED), 0);
				break;
			case 2:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_MIXCHAPTER14, BN_CLICKED), 0);
				break;
			}
			// Randomize Shrine Options
			switch (configs[1]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_NOSHRINE, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_RANDOMSHRINE, BN_CLICKED), 0);
				break;
			case 2:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_INCLUDESHRINE, BN_CLICKED), 0);
				break;
			}

			// Randomize Gate Options
			switch (configs[2]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_NOGATE, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_RANDOMGATE, BN_CLICKED), 0);
				break;
			case 2:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_INCLUDEGATE, BN_CLICKED), 0);
				break;
			}

			// Galdera Options
			switch (configs[3]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_NOGALDERA, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_INCLUDEGALDERA, BN_CLICKED), 0);
				break;
			}

			// PC win condition 
			switch (configs[4]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_PCWIN, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_GALDERAWIN, BN_CLICKED), 0);
				break;
			}

			// Full Random
			ShowWindow(GetDlgItem(hwnd, IDS_FULLRANDOM), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, IDS_NOFULLRANDOM), SW_HIDE);
			configs[5] == 1 ? CheckDlgButton(hwnd, IDB_FULLRANDOM, BST_CHECKED) : CheckDlgButton(hwnd, IDB_FULLRANDOM, BST_UNCHECKED);
			configs[5] == 1 ? ShowWindow(GetDlgItem(hwnd, IDS_FULLRANDOM), SW_SHOW) : ShowWindow(GetDlgItem(hwnd, IDS_NOFULLRANDOM), SW_SHOW);

			// Solo Random
			ShowWindow(GetDlgItem(hwnd, IDS_SOLORANDOM), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, IDS_NOSOLORANDOM), SW_HIDE);
			configs[6] == 1 ? CheckDlgButton(hwnd, IDB_SOLORANDOM, BST_CHECKED) : CheckDlgButton(hwnd, IDB_SOLORANDOM, BST_UNCHECKED);
			configs[6] == 1 ? ShowWindow(GetDlgItem(hwnd, IDS_SOLORANDOM), SW_SHOW) : ShowWindow(GetDlgItem(hwnd, IDS_NOSOLORANDOM), SW_SHOW);

			// Force Bosses 
			ShowWindow(GetDlgItem(hwnd, IDS_FORCEBOSS), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, IDS_NOFORCEBOSS), SW_HIDE);
			configs[7] == 1 ? CheckDlgButton(hwnd, IDB_FORCEBOSS, BST_CHECKED) : CheckDlgButton(hwnd, IDB_FORCEBOSS, BST_UNCHECKED);
			configs[7] == 1 ? ShowWindow(GetDlgItem(hwnd, IDS_FORCEBOSS), SW_SHOW) : ShowWindow(GetDlgItem(hwnd, IDS_NOFORCEBOSS), SW_SHOW);

			// Duplicates 
			switch (configs[8]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_NODUPLICATE, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_INCLUDEDUPLICATE, BN_CLICKED), 0);
				break;
			}

			// Forcing PC option
			switch (configs[9]) {
			case 0:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_NOFORCEPC, BN_CLICKED), 0);
				break;
			case 1:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_FORCEPC, BN_CLICKED), 0);
				break;
			case 2:
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDB_SPECIFICPC, BN_CLICKED), 0);
				break;
			}
			// Force PC character options
			SendMessage(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), CB_SETCURSEL, configs[10], 0);
			// Forcing bosses is done in the dialog
			break;
			// File Browser Button
		case IDB_FILE_BUTTON:
		{
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
		}
		// Radial button switching
		// Chapter Mixing
		case IDB_DEFAULTMIX:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_DEFAULTMIX, IDB_MIXCHAPTER24, IDB_MIXCHAPTER14, GetDlgItem(hwnd, IDS_DEFAULTMIX), GetDlgItem(hwnd, IDS_MIXCHAPTER24), GetDlgItem(hwnd, IDS_MIXCHAPTER14));
				configs[0] = 0;
				break;
			}
			break;
		case IDB_MIXCHAPTER24:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_MIXCHAPTER24, IDB_DEFAULTMIX, IDB_MIXCHAPTER14, GetDlgItem(hwnd, IDS_MIXCHAPTER24), GetDlgItem(hwnd, IDS_DEFAULTMIX), GetDlgItem(hwnd, IDS_MIXCHAPTER14));
				configs[0] = 1;
				break;
			}
			break;
		case IDB_MIXCHAPTER14:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_MIXCHAPTER14, IDB_MIXCHAPTER24, IDB_DEFAULTMIX, GetDlgItem(hwnd, IDS_MIXCHAPTER14), GetDlgItem(hwnd, IDS_MIXCHAPTER24), GetDlgItem(hwnd, IDS_DEFAULTMIX));
				configs[0] = 2;
				break;
			}
			break;
			// Shrine Boss Randomization
		case IDB_NOSHRINE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_NOSHRINE, IDB_RANDOMSHRINE, IDB_INCLUDESHRINE, GetDlgItem(hwnd, IDS_NOSHRINE), GetDlgItem(hwnd, IDS_RANDOMIZESHRINE), GetDlgItem(hwnd, IDS_INCLUDESHRINE));
				configs[1] = 0;
				break;
			}
			break;
		case IDB_RANDOMSHRINE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_RANDOMSHRINE, IDB_NOSHRINE, IDB_INCLUDESHRINE, GetDlgItem(hwnd, IDS_RANDOMIZESHRINE), GetDlgItem(hwnd, IDS_NOSHRINE), GetDlgItem(hwnd, IDS_INCLUDESHRINE));
				configs[1] = 1;
				break;
			}
			break;
		case IDB_INCLUDESHRINE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_INCLUDESHRINE, IDB_RANDOMSHRINE, IDB_NOSHRINE, GetDlgItem(hwnd, IDS_INCLUDESHRINE), GetDlgItem(hwnd, IDS_RANDOMIZESHRINE), GetDlgItem(hwnd, IDS_NOSHRINE));
				configs[1] = 2;
				break;
			}
			break;
			// Gate Boss Randomization
		case IDB_NOGATE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_NOGATE, IDB_RANDOMGATE, IDB_INCLUDEGATE, GetDlgItem(hwnd, IDS_NOGATE), GetDlgItem(hwnd, IDS_RANDOMIZEGATE), GetDlgItem(hwnd, IDS_INCLUDEGATE));
				configs[2] = 0;
				break;
			}
			break;
		case IDB_RANDOMGATE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_RANDOMGATE, IDB_NOGATE, IDB_INCLUDEGATE, GetDlgItem(hwnd, IDS_RANDOMIZEGATE), GetDlgItem(hwnd, IDS_NOGATE), GetDlgItem(hwnd, IDS_INCLUDEGATE));
				configs[2] = 1;
				break;
			}
			break;
		case IDB_INCLUDEGATE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_INCLUDEGATE, IDB_RANDOMGATE, IDB_NOGATE, GetDlgItem(hwnd, IDS_INCLUDEGATE), GetDlgItem(hwnd, IDS_RANDOMIZEGATE), GetDlgItem(hwnd, IDS_NOGATE));
				configs[2] = 2;
				break;
			}
			break;
			// Galdera Randomization
		case IDB_NOGALDERA:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_NOGALDERA, IDB_INCLUDEGALDERA, GetDlgItem(hwnd, IDS_NOGALDERA), GetDlgItem(hwnd, IDS_INCLUDEGALDERA));
				configs[3] = 0;
				break;
			}
			break;
		case IDB_INCLUDEGALDERA:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_INCLUDEGALDERA, IDB_NOGALDERA, GetDlgItem(hwnd, IDS_INCLUDEGALDERA), GetDlgItem(hwnd, IDS_NOGALDERA));
				configs[3] = 1;
				break;
			}
			break;
			// Duplicate Options
		case IDB_NODUPLICATE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_NODUPLICATE, IDB_INCLUDEDUPLICATE, GetDlgItem(hwnd, IDS_NODUPLICATE), GetDlgItem(hwnd, IDS_INCLUDEDUPLICATE));
				configs[8] = 0;
				break;
			}
			break;
		case IDB_INCLUDEDUPLICATE:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_INCLUDEDUPLICATE, IDB_NODUPLICATE, GetDlgItem(hwnd, IDS_INCLUDEDUPLICATE), GetDlgItem(hwnd, IDS_NODUPLICATE));
				configs[8] = 1;
				break;
			}
			break;
			// PC Win Conditions
		case IDB_PCWIN:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_PCWIN, IDB_GALDERAWIN, GetDlgItem(hwnd, IDS_PCWIN), GetDlgItem(hwnd, IDS_GALDERAWIN));
				configs[4] = 0;
				break;
			}
			break;
		case IDB_GALDERAWIN:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				duoStateCheck(hwnd, IDB_GALDERAWIN, IDB_PCWIN, GetDlgItem(hwnd, IDS_GALDERAWIN), GetDlgItem(hwnd, IDS_PCWIN));
				configs[4] = 1;
				break;
			}
			break;
			// Full Random
		case IDB_FULLRANDOM:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				if ((bool)IsDlgButtonChecked(hwnd, IDB_FULLRANDOM) == true) {
					ShowWindow(GetDlgItem(hwnd, IDS_NOFULLRANDOM), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDS_FULLRANDOM), SW_SHOW);
					configs[5] = 1;
					// Grey out other options
					EnableWindow(GetDlgItem(hwnd, IDB_DEFAULTMIX), false);
					EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER24), false);
					EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), false);
					EnableWindow(GetDlgItem(hwnd, IDB_NOSHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_RANDOMSHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_NOGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_RANDOMGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_NOGALDERA), false);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGALDERA), false);
					EnableWindow(GetDlgItem(hwnd, IDB_NODUPLICATE), false);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEDUPLICATE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_DEFAULTMIX), false);
					EnableWindow(GetDlgItem(hwnd, IDS_MIXCHAPTER24), false);
					EnableWindow(GetDlgItem(hwnd, IDS_MIXCHAPTER14), false);
					EnableWindow(GetDlgItem(hwnd, IDS_NOSHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_RANDOMIZESHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDESHRINE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_NOGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_RANDOMIZEGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEGATE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_NOGALDERA), false);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEGALDERA), false);
					EnableWindow(GetDlgItem(hwnd, IDS_NODUPLICATE), false);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEDUPLICATE), false);

				}
				else {
					ShowWindow(GetDlgItem(hwnd, IDS_NOFULLRANDOM), SW_SHOW);
					ShowWindow(GetDlgItem(hwnd, IDS_FULLRANDOM), SW_HIDE);
					configs[5] = 0;
					// Undo Greying out
					EnableWindow(GetDlgItem(hwnd, IDB_DEFAULTMIX), true);
					EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER24), true);
					EnableWindow(GetDlgItem(hwnd, IDB_MIXCHAPTER14), true);
					EnableWindow(GetDlgItem(hwnd, IDB_NOSHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_RANDOMSHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDESHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_NOGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_RANDOMGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_NOGALDERA), true);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEGALDERA), true);
					EnableWindow(GetDlgItem(hwnd, IDB_NODUPLICATE), true);
					EnableWindow(GetDlgItem(hwnd, IDB_INCLUDEDUPLICATE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_DEFAULTMIX), true);
					EnableWindow(GetDlgItem(hwnd, IDS_MIXCHAPTER24), true);
					EnableWindow(GetDlgItem(hwnd, IDS_MIXCHAPTER14), true);
					EnableWindow(GetDlgItem(hwnd, IDS_NOSHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_RANDOMIZESHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDESHRINE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_NOGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_RANDOMIZEGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEGATE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_NOGALDERA), true);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEGALDERA), true);
					EnableWindow(GetDlgItem(hwnd, IDS_NODUPLICATE), true);
					EnableWindow(GetDlgItem(hwnd, IDS_INCLUDEDUPLICATE), true);
				}
				break;
			}
			break;
			// Solo Traveler Randomization
		case IDB_SOLORANDOM:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				if ((bool)IsDlgButtonChecked(hwnd, IDB_SOLORANDOM) == true) {
					ShowWindow(GetDlgItem(hwnd, IDS_NOSOLORANDOM), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDS_SOLORANDOM), SW_SHOW);
					configs[6] = 1;
				}
				else {
					ShowWindow(GetDlgItem(hwnd, IDS_NOSOLORANDOM), SW_SHOW);
					ShowWindow(GetDlgItem(hwnd, IDS_SOLORANDOM), SW_HIDE);
					configs[6] = 0;
				}
				break;
			}
			break;
			// Boss Forcing
		case IDB_FORCEBOSS:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				if ((bool)IsDlgButtonChecked(hwnd, IDB_FORCEBOSS) == true) {
					ShowWindow(GetDlgItem(hwnd, IDS_NOFORCEBOSS), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDS_FORCEBOSS), SW_SHOW);
					// Enable boss options
					EnableWindow(GetDlgItem(hwnd, IDB_FORCEBOSSOPTION), true);
					configs[7] = 1;

				}
				else {
					ShowWindow(GetDlgItem(hwnd, IDS_NOFORCEBOSS), SW_SHOW);
					ShowWindow(GetDlgItem(hwnd, IDS_FORCEBOSS), SW_HIDE);
					// Disable boss options
					EnableWindow(GetDlgItem(hwnd, IDB_FORCEBOSSOPTION), false);
					configs[7] = 0;
				}
				break;
			}
			break;
		case IDB_NOFORCEPC:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_NOFORCEPC, IDB_FORCEPC, IDB_SPECIFICPC, GetDlgItem(hwnd, IDS_NOFORCEPC), GetDlgItem(hwnd, IDS_FORCEPC), GetDlgItem(hwnd, IDS_SPECIFICPC));
				// Enable disable specific PC
				EnableWindow(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), false);
				configs[9] = 0;
				break;
			}
			break;
		case IDB_FORCEPC:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_FORCEPC, IDB_NOFORCEPC, IDB_SPECIFICPC, GetDlgItem(hwnd, IDS_FORCEPC), GetDlgItem(hwnd, IDS_NOFORCEPC), GetDlgItem(hwnd, IDS_SPECIFICPC));
				// Enable disable specific PC
				EnableWindow(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), false);
				configs[9] = 1;
				break;
			}
			break;
		case IDB_SPECIFICPC:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				triStateCheck(hwnd, IDB_SPECIFICPC, IDB_FORCEPC, IDB_NOFORCEPC, GetDlgItem(hwnd, IDS_SPECIFICPC), GetDlgItem(hwnd, IDS_FORCEPC), GetDlgItem(hwnd, IDS_NOFORCEPC));
				// Enable specific PC options
				EnableWindow(GetDlgItem(hwnd, IDB_SPECIFICPCOPTION), true);
				configs[9] = 2;
				break;
			}
			break;
			return 0;

		}


	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}