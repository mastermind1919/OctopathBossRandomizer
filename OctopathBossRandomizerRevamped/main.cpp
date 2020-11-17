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

	HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL_ICON));

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
		HWND WinText = createTextString(L"Win condition:", 395, 347, 170, 15, hwnd, NULL);
		HWND PCWin = createRadioButton(L"Main PC Completion", 400, 365, 170, 15, hwnd, MAKEINTRESOURCE(IDB_PCWIN));
		HWND GalderaWin = createRadioButton(L"Gate of Finis Completion", 400, 383, 170, 15, hwnd, MAKEINTRESOURCE(IDB_GALDERAWIN));
		HWND SpecialOptionsText = createTextString(L"Special Options:", 590, 65, 150, 15, hwnd, NULL);
		HWND fullRandom = createCheckButton(L"Full Random", 595, 83, 170, 15, hwnd, MAKEINTRESOURCE(IDB_FULLRANDOM));
		HWND soloRandom = createCheckButton(L"Solo Traveler Randomizer", 595, 101, 170, 15, hwnd, MAKEINTRESOURCE(IDB_SOLORANDOM));
		HWND forceBoss = createCheckButton(L"Force Boss Tier", 595, 119, 100, 15, hwnd, MAKEINTRESOURCE(IDB_FORCEBOSS));
		HWND forceBossOptions = CreateWindow(L"Button", L"Options", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, 695, 117, 50, 19, hwnd, (HMENU)MAKEINTRESOURCE(IDB_FORCEBOSSOPTION), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		HWND ForcingOptionsText = createTextString(L"PC Forcing Options:", 590, 215, 170, 15, hwnd, NULL);
		HWND noForcingPC = createRadioButton(L"No PC Forcing", 595, 233, 170, 15, hwnd, MAKEINTRESOURCE(IDB_NOFORCE));
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
		HWND descriptionString = createTextString(L"These are descriptions of the currently selected options, hover over a new option to see it's difference.", 15, 67, 350, 30, hwnd, MAKEINTRESOURCE(IDS_DESCRIPTIONSTRING));
		HWND dashString1 = createTextString(L"*", 15, 103, 4, 15, hwnd, NULL);
		HWND dashString2 = createTextString(L"*", 15, 151, 4, 15, hwnd, NULL);
		HWND dashString3 = createTextString(L"*", 15, 169, 4, 15, hwnd, NULL);
		HWND dashString4 = createTextString(L"*", 15, 187, 4, 15, hwnd, NULL);
		HWND dashString5 = createTextString(L"*", 15, 205, 4, 15, hwnd, NULL);
		HWND dashString6 = createTextString(L"*", 15, 223, 4, 15, hwnd, NULL);
		HWND dashString7 = createTextString(L"*", 15, 256, 4, 15, hwnd, NULL);
		HWND dashString8 = createTextString(L"*", 15, 289, 4, 15, hwnd, NULL);
		HWND dashString9 = createTextString(L"*", 15, 308, 4, 15, hwnd, NULL);
		HWND dashString10 = createTextString(L"*", 15, 341, 4, 15, hwnd, NULL);
		HWND DefaultMixString = createTextString(L"Do not allow for Chapter 1 - 4 bosses pools to be mixed, so a Chapter 4 boss can never appear in a Chapter 2 area.", 20, 100, 350, 30, hwnd, MAKEINTRESOURCE(IDS_DEFAULTMIX));
		HWND MixChapter24String = createTextString(L"Allows for any boss in the Chapter 4 pool and include options to be mixed with Chapter 2 and Chapter 3 bosses, so you can get a Chapter 4 boss for a Chapter 2 area.", 20, 100, 350, 45, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER24));
		HWND MixChapter14String = createTextString(L"Allows for any boss in the Chapter 4 pool and include options to be mixed with Chapters 1 - 3, so yes, you can get a Chapter 4 or nastier boss for a first character.", 20, 100, 350, 45, hwnd, MAKEINTRESOURCE(IDS_MIXCHAPTER14));
		HWND NoShrineString = createTextString(L"Do not randomize the Shrine Bosses in any way.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOSHRINE));
		HWND RandomizeShrineString = createTextString(L"Randomize the Shrine Bosses among themselves.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZESHIRNE));
		HWND IncludeShrineString = createTextString(L"Include the Shrine Bosses in the Chapter 4 boss pool.", 20, 148, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDESHRINE));
		HWND NoGateString = createTextString(L"Do not randomize the Gate of Finis Bosses in any way.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOGATE));
		HWND RandomizeGateString = createTextString(L"Randomize the Gate of Finis Bosses among themselves.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_RANDOMIZEGATE));
		HWND IncludeGateString = createTextString(L"Include the Gate of Finis Bosses in the Chapter 4 boss pool.", 20, 166, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGATE));
		HWND NoGalderaString = createTextString(L"Do not randomize Galdera.", 20, 184, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NOGALDERA));
		HWND IncludeGalderaString = createTextString(L"Include Galdera in the Chapter 4 boss pool.", 20, 184, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEGALDERA));
		HWND NoDuplicateString = createTextString(L"Do not allow for duplicate bosses, each one is unique.", 20, 202, 350, 15, hwnd, MAKEINTRESOURCE(IDS_NODUPLICATE));
		HWND IncludeDuplicateString = createTextString(L"Allows for duplicate bosses, so a boss can show up multiple times.", 20, 202, 350, 15, hwnd, MAKEINTRESOURCE(IDS_INCLUDEDUPLICATE));
		HWND NoFullRandomString = createTextString(L"Full Random is Disabled", 20, 220, 350, 30, hwnd, MAKEINTRESOURCE(IDS_NOFULLRANDOM));
		HWND FullRandomString = createTextString(L"Ignores all randomization options and make each boss have an equal change of showing up. Obeys forcing rules.", 20, 220, 345, 30, hwnd, MAKEINTRESOURCE(IDS_FULLRANDOM));
		HWND NoSoloRandomString = createTextString(L"Solo Traveler Randomization is Disabled", 20, 253, 350, 30, hwnd, MAKEINTRESOURCE(IDS_NOSOLORANDOM));
		HWND SoloRandomString = createTextString(L"Forces a random Solo Traveler to fight a certain boss. This traveler doesn't have to be in the party.", 20, 253, 345, 30, hwnd, MAKEINTRESOURCE(IDS_SOLORANDOM));
		HWND NoForcingString = createTextString(L"No boss forcing is done.", 20, 286, 345, 15, hwnd, MAKEINTRESOURCE(IDS_NOFORCE));
		HWND ForcingString = createTextString(L"Boss forcing is enabled, Check options for details.", 20, 286, 345, 15, hwnd, MAKEINTRESOURCE(IDS_FORCEBOSS));
		HWND NoForcingPCString = createTextString(L"Do not force a random PC", 20, 305, 345, 30, hwnd, MAKEINTRESOURCE(IDS_NOFORCEPC));
		HWND ForcingPCString = createTextString(L"Forces a random PC for a new game. This PC will always\nhave a Chapter 1 boss", 20, 305, 345, 30, hwnd, MAKEINTRESOURCE(IDS_FORCEPC));
		HWND SpecificPCString = createTextString(L"Forces the specified PC for a new game. This PC will always have a Chapter 1 boss", 20, 305, 345, 30, hwnd, MAKEINTRESOURCE(IDS_SPECIFICPC));
		HWND PCWinConditionString = createTextString(L"Credits roll upon completion of the main PC.", 20, 338, 345, 15, hwnd, MAKEINTRESOURCE(IDS_PCWIN));
		HWND GalderaWinConditionString = createTextString(L"Credits roll upon completing the Gate of Finis and defeating Galdera.", 20, 338, 345, 15, hwnd, MAKEINTRESOURCE(IDS_GALDERAWIN));
	
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
		intvector configs(16);
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
			CheckDlgButton(hwnd, IDB_NOFORCE, BST_CHECKED);
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

		// Force bosses dialog combo boxes

		// Enforce option box greying out if not selected

		SendDlgItemMessage(hwnd, IDE_EDIT, WM_SETTEXT, 0, (LPARAM)pakDir.c_str());
	}
		// Set font for child windows
		EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
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
		case IDB_RANDOMIZE_BUTTON:

			break;

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
		return 0;
			
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}