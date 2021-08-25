// Octopath Boss Randomizer
// Created by: Mastermind1919

// enable syslink in dialogs
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// use Unicode
#ifndef UNICODE
#define UNICODE
#endif

// use resources
#include "resource.h"

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
#include <vsstyle.h>
#include <sstream>

// Global Variables for dialog boxes
intvector configs(27);
// Global variable for HWND 
std::vector<HWND> OctoBoss;
std::vector<HWND> MainBossContents(100);
std::vector<HWND> SideBossContents(100);
std::vector<HWND> ScalingContents(100);
HWND TabMenu;

// for avoid endless loop with EN_CHANGE notification
bool onEditChange = true;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

// From SideBossRandomize
vectorvector uniformSideInsertRandomize(std::mt19937 &rng, intvector configs, vectorvector bossInput, HWND sideBossPercent, HWND preBossPercent);
// From event
bool eventFiles(vectorvector soloVector, intvector configs);

// Function for four check button groups, check is the button which is checked
void quadStateCheck(HWND hwnd, int check, int firstCheck, int secondCheck, int thirdCheck, int fourthCheck, HWND firstString, HWND secondString, HWND thirdString, HWND fourthString) {
	CheckDlgButton(hwnd, firstCheck, check == 1 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, secondCheck, check == 2 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, thirdCheck, check == 3 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, fourthCheck, check == 4 ? BST_CHECKED : BST_UNCHECKED);
	ShowWindow(firstString, check == 1 ? SW_SHOW : SW_HIDE);
	ShowWindow(secondString, check == 2 ? SW_SHOW : SW_HIDE);
	ShowWindow(thirdString, check == 3 ? SW_SHOW : SW_HIDE);
	ShowWindow(fourthString, check == 4 ? SW_SHOW : SW_HIDE);
}

// Function for three check button groups, check is the button which is checked
void triStateCheck(HWND hwnd, int check, int firstCheck, int secondCheck, int thirdCheck, HWND firstString, HWND secondString, HWND thirdString) {
	CheckDlgButton(hwnd, firstCheck, check == 1 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, secondCheck, check == 2 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, thirdCheck, check == 3 ? BST_CHECKED : BST_UNCHECKED);
	ShowWindow(firstString, check == 1 ? SW_SHOW : SW_HIDE);
	ShowWindow(secondString, check == 2 ? SW_SHOW : SW_HIDE);
	ShowWindow(thirdString, check == 3 ? SW_SHOW : SW_HIDE);
}

// Two check button groups, check is the button which is checked
void duoStateCheck(HWND hwnd, int check, int firstCheck, int secondCheck, HWND firstString, HWND secondString) {
	CheckDlgButton(hwnd, firstCheck, check == 1 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwnd, secondCheck, check == 2 ? BST_CHECKED : BST_UNCHECKED);
	ShowWindow(firstString, check == 1 ? SW_SHOW : SW_HIDE);
	ShowWindow(secondString, check == 2 ? SW_SHOW : SW_HIDE);
}

// Update Text Strings based on edit control input
int editUpdate(HWND editHWND, HWND stringHWND, const wchar_t* minText, const wchar_t* maxText, const wchar_t* preText, const wchar_t* postText, const wchar_t* min, const wchar_t* max) {
	// Disable EN_CHANGE Messages
	onEditChange = false;
	// Retrieve the text of the edit control
	int config = 0;
	LRESULT len = SendMessage(editHWND, WM_GETTEXTLENGTH, 0, 0);
	// Check if len is 0, set min message
	if (len == 0) {

		SendMessage(editHWND, WM_SETTEXT, NULL, (LPARAM)min);
		SendMessage(editHWND, EM_SETSEL, 1, 1);
		// Update Description
		SendMessage(stringHWND, WM_SETTEXT, NULL, (LPARAM)minText);
		config = std::stoi(min);
	}
	else {
		WCHAR* editString = new WCHAR[len];
		SendMessage(editHWND, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)editString);
		// Check if the input is over max
		if (std::stoi(editString) > std::stoi(max)) {
			// Max out the range at max value
			SendMessage(editHWND, WM_SETTEXT, NULL, (LPARAM)max);
			SendMessage(editHWND, EM_SETSEL, 3, 3);
			// Change description box
			SendMessage(stringHWND, WM_SETTEXT, NULL, (LPARAM)maxText);
			config = std::stoi(max);
		}
		// check if at min
		else if (std::stoi(editString) <= std::stoi(min)) {
			// Min out range
			SendMessage(editHWND, WM_SETTEXT, NULL, (LPARAM)min);
			SendMessage(editHWND, EM_SETSEL, 1, 1);
			// Update Description
			SendMessage(stringHWND, WM_SETTEXT, NULL, (LPARAM)minText);
			config = std::stoi(min);
		}
		else {
			// Default case, update description text
			// Prune leading 0s
			std::wstring prunedString = std::wstring(editString).substr(std::wstring(editString).find_first_not_of(L"0"), std::string::npos);
			std::wstring editText = std::wstring(preText) + prunedString + std::wstring(postText);
			SendMessage(stringHWND, WM_SETTEXT, NULL, (LPARAM)editText.c_str());
			// Update Config
			config = std::stoi(editString);
		}
	}
	// enable EN_CHANGE messages
	onEditChange = true;
	return config;
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

// Create text strings
HWND createEditBox(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	// Create the text strings for the interface
	HWND hwndEdit = CreateWindow(
		L"Edit",
		name,
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
		startx,
		starty,
		sizex,
		sizey,
		hwnd,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);
	return hwndEdit;
}

// Create check buttons
HWND createCheckButton(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	HWND checkButton = CreateWindow(
		L"Button",
		name,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_LEFT | BS_FLAT,
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

// Create Slider
HWND createSlider(LPCWSTR name, int startx, int starty, int sizex, int sizey, HWND hwnd, LPWSTR id) {
	HWND slider = CreateWindowEx(
		NULL,
		TRACKBAR_CLASS,
		name,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | TBS_NOTICKS,
		startx,
		starty,
		sizex,
		sizey,
		hwnd,
		(HMENU)id,
		(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
		NULL
	);
	return slider;
}

// Create the main window
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	const size_t MAX_LOADSTRING = 256;
	const int bufferSize = 256;

	WCHAR className[MAX_LOADSTRING - 18];
	WCHAR title[MAX_LOADSTRING];
	WCHAR version[10]; // Version should never be longer than 6 bytes
	LoadString(hInstance, APP_NAME, title, bufferSize);
	LoadString(hInstance, APP_CLASS, className, (bufferSize - 18));
	LoadString(hInstance, VERSION, version, 10);

	// Combine the version and class name to show both in title
	WCHAR classVersion[MAX_LOADSTRING];
	swprintf_s(classVersion, L"%s %s", className, version);

	HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALLICON));

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hIcon = icon;
	wc.hInstance = hInstance;
	wc.lpszClassName = title;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN);

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
	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// Zero padding for config sync
std::wstring zeroPad(int configs) {
	// Error out if over 3 digits
	if (configs > 999) {
		std::cerr << "zeroPad: Greater than 3 digits input.";
		return L"err";
	}
	else {
		std::wstringstream tempConfig;
		// if 3 digits, return original value
		if (configs >= 100) {
			tempConfig << configs;
		}
		else {
			// Check for single digit
			if (configs < 10) {
				tempConfig << L"00" << configs;
			}
			// Two digits
			else {
				tempConfig << L"0" << configs;
			}
		}
		return tempConfig.str();
	}
}
// Convert zero padded inputs for string boxes into ints
std::wstring unpadZero(wchar_t input1, wchar_t input2, wchar_t input3) {
	// combine the inputs into one string
	std::wstringstream inputStrings;
	if (!std::wstring(1, input1).compare(L"0")) {
		if (!std::wstring(1, input2).compare(L"0")) {
			inputStrings << input3;
		}
		else {
			inputStrings << input2 << input3;
		}
	}
	else {
		inputStrings << input1;
		inputStrings << input2;
		inputStrings << input3;
	}
	return inputStrings.str();
}

void configParser(std::string input, std::wstring* pakPathPointer) {
	// Use simpler config writer
	std::wstring lines;
	std::wifstream configFile(input.c_str());
	if (configFile.is_open() == true) {
		for (std::string line; std::getline(configFile, lines); ) {
			// ignore lines containing "//" and empty lines
			if (lines.find_first_of(L"//") == std::string::npos && lines != L"") {
				// Stores the location of quotes
				intvector quotes;
				int evenTest = 0;
				for (int i = 0; i < lines.size(); i++) {
					if (lines[i] == '\"') {
						if (evenTest == 0 || evenTest == 2) {
							quotes.push_back(i + 1);
							evenTest++;
						}
						else {
							quotes.push_back(i - 1);
							evenTest++;
						}
					}
				}
				for (int i = 0; i < 20; i++) {
					configs[i] = lines.at(i);
				}

				// Retieve the percentage based options
				// Map for max values
				std::map<int, int> maxValues;
				maxValues[0] = 2;
				maxValues[1] = 2;
				maxValues[2] = 2;
				maxValues[3] = 1;
				maxValues[4] = 1;
				maxValues[5] = 1;
				maxValues[6] = 1;
				maxValues[7] = 1;
				maxValues[8] = 1;
				maxValues[9] = 2;
				maxValues[10] = 8;
				maxValues[11] = 8;
				maxValues[12] = 8;
				maxValues[13] = 9;
				maxValues[14] = 4;
				maxValues[15] = 8;
				maxValues[16] = 1;
				maxValues[17] = 7;
				maxValues[18] = 3;
				maxValues[19] = 2;
				maxValues[22] = 1;
				maxValues[26] = 1;
				for (int i = 0; i < 20; i++) {
					if (_wtoi(std::wstring(1, lines.at(i)).c_str()) > maxValues[i]) {
						// Set config to max value if input is over it
						configs[i] = maxValues[i];
					}
					else {
						// Set config to whatever the input value is
						configs[i] = _wtoi(std::wstring(1, lines.at(i)).c_str());
					}
				}
				// for hardmode scaling option
				if (_wtoi(std::wstring(1, lines.at(29)).c_str()) > maxValues[22]) {
					// Set config to max value if input is over it
					configs[22] = maxValues[22];
				}
				else {
					// Set config to whatever the input value is
					configs[22] = _wtoi(std::wstring(1, lines.at(29)).c_str());
				}
				// for lone hardmode scaling option
				if (_wtoi(std::wstring(1, lines.at(41)).c_str()) > maxValues[26]) {
					// Set config to max value if input is over it
					configs[26] = maxValues[26];
				}
				else {
					// Set config to whatever the input value is
					configs[26] = _wtoi(std::wstring(1, lines.at(41)).c_str());
				}
				SendMessage(SideBossContents[SIDE_BOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)unpadZero(lines.at(21), lines.at(22), lines.at(23)).c_str());
				SendMessage(SideBossContents[SIDE_PREBOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)unpadZero(lines.at(25), lines.at(26), lines.at(27)).c_str());
				SendMessage(ScalingContents[SCALE_RANDOBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(lines.at(31), lines.at(32), lines.at(33)).c_str());
				SendMessage(ScalingContents[SCALE_EXPBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(lines.at(35), lines.at(36), lines.at(37)).c_str());
				SendMessage(ScalingContents[SCALE_JPBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(lines.at(39), lines.at(40), lines.at(41)).c_str());

				// Pak dir
				std::wstring pakDir = lines.substr(quotes[0], quotes[1]);
				std::wstring trimmedpakDir = pakDir.substr(0, pakDir.length() - 1);
				*pakPathPointer = trimmedpakDir;

			}
		}
	}
	else {
		// If config not found, use default values
		configs[0] = 0;
		configs[1] = 1;
		configs[2] = 1;
		configs[3] = 0;
		configs[4] = 0;
		configs[5] = 0;
		configs[6] = 0;
		configs[7] = 0;
		configs[8] = 0;
		configs[9] = 0;
		configs[10] = 0;
		configs[11] = 0;
		configs[12] = 0;
		configs[13] = 0;
		configs[14] = 0;
		configs[15] = 0;
		configs[16] = 0;
		configs[17] = 0;
		configs[18] = 0;
		configs[19] = 0;
		configs[22] = 0;
		configs[26] = 0;

		SendMessage(SideBossContents[SIDE_BOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)L"20");
		SendMessage(SideBossContents[SIDE_PREBOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)L"20");
		SendMessage(ScalingContents[SCALE_RANDOBOX - 300], WM_SETTEXT, 0, (LPARAM)L"0");
		SendMessage(ScalingContents[SCALE_EXPBOX - 300], WM_SETTEXT, 0, (LPARAM)L"1");
		SendMessage(ScalingContents[SCALE_JPBOX - 300], WM_SETTEXT, 0, (LPARAM)L"1");
		*pakPathPointer = L"";
	}

	configFile.close();

}

void configWriter(std::string input, intvector configs, wchar_t* pakPath) {

	// Write to config file
	std::wstring lines;
	std::wofstream configFile(input.c_str());
	// Write a first line for explaining
	configFile << "// Please do not modify this file" << std::endl;
	// for the first options, copy them
	for (int i = 0; i < 20; i++) {
		configFile << configs[i];
	}
	// Pad the percent based options with leading zeros
	configFile << L"|";
	configFile << zeroPad(configs[20]);
	configFile << L"|";
	configFile << zeroPad(configs[21]);
	configFile << L"|";
	configFile << configs[22];
	configFile << L"|";
	configFile << zeroPad(configs[23]);
	configFile << L"|";
	configFile << zeroPad(configs[24]);
	configFile << L"|";
	configFile << zeroPad(configs[25]);
	configFile << L"|";
	configFile << configs[26];

	// Pak dir
	configFile << "\"" << pakPath << "\"" << std::endl;;
	configFile.close();
}

// Apply Config options to string box
void ApplyStringBox(int tabMenu) {
	// Apply string options based on tab menu
	switch (tabMenu) {
	case 0:
		// Mixing Options
		switch (configs[0]) {
		case 0:
			ShowWindow(MainBossContents[MAIN_DEFAULTMIXSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_MIX24STRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_MIX14STRING - 100], SW_HIDE);
			break;
		case 1:
			ShowWindow(MainBossContents[MAIN_DEFAULTMIXSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_MIX24STRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_MIX14STRING - 100], SW_HIDE);
			break;
		case 2:
			ShowWindow(MainBossContents[MAIN_DEFAULTMIXSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_MIX24STRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_MIX14STRING - 100], SW_SHOW);
			break;
		}
		// Shrine Boss Options
		switch (configs[1]) {
		case 0:
			ShowWindow(MainBossContents[MAIN_NOSHRINESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_RANDOMSHRINESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_INCLUDESHRINESTRING - 100], SW_HIDE);
			break;
		case 1:
			ShowWindow(MainBossContents[MAIN_NOSHRINESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_RANDOMSHRINESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_INCLUDESHRINESTRING - 100], SW_HIDE);
			break;
		case 2:
			ShowWindow(MainBossContents[MAIN_NOSHRINESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_RANDOMSHRINESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_INCLUDESHRINESTRING - 100], SW_SHOW);
			break;
		}
		// Gate Boss Options
		switch (configs[2]) {
		case 0:
			ShowWindow(MainBossContents[MAIN_NOGATESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_RANDOMGATESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_INCLUDEGATESTRING - 100], SW_HIDE);
			break;
		case 1:
			ShowWindow(MainBossContents[MAIN_NOGATESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_RANDOMGATESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_INCLUDEGATESTRING - 100], SW_HIDE);
			break;
		case 2:
			ShowWindow(MainBossContents[MAIN_NOGATESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_RANDOMGATESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_INCLUDEGATESTRING - 100], SW_SHOW);
			break;
		}
		// Galdera Options
		if (configs[3] == 0) {
			ShowWindow(MainBossContents[MAIN_NOGALDERASTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_INCLUDEGALDERASTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_NOGALDERASTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_INCLUDEGALDERASTRING - 100], SW_SHOW);
		}
		// Duplicate Options
		if (configs[4] == 0) {
			ShowWindow(MainBossContents[MAIN_NODUPLICATESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_ALLOWDUPLICATESTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_NODUPLICATESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_ALLOWDUPLICATESTRING - 100], SW_SHOW);
		}
		// Win Condition
		if (configs[5] == 0) {
			ShowWindow(MainBossContents[MAIN_PCWINSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_GALDERAWINSTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_PCWINSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_GALDERAWINSTRING - 100], SW_SHOW);
		}
		// Full Random
		if (configs[6] == 0) {
			ShowWindow(MainBossContents[MAIN_NOFULLRANDOMSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_FULLRANDOMSTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_NOFULLRANDOMSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FULLRANDOMSTRING - 100], SW_SHOW);
		}
		// Solo Random
		if (configs[7] == 0) {
			ShowWindow(MainBossContents[MAIN_NOSOLORANDOMSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_SOLORANDOMSTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_NOSOLORANDOMSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_SOLORANDOMSTRING - 100], SW_SHOW);
		}
		// Force Boss Option
		if (configs[8] == 0) {
			ShowWindow(MainBossContents[MAIN_NOFORCESTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_FORCESTRING - 100], SW_HIDE);
		}
		else {
			ShowWindow(MainBossContents[MAIN_NOFORCESTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FORCESTRING - 100], SW_SHOW);
		}
		// Character Forcing Option
		switch (configs[9]) {
		case 0:
			ShowWindow(MainBossContents[MAIN_NOFORCEPCSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_FORCERANDOMPCSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100], SW_HIDE);
			break;
		case 1:
			ShowWindow(MainBossContents[MAIN_NOFORCEPCSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FORCERANDOMPCSTRING - 100], SW_SHOW);
			ShowWindow(MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100], SW_HIDE);
			break;
		case 2:
			ShowWindow(MainBossContents[MAIN_NOFORCEPCSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FORCERANDOMPCSTRING - 100], SW_HIDE);
			ShowWindow(MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100], SW_SHOW);
			break;
		}
		break;
	case 1:
		// Side Boss Randomization Options
		switch (configs[18]) {
		case 0:
			ShowWindow(SideBossContents[SIDE_NORINGSTRING - 200], SW_SHOW);
			ShowWindow(SideBossContents[SIDE_SAMERINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXRINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXBOSSSTRING - 200], SW_HIDE);
			break;
		case 1:
			ShowWindow(SideBossContents[SIDE_NORINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_SAMERINGSTRING - 200], SW_SHOW);
			ShowWindow(SideBossContents[SIDE_MIXRINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXBOSSSTRING - 200], SW_HIDE);
			break;
		case 2:
			ShowWindow(SideBossContents[SIDE_NORINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_SAMERINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXRINGSTRING - 200], SW_SHOW);
			ShowWindow(SideBossContents[SIDE_MIXBOSSSTRING - 200], SW_HIDE);
			break;
		case 3:
			ShowWindow(SideBossContents[SIDE_NORINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_SAMERINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXRINGSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_MIXBOSSSTRING - 200], SW_SHOW);
			break;
		}
		// Pre Boss Encounters
		switch (configs[19]) {
		case 0:
			ShowWindow(SideBossContents[SIDE_NOPREBOSSSTRING - 200], SW_SHOW);
			ShowWindow(SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_PREBOSSMIXSTRING - 200], SW_HIDE);
			break;
		case 1:
			ShowWindow(SideBossContents[SIDE_NOPREBOSSSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SW_SHOW);
			ShowWindow(SideBossContents[SIDE_PREBOSSMIXSTRING - 200], SW_HIDE);
			break;
		case 2:
			ShowWindow(SideBossContents[SIDE_NOPREBOSSSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SW_HIDE);
			ShowWindow(SideBossContents[SIDE_PREBOSSMIXSTRING - 200], SW_SHOW);
			break;
		}
		// Just show the other strings
		ShowWindow(SideBossContents[SIDE_BOSSCHANCESTRING - 200], SW_SHOW);
		ShowWindow(SideBossContents[SIDE_PREBOSSCHANCESTRING - 200], SW_SHOW);
		break;
		// Scaling Options
	case 2:
		// Hardmode Enabler
		if (configs[22] == 0) {
			ShowWindow(ScalingContents[SCALE_NOHARDMODESTRING - 300], SW_SHOW);
			ShowWindow(ScalingContents[SCALE_HARDMODESTRING - 300], SW_HIDE);
		}
		else {
			ShowWindow(ScalingContents[SCALE_NOHARDMODESTRING - 300], SW_HIDE);
			ShowWindow(ScalingContents[SCALE_HARDMODESTRING - 300], SW_SHOW);
		}
		// Boss scaling by location
		if (configs[26] == 0) {
			ShowWindow(ScalingContents[SCALE_NOBOSSLOCSTRING - 300], SW_SHOW);
			ShowWindow(ScalingContents[SCALE_BOSSLOCSTRING - 300], SW_HIDE);
		}
		else {
			ShowWindow(ScalingContents[SCALE_NOBOSSLOCSTRING - 300], SW_HIDE);
			ShowWindow(ScalingContents[SCALE_BOSSLOCSTRING - 300], SW_SHOW);
		}
		// Just show the rest of the strings
		ShowWindow(ScalingContents[SCALE_RANDOSTRING - 300], SW_SHOW);
		ShowWindow(ScalingContents[SCALE_JPSTRING - 300], SW_SHOW);
		ShowWindow(ScalingContents[SCALE_EXPSTRING - 300], SW_SHOW);
		break;
		// Error catcher
	default:
		std::cout << "ApplyStringBox: How did we get here?" << std::endl;
		break;
	}
}

// Dialog box for config sync
INT_PTR CALLBACK ConfigDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_INITDIALOG:
	{
		// Hide error message
		ShowWindow(GetDlgItem(hwnd, IDC_CONFIGERROR), SW_HIDE);
		// Populate the config edit box with the config options of the current randomizer settings
		std::wstringstream editConfigSString;
		for (int i = 0; i < 20; i++) {
			editConfigSString << configs[i];
		}
		// Pad the percent based options with leading zeros
		editConfigSString << L"/";
		editConfigSString << zeroPad(configs[20]);
		editConfigSString << L"/";
		editConfigSString << zeroPad(configs[21]);
		editConfigSString << L"/";
		editConfigSString << configs[22];
		editConfigSString << L"/";
		editConfigSString << zeroPad(configs[23]);
		editConfigSString << L"/";
		editConfigSString << zeroPad(configs[24]);
		editConfigSString << L"/";
		editConfigSString << zeroPad(configs[25]);
		editConfigSString << L"/";
		editConfigSString << configs[26];

		// Make the edit box reflect the config options
		std::wstring editConfigString = editConfigSString.str();
		SendMessage(GetDlgItem(hwnd, IDC_CONFIGSYNCEDIT), WM_SETTEXT, 0, (LPARAM)editConfigString.c_str());
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		{
			// Apply the options in the config box
			// Check if the input is formatted correctly
			// First, check the length, which should always be the same
			LRESULT len = SendMessage(GetDlgItem(hwnd, IDC_CONFIGSYNCEDIT), WM_GETTEXTLENGTH, 0, 0);
			// If buffer is less than 43, notify user and allow ability to change string
			if (len <= 43) {
				ShowWindow(GetDlgItem(hwnd, IDC_CONFIGERROR), SW_SHOW);
				break;
			}
			WCHAR* editInput = new WCHAR[len];
			SendMessage(GetDlgItem(hwnd, IDC_CONFIGSYNCEDIT), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)editInput);
			std::wstring editStringInput = editInput;
			// Start populating the controls based on input, with error correction if necessary.
			// Map for max values
			std::map<int, int> maxValues;
			maxValues[0] = 2;
			maxValues[1] = 2;
			maxValues[2] = 2;
			maxValues[3] = 1;
			maxValues[4] = 1;
			maxValues[5] = 1;
			maxValues[6] = 1;
			maxValues[7] = 1;
			maxValues[8] = 1;
			maxValues[9] = 2;
			maxValues[10] = 8;
			maxValues[11] = 8;
			maxValues[12] = 8;
			maxValues[13] = 9;
			maxValues[14] = 4;
			maxValues[15] = 8;
			maxValues[16] = 1;
			maxValues[17] = 7;
			maxValues[18] = 3;
			maxValues[19] = 2;
			maxValues[22] = 1;
			maxValues[26] = 1;
			for (int i = 0; i < 20; i++) {
				if (_wtoi(std::wstring(1, editInput[i]).c_str()) > maxValues[i]) {
					// Set config to max value if input is over it
					configs[i] = maxValues[i];
				}
				else {
					// Set config to whatever the input value is
					configs[i] = _wtoi(std::wstring(1, editInput[i]).c_str());
				}
			}
			// for hardmode scaling option
			if (_wtoi(std::wstring(1, editInput[29]).c_str()) > maxValues[22]) {
				// Set config to max value if input is over it
				configs[22] = maxValues[22];
			}
			else {
				// Set config to whatever the input value is
				configs[22] = _wtoi(std::wstring(1, editInput[29]).c_str());
			}
			// for boss location scaling
			if (_wtoi(std::wstring(1, editInput[43]).c_str()) > maxValues[26]) {
				// Set config to max value if input is over it
				configs[26] = maxValues[26];
			}
			else {
				// Set config to whatever the input value is
				configs[26] = _wtoi(std::wstring(1, editInput[43]).c_str());
			}
			// Update the config options by sending push button notifications
			switch (configs[0]) {
			case 0:
				SendMessage(MainBossContents[MAIN_DEFAULTMIX - 100], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(MainBossContents[MAIN_MIX24 - 100], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(MainBossContents[MAIN_MIX14 - 100], BM_CLICK, 0, 0);
				break;
			}
			switch (configs[1]) {
			case 0:
				SendMessage(MainBossContents[MAIN_NOSHRINE - 100], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(MainBossContents[MAIN_RANDOMSHRINE - 100], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(MainBossContents[MAIN_INCLUDESHRINE - 100], BM_CLICK, 0, 0);
				break;
			}
			switch (configs[2]) {
			case 0:
				SendMessage(MainBossContents[MAIN_NOGATE - 100], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(MainBossContents[MAIN_RANDOMGATE - 100], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(MainBossContents[MAIN_INCLUDEGATE - 100], BM_CLICK, 0, 0);
				break;
			}
			configs[3] == 0 ? SendMessage(MainBossContents[MAIN_NOGALDERA - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_INCLUDEGALDERA - 100], BM_CLICK, 0, 0);
			configs[4] == 0 ? SendMessage(MainBossContents[MAIN_NODUPLICATE - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_ALLOWDUPLICATE - 100], BM_CLICK, 0, 0);
			configs[5] == 0 ? SendMessage(MainBossContents[MAIN_PCWIN - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_GALDERAWIN - 100], BM_CLICK, 0, 0);
			if ((bool)configs[6] != (bool)SendMessage(MainBossContents[MAIN_FULLRANDOM - 100], BM_GETCHECK, 0, 0)) {
				SendMessage(MainBossContents[MAIN_FULLRANDOM - 100], BM_CLICK, 0, 0);
			}
			if ((bool)configs[7] != (bool)SendMessage(MainBossContents[MAIN_SOLORANDOM - 100], BM_GETCHECK, 0, 0)) {
				SendMessage(MainBossContents[MAIN_SOLORANDOM - 100], BM_CLICK, 0, 0);
			}
			if ((bool)configs[8] != (bool)SendMessage(MainBossContents[MAIN_FORCEBOSS - 100], BM_GETCHECK, 0, 0)) {
				SendMessage(MainBossContents[MAIN_FORCEBOSS - 100], BM_CLICK, 0, 0);
			}
			switch (configs[9]) {
			case 0:
				SendMessage(MainBossContents[MAIN_NOFORCEPC - 100], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(MainBossContents[MAIN_FORCEPC- 100], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(MainBossContents[MAIN_SPECIFICPC - 100], BM_CLICK, 0, 0);
				break;
			}
			// Force Boss Options are set when that dialog menu is entered
			SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_SETCURSEL, configs[17], 0);
			switch (configs[18]) {
			case 0:
				SendMessage(SideBossContents[SIDE_NORING - 200], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(SideBossContents[SIDE_SAMERING - 200], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(SideBossContents[SIDE_MIXRING - 200], BM_CLICK, 0, 0);
				break;
			case 3:
				SendMessage(SideBossContents[SIDE_MIXBOSS - 200], BM_CLICK, 0, 0);
				break;
			}
			switch (configs[19]) {
			case 0:
				SendMessage(SideBossContents[SIDE_NOPREBOSS - 200], BM_CLICK, 0, 0);
				break;
			case 1:
				SendMessage(SideBossContents[SIDE_PREBOSSRANDO - 200], BM_CLICK, 0, 0);
				break;
			case 2:
				SendMessage(SideBossContents[SIDE_PREBOSSMIX - 200], BM_CLICK, 0, 0);
				break;
			}
			if ((bool)configs[22] != (bool)SendMessage(ScalingContents[SCALE_HARDMODE - 300], BM_GETCHECK, 0, 0)) {
				SendMessage(ScalingContents[SCALE_HARDMODE - 300], BM_CLICK, 0, 0);
			}
			if ((bool)configs[26] != (bool)SendMessage(ScalingContents[SCALE_BOSSLOC - 300], BM_GETCHECK, 0, 0)) {
				SendMessage(ScalingContents[SCALE_BOSSLOC - 300], BM_CLICK, 0, 0);
			}
			// set the percentage based inputs, WM_SETTEXT triggers bounding via functions
			SendMessage(SideBossContents[SIDE_BOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)unpadZero(editInput[21], editInput[22], editInput[23]).c_str());
			SendMessage(SideBossContents[SIDE_PREBOSSCHANCE - 200], WM_SETTEXT, 0, (LPARAM)unpadZero(editInput[25], editInput[26], editInput[27]).c_str());
			SendMessage(ScalingContents[SCALE_RANDOBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(editInput[31], editInput[32], editInput[33]).c_str());
			SendMessage(ScalingContents[SCALE_EXPBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(editInput[35], editInput[36], editInput[37]).c_str());
			SendMessage(ScalingContents[SCALE_JPBOX - 300], WM_SETTEXT, 0, (LPARAM)unpadZero(editInput[39], editInput[40], editInput[41]).c_str());

			// Go to the main tab after updating configs
			SendMessage(TabMenu, TCM_SETCURSEL, 0, 0);
			for (int i = 0; i < 50; i++) {
				ShowWindow(MainBossContents[i], SW_SHOW);
			}
			// Hide other options
			for (int i = 0; i < 100; i++) {
				ShowWindow(SideBossContents[i], SW_HIDE);
				ShowWindow(ScalingContents[i], SW_HIDE);
			}
			ApplyStringBox(0);
			EndDialog(hwnd, IDOK);
			break;
		}
		case IDCANCEL:
			// End the config sync without changing options
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;
	}
	return DefWindowProc(hwnd, Message, wParam, lParam);
}

// Dialog box for boss forcing options
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
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_SETCURSEL, configs[10], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_SETCURSEL, configs[11], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_SETCURSEL, configs[12], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_ADDSTRING, 0, LPARAM(L"9"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_SETCURSEL, configs[13], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_SETCURSEL, configs[14], 0);
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"No"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"1"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"2"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"3"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"4"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"5"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"6"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"7"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_ADDSTRING, 0, LPARAM(L"8"));
		SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_SETCURSEL, configs[15], 0);
		configs[16] == 1 ? CheckDlgButton(hwnd, IDC_FORCEGALDERA, BST_CHECKED) : CheckDlgButton(hwnd, IDC_FORCEGALDERA, BST_UNCHECKED);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			// Post changes to config
			configs[10] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER1), CB_GETCURSEL, 0, 0);
			configs[11] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER2), CB_GETCURSEL, 0, 0);
			configs[12] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER3), CB_GETCURSEL, 0, 0);
			configs[13] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER4), CB_GETCURSEL, 0, 0);
			configs[14] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER5), CB_GETCURSEL, 0, 0);
			configs[15] = (int)SendMessage(GetDlgItem(hwnd, IDC_COMBOTIER6), CB_GETCURSEL, 0, 0);
			(bool)IsDlgButtonChecked(hwnd, IDC_FORCEGALDERA) == true ? configs[16] = 1 : configs[16] = 0;
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
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

// The main meat of the program, the window loop
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		// Executed on window loading
	case WM_CREATE:
	{
		RECT mainWindowSize;
		GetClientRect(hwnd, &mainWindowSize);
		// Create the persistant windows for each tab of the randomizer
		HWND pakDIR = CreateWindow(L"Static", L"Octopath Traveler Pak Directory:", WS_VISIBLE | WS_CHILD, 10, 10, 200, 15, hwnd, NULL, NULL, NULL);
		HWND seedText = CreateWindow(L"Static", L"Seed (Leave blank for random seed):", WS_VISIBLE | WS_CHILD, 560, 10, 200, 15, hwnd, NULL, NULL, NULL);
		// File text box with input
		HWND pakEdit = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES, L"Edit", TEXT(""), WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP, 10, 28, 447, 21, hwnd, (HMENU)EDITBOX, NULL, NULL);
		// File Browser Button
		HWND fileButton = CreateWindow(L"Button", L"Browse", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, 460, 28, 60, 21, hwnd, (HMENU)MAKEINTRESOURCE(FILEBUTTON), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		// Randomize button
		HWND randomizeButton = CreateWindow(L"Button", L"Randomize", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, 330, 447, 90, 33, hwnd, (HMENU)MAKEINTRESOURCE(RANDOMIZEBUTTON), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		// Seed edit control
		HWND seedEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", TEXT(""), WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP, 560, 28, 200, 21, hwnd, (HMENU)SEEDBOX, NULL, NULL);
		// For some reason, entering above 9 characters causes an error
		// set 9 character limit for now
		SendMessage(seedEdit, EM_SETLIMITTEXT, (WPARAM)9, 0);
		// Config Sync Button
		HWND configSync = CreateWindow(L"Button", L"Config Sync", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, 660, 447, 90, 33, hwnd, (HMENU)MAKEINTRESOURCE(CONFIGSYNC), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		// Description Text
		RECT rectangle;
		SetRect(&rectangle, (mainWindowSize.left + 10), 62, (mainWindowSize.right / 2 - 35), 440);
		HWND descriptionString = createTextString(L"These are descriptions of the currently selected options.", (rectangle.left + 5), (rectangle.top + 5), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(DESCRIPTIONSTRING));

		// Tab buttons for each seperate option menu
		TabMenu = CreateWindowEx(NULL, WC_TABCONTROL, NULL, WS_BORDER | WS_CHILD | WS_VISIBLE, (mainWindowSize.right / 2 - 25), 62, ((mainWindowSize.right - 10) - (mainWindowSize.right / 2 - 25)), 378, hwnd, (HMENU)MAKEINTRESOURCE(TABMENU), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		// Set up tab menu tabs 
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_TAB_CLASSES;
		InitCommonControlsEx(&icex);
		// Add the tabs
		TCITEM tie;
		// Char for loading in strings
		TCHAR achTemp[256];

		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = -1;
		tie.pszText = achTemp;
		LoadString((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAINBOSSOPTIONS, achTemp, sizeof(achTemp) / sizeof(achTemp[0]));
		TabCtrl_InsertItem(TabMenu, 0, &tie);
		LoadString((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), SIDEBOSSOPTIONS, achTemp, sizeof(achTemp) / sizeof(achTemp[0]));
		TabCtrl_InsertItem(TabMenu, 1, &tie);
		LoadString((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), SCALINGOPTIONS, achTemp, sizeof(achTemp) / sizeof(achTemp[0]));
		TabCtrl_InsertItem(TabMenu, 2, &tie);

		// Create the child windows for the tabs
		RECT tabSize;
		TabCtrl_SetItemSize(TabMenu, 100, 23);

		// Main Boss Options
		SetRect(&tabSize, (mainWindowSize.right / 2 - 25), 65, ((mainWindowSize.right - 10) - (mainWindowSize.right / 2 - 25)), 378);

		// Right side
		MainBossContents[MAIN_MIXTEXT - 100] = createTextString(L"Chapter Mixing Options:", (tabSize.left + 7), (tabSize.top + 27), 150, 15, hwnd, MAKEINTRESOURCE(MAIN_MIXTEXT));
		MainBossContents[MAIN_DEFAULTMIX - 100] = createRadioButton(L"No Chapter Boss Pool Mixing", (tabSize.left + 12), (tabSize.top + 45), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_DEFAULTMIX));
		MainBossContents[MAIN_MIX24 - 100] = createRadioButton(L"Mix Chapters 2 - 4 Boss Pools", (tabSize.left + 12), (tabSize.top + 63), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_MIX24));
		MainBossContents[MAIN_MIX14 - 100] = createRadioButton(L"Mix Chapters 1 - 4 Boss Pools", (tabSize.left + 12), (tabSize.top + 81), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_MIX14));
		MainBossContents[MAIN_SHRINETEXT - 100] = createTextString(L"Shrine Boss Options:", (tabSize.left + 7), (tabSize.top + 99), 150, 15, hwnd, MAKEINTRESOURCE(MAIN_SHRINETEXT));
		MainBossContents[MAIN_NOSHRINE - 100] = createRadioButton(L"No Shrine Boss Randomization", (tabSize.left + 12), (tabSize.top + 117), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_NOSHRINE));
		MainBossContents[MAIN_RANDOMSHRINE - 100] = createRadioButton(L"Randomize Shrine Bosses", (tabSize.left + 12), (tabSize.top + 135), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_RANDOMSHRINE));
		MainBossContents[MAIN_INCLUDESHRINE - 100] = createRadioButton(L"Include Shrine Bosses", (tabSize.left + 12), (tabSize.top + 153), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDESHRINE));
		MainBossContents[MAIN_GATETEXT - 100] = createTextString(L"Gate of Finis Boss Options:", (tabSize.left + 7), (tabSize.top + 171), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_GATETEXT));
		MainBossContents[MAIN_NOGATE - 100] = createRadioButton(L"No Gate Boss Randomization", (tabSize.left + 12), (tabSize.top + 189), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_NOGATE));
		MainBossContents[MAIN_RANDOMGATE - 100] = createRadioButton(L"Randomize Gate Bosses", (tabSize.left + 12), (tabSize.top + 207), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_RANDOMGATE));
		MainBossContents[MAIN_INCLUDEGATE - 100] = createRadioButton(L"Include Gate Bosses", (tabSize.left + 12), (tabSize.top + 225), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDEGATE));
		MainBossContents[MAIN_GALDERATEXT - 100] = createTextString(L"Galdera Options:", (tabSize.left + 7), (tabSize.top + 243), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_GALDERATEXT));
		MainBossContents[MAIN_NOGALDERA - 100] = createRadioButton(L"No Galdera Randomization", (tabSize.left + 12), (tabSize.top + 261), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_NOGALDERA));
		MainBossContents[MAIN_INCLUDEGALDERA - 100] = createRadioButton(L"Include Galdera", (tabSize.left + 12), (tabSize.top + 279), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDEGALDERA));
		MainBossContents[MAIN_DUPLICATETEXT - 100] = createTextString(L"Duplicate Boss Options:", (tabSize.left + 7), (tabSize.top + 297), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_DUPLICATETEXT));
		MainBossContents[MAIN_NODUPLICATE - 100] = createRadioButton(L"No Duplicates", (tabSize.left + 12), (tabSize.top + 315), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_NODUPLICATE));
		MainBossContents[MAIN_ALLOWDUPLICATE - 100] = createRadioButton(L"Allow Duplicates", (tabSize.left + 12), (tabSize.top + 333), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_ALLOWDUPLICATE));
		MainBossContents[MAIN_WINTEXT - 100] = createTextString(L"Win condition:", (tabSize.left + 200), (tabSize.top + 27), 150, 15, hwnd, MAKEINTRESOURCE(MAIN_WINTEXT));
		MainBossContents[MAIN_PCWIN - 100] = createRadioButton(L"Main PC Completion", (tabSize.left + 205), (tabSize.top + 45), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_PCWIN));
		MainBossContents[MAIN_GALDERAWIN - 100] = createRadioButton(L"Gate of Finis Completion", (tabSize.left + 205), (tabSize.top + 63), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_GALDERAWIN));
		MainBossContents[MAIN_SPECIALTEXT - 100] = createTextString(L"Special Options:", (tabSize.left + 200), (tabSize.top + 81), 150, 15, hwnd, MAKEINTRESOURCE(MAIN_SPECIALTEXT));
		MainBossContents[MAIN_FULLRANDOM - 100] = createCheckButton(L"Full Random", (tabSize.left + 205), (tabSize.top + 99), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_FULLRANDOM));
		MainBossContents[MAIN_SOLORANDOM - 100] = createCheckButton(L"Solo Traveler Randomizer", (tabSize.left + 205), (tabSize.top + 117), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_SOLORANDOM));
		MainBossContents[MAIN_FORCEBOSS - 100] = createCheckButton(L"Force Boss Tier", (tabSize.left + 205), (tabSize.top + 135), 100, 15, hwnd, MAKEINTRESOURCE(MAIN_FORCEBOSS));
		MainBossContents[MAIN_FORCEBOSSOPTION - 100] = CreateWindow(L"Button", L"Options", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_CENTER, (tabSize.left + 305), (tabSize.top + 135), 50, 19, hwnd, (HMENU)MAKEINTRESOURCE(MAIN_FORCEBOSSOPTION), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
		MainBossContents[MAIN_FORCEPCTEXT - 100] = createTextString(L"PC Forcing Options:", (tabSize.left + 200), (tabSize.top + 153), 150, 15, hwnd, MAKEINTRESOURCE(MAIN_FORCEPCTEXT));
		MainBossContents[MAIN_NOFORCEPC - 100] = createRadioButton(L"No PC Forcing", (tabSize.left + 205), (tabSize.top + 171), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_NOFORCEPC));
		MainBossContents[MAIN_FORCEPC - 100] = createRadioButton(L"Force Random PC", (tabSize.left + 205), (tabSize.top + 189), 170, 15, hwnd, MAKEINTRESOURCE(MAIN_FORCEPC));
		MainBossContents[MAIN_SPECIFICPC - 100] = createRadioButton(L"Force Specific PC:", (tabSize.left + 205), (tabSize.top + 207), 110, 15, hwnd, MAKEINTRESOURCE(MAIN_SPECIFICPC));
		MainBossContents[MAIN_SPECIFICPCOPTION - 100] = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, (tabSize.left + 315), (tabSize.top + 207), 70, 15, hwnd, (HMENU)MAKEINTRESOURCE(MAIN_SPECIFICPCOPTION), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

		// Description Box
		MainBossContents[MAIN_DEFAULTMIXSTRING - 100] = createTextString(L"- Do not allow for Chapter 1-4 boss pools to be mixed, so a Chapter 4\r  boss can never appear in a Chapter 3 or lower area.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_DEFAULTMIXSTRING));
		MainBossContents[MAIN_MIX24STRING - 100] = createTextString(L"- Allows for Chapter 2-4 boss pools to be mixed, so a Chapter 4 boss\r  can appear in a Chapter 2 area.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_MIX24STRING));
		MainBossContents[MAIN_MIX14STRING - 100] = createTextString(L"- Allows for Chapter 1-4 boss pools to be mixed, so a Chapter 4 boss\r  can appear in a Chapter 1 area.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_MIX14STRING));
		MainBossContents[MAIN_NOSHRINESTRING - 100] = createTextString(L"- Do not randomize Shrine Bosses.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_NOSHRINESTRING));
		MainBossContents[MAIN_RANDOMSHRINESTRING - 100] = createTextString(L"- Randomize the Shrine Bosses among themselves.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_RANDOMSHRINESTRING));
		MainBossContents[MAIN_INCLUDESHRINESTRING - 100] = createTextString(L"- Include the Shrine Bosses with the Chapter 4 boss pool.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDESHRINESTRING));
		MainBossContents[MAIN_NOGATESTRING - 100] = createTextString(L"- Do not randomize the Gate of Finis Bosses.", (rectangle.left + 5), (rectangle.top + 74), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_NOGATESTRING));
		MainBossContents[MAIN_RANDOMGATESTRING - 100] = createTextString(L"- Randomize the Gate of Finis Bosses among themselves.", (rectangle.left + 5), (rectangle.top + 74), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_RANDOMGATESTRING));
		MainBossContents[MAIN_INCLUDEGATESTRING - 100] = createTextString(L"- Include the Gate of Finis Bosses in the Chapter 4 boss pool.", (rectangle.left + 5), (rectangle.top + 74), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDEGATESTRING));
		MainBossContents[MAIN_NOGALDERASTRING - 100] = createTextString(L"- Do not randomize Galdera.", (rectangle.left + 5), (rectangle.top + 92), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_NOGALDERASTRING));
		MainBossContents[MAIN_INCLUDEGALDERASTRING - 100] = createTextString(L"- Include Galdera in the Chapter 4 boss pool.", (rectangle.left + 5), (rectangle.top + 92), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_INCLUDEGALDERASTRING));
		MainBossContents[MAIN_NODUPLICATESTRING - 100] = createTextString(L"- Do not allow for duplicates, each boss is unique.", (rectangle.left + 5), (rectangle.top + 110), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_NODUPLICATESTRING));
		MainBossContents[MAIN_ALLOWSUPLICATESTRING - 100] = createTextString(L"- Allow for duplicate bosses across encounters.", (rectangle.left + 5), (rectangle.top + 110), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_ALLOWSUPLICATESTRING));
		MainBossContents[MAIN_PCWINSTRING - 100] = createTextString(L"- Credits roll after completing the main PC's story.", (rectangle.left + 5), (rectangle.top + 128), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_PCWINSTRING));
		MainBossContents[MAIN_GALDERAWINSTRING - 100] = createTextString(L"- Credits roll after completing the Gate of Finis.", (rectangle.left + 5), (rectangle.top + 128), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_GALDERAWINSTRING));
		MainBossContents[MAIN_NOFULLRANDOMSTRING - 100] = createTextString(L"- Deprecated, currently unused.", (rectangle.left + 5), (rectangle.top + 146), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_NOFULLRANDOMSTRING));
		MainBossContents[MAIN_FULLRANDOMSTRING - 100] = createTextString(L"- Deprecated, currently unused.", (rectangle.left + 5), (rectangle.top + 146), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_FULLRANDOMSTRING));
		MainBossContents[MAIN_NOSOLORANDOMSTRING - 100] = createTextString(L"- Solo Random is disabled.", (rectangle.left + 5), (rectangle.top + 179), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_NOSOLORANDOMSTRING));
		MainBossContents[MAIN_SOLORANDOMSTRING - 100] = createTextString(L"- Forces a random solo traveler to fight any one boss. It is\r  recommended to have all characters before Chapter 2.", (rectangle.left + 5), (rectangle.top + 179), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(MAIN_SOLORANDOMSTRING));
		MainBossContents[MAIN_NOFORCESTRING - 100] = createTextString(L"- No boss forcing is done.", (rectangle.left + 5), (rectangle.top + 212), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_NOFORCESTRING));
		MainBossContents[MAIN_FORCESTRING - 100] = createTextString(L"- Boss forcing is enabled, check options for details.", (rectangle.left + 5), (rectangle.top + 212), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(MAIN_FORCESTRING));
		MainBossContents[MAIN_NOFORCEPCSTRING - 100] = createTextString(L"- Do not force a specific PC.", (rectangle.left + 5), (rectangle.top + 230), ((rectangle.right - 5) - (rectangle.left + 5)), 45, hwnd, MAKEINTRESOURCE(MAIN_NOFORCEPCSTRING));
		MainBossContents[MAIN_FORCERANDOMPCSTRING - 100] = createTextString(L"- Forces a random PC on the new game screen.\r  This PC will always have a chapter 1 boss.", (rectangle.left + 5), (rectangle.top + 230), ((rectangle.right - 5) - (rectangle.left + 5)), 45, hwnd, MAKEINTRESOURCE(MAIN_FORCERANDOMPCSTRING));
		MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100] = createTextString(L"- Forces a specific PC on the new game screen.\r This PC will always have a chapter 1 boss. Check options\r  for the specific character.", (rectangle.left + 5), (rectangle.top + 230), ((rectangle.right - 5) - (rectangle.left + 5)), 45, hwnd, MAKEINTRESOURCE(MAIN_FORCESPECIFICPCSTRING));

		// Fill the combo box
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Ophilia"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Cyrus"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Tressa"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Olberic"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Primrose"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Alfyn"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"Therion"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_ADDSTRING, 0, LPARAM(L"H'annit"));
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_SETCURSEL, 0, 0);
		
		// Hide all the string contents
		for (int i = 50; i < 100; i++) {
			ShowWindow(MainBossContents[i], SW_HIDE);
		}

		// Side Boss Options

		// Right side
		SideBossContents[SIDE_RINGOPTIONS - 200] = createTextString(L"Side Boss Options:", (tabSize.left + 7), (tabSize.top + 27), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_RINGOPTIONS));
		SideBossContents[SIDE_NORING - 200] = createRadioButton(L"No Randomization", (tabSize.left + 12), (tabSize.top + 45), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_NORING));
		SideBossContents[SIDE_SAMERING - 200] = createRadioButton(L"Same Tier Randomization", (tabSize.left + 12), (tabSize.top + 63), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_SAMERING));
		SideBossContents[SIDE_MIXRING - 200] = createRadioButton(L"Mixed Tier Randomization", (tabSize.left + 12), (tabSize.top + 81), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_MIXRING));
		SideBossContents[SIDE_MIXBOSS - 200] = createRadioButton(L"Mixed Story/Side Bosses", (tabSize.left + 12), (tabSize.top + 99), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_MIXBOSS));
		SideBossContents[SIDE_PREBOSSOPTIONS - 200] = createTextString(L"Preboss Encounter Options:", (tabSize.left + 7), (tabSize.top + 117), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSOPTIONS));
		SideBossContents[SIDE_NOPREBOSS - 200] = createRadioButton(L"No Preboss Randomization", (tabSize.left + 12), (tabSize.top + 135), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_NOPREBOSS));
		SideBossContents[SIDE_PREBOSSRANDO - 200] = createRadioButton(L"Randomize Preboss Encounters", (tabSize.left + 12), (tabSize.top + 153), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSRANDO));
		SideBossContents[SIDE_PREBOSSMIX - 200] = createRadioButton(L"Mix Story/Preboss Encounters", (tabSize.left + 12), (tabSize.top + 171), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSMIX));
		SideBossContents[SIDE_BOSSCHANCETEXT - 200] = createTextString(L"Side Boss Replacement Chance:", (tabSize.left + 200), (tabSize.top + 27), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_BOSSCHANCETEXT));
		SideBossContents[SIDE_BOSSCHANCE - 200] = createEditBox(L"", (tabSize.left + 205), (tabSize.top + 45), 50, 17, hwnd, MAKEINTRESOURCE(SIDE_BOSSCHANCE));
		SideBossContents[SIDE_BOSSCHANCERANGE - 200] = createTextString(L"Range: 0-100%", (tabSize.left + 258), (tabSize.top + 47), 100, 15, hwnd, MAKEINTRESOURCE(SIDE_BOSSCHANCERANGE));
		SideBossContents[SIDE_PREBOSSCHANCETEXT - 200] = createTextString(L"Preboss Replacement Chance:", (tabSize.left + 200), (tabSize.top + 65), 170, 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSCHANCETEXT));
		SideBossContents[SIDE_PREBOSSCHANCE - 200] = createEditBox(L"", (tabSize.left + 205), (tabSize.top + 83), 50, 17, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSCHANCE));
		SideBossContents[SIDE_PREBOSSCHANCERANGE - 200] = createTextString(L"Range: 0-100%", (tabSize.left + 258), (tabSize.top + 85), 100, 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSCHANCERANGE));

		// Description Box
		SideBossContents[SIDE_NORINGSTRING - 200] = createTextString(L"- Do not randomize the Side Story Bosses.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_NORINGSTRING));
		SideBossContents[SIDE_SAMERINGSTRING - 200] = createTextString(L"- Radomize the Side Story Bosses among those of the same tier.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_SAMERINGSTRING));
		SideBossContents[SIDE_MIXRINGSTRING - 200] = createTextString(L"- Randomize the Side Story Bosses .", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_MIXRINGSTRING));
		SideBossContents[SIDE_MIXBOSSSTRING - 200] = createTextString(L"- Include the Side Story Bosses in the pool with the Story bosses.\r  Follows Side Boss Replacement Chance.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SIDE_MIXBOSSSTRING));
		SideBossContents[SIDE_NOPREBOSSSTRING - 200] = createTextString(L"- Do not randomize the Encounters before Story Bosses.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_NOPREBOSSSTRING));
		SideBossContents[SIDE_PREBOSSRANDOSTRING - 200] = createTextString(L"- Randomizes the Encounters before Story Bosses among themselves.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSRANDOSTRING));
		SideBossContents[SIDE_PREBOSSMIXSTRING - 200] = createTextString(L"- Include the Encounters before Story Bosses with the Story Bosses Themselves.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSMIXSTRING));
		SideBossContents[SIDE_BOSSCHANCESTRING - 200] = createTextString(L"- X% Chance that a Side Story Boss\r  will replace a Story Boss.", (rectangle.left + 5), (rectangle.top + 74), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SIDE_BOSSCHANCESTRING));
		SideBossContents[SIDE_PREBOSSCHANCESTRING - 200] = createTextString(L"- X% Chance that an Pre-Story Boss encounter\r  will replace a Story Boss.", (rectangle.left + 5), (rectangle.top + 107), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SIDE_PREBOSSCHANCESTRING));

		// Hide all the side boss options
		for (int i = 0; i < 100; i++) {
			ShowWindow(SideBossContents[i], SW_HIDE);
		}

		// Scaling Options

		// Right side
		ScalingContents[SCALE_HARDMODE - 300] = createCheckButton(L"Enable Hardmode Scaling", (tabSize.left + 7), (tabSize.top + 27), 170, 15, hwnd, MAKEINTRESOURCE(SCALE_HARDMODE));
		ScalingContents[SCALE_BOSSLOC - 300] = createCheckButton(L"Stat Scaling by Location", (tabSize.left + 7), (tabSize.top + 45), 170, 15, hwnd, MAKEINTRESOURCE(SCALE_BOSSLOC));
		ScalingContents[SCALE_RANDOTEXT - 300] = createTextString(L"Boss Stat Randomization", (tabSize.left + 7), (tabSize.top + 63), 170, 15, hwnd, MAKEINTRESOURCE(SCALE_RANDOTEXT));
		ScalingContents[SCALE_RANDOBOX - 300] = createEditBox(L"", (tabSize.left + 12), (tabSize.top + 81), 50, 17, hwnd, MAKEINTRESOURCE(SCALE_RANDOBOX));
		ScalingContents[SCALE_RANDORANGE - 300] = createTextString(L"Range: 0-100%", (tabSize.left + 65), (tabSize.top + 81), 100, 15, hwnd, MAKEINTRESOURCE(SCALE_RANDORANGE));
		ScalingContents[SCALE_EXPTEXT - 300] = createTextString(L"EXP Multiplier", (tabSize.left + 7), (tabSize.top + 101), 170, 15, hwnd, MAKEINTRESOURCE(SCALE_EXPTEXT));
		ScalingContents[SCALE_EXPBOX - 300] = createEditBox(L"", (tabSize.left + 12), (tabSize.top + 119), 50, 17, hwnd, MAKEINTRESOURCE(SCALE_EXPBOX));
		ScalingContents[SCALE_EXPRANGE - 300] = createTextString(L"Range: 1-100x", (tabSize.left + 65), (tabSize.top + 121), 100, 15, hwnd, MAKEINTRESOURCE(SCALE_EXPRANGE));
		ScalingContents[SCALE_JPTEXT - 300] = createTextString(L"JP Multipler", (tabSize.left + 7), (tabSize.top + 139), 170, 15, hwnd, MAKEINTRESOURCE(SCALE_JPTEXT));
		ScalingContents[SCALE_JPBOX - 300] = createEditBox(L"", (tabSize.left + 12), (tabSize.top + 157), 50, 17, hwnd, MAKEINTRESOURCE(SCALE_JPBOX));
		ScalingContents[SCALE_JPRANGE - 300] = createTextString(L"Range: 1-100x", (tabSize.left + 65), (tabSize.top + 159), 100, 15, hwnd, MAKEINTRESOURCE(SCALE_JPRANGE));


		// Description Box
		ScalingContents[SCALE_NOHARDMODESTRING - 300] = createTextString(L"- Hardmode Boss Scaling is disabled.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SCALE_NOHARDMODESTRING));
		ScalingContents[SCALE_HARDMODESTRING - 300] = createTextString(L"- Applies Melodia's hardmode for compatibility reasons. \r  Adds a 1.35x multiplier to HP among other attributes.", (rectangle.left + 5), (rectangle.top + 23), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SCALE_HARDMODESTRING));
		ScalingContents[SCALE_NOBOSSLOCSTRING - 300] = createTextString(L"- Do not scale boss stats based on chapter location.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SCALE_NOBOSSLOCSTRING));
		ScalingContents[SCALE_BOSSLOCSTRING - 300] = createTextString(L"- Scale boss stats based on boss location. For example: a\r  Chapter 4 boss in a Chapter 1 area will have Chapter 1 stats.", (rectangle.left + 5), (rectangle.top + 56), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SCALE_BOSSLOCSTRING));
		ScalingContents[SCALE_RANDOSTRING - 300] = createTextString(L"- Randomizes each boss stat by up to +/- X%.\r  Can result in some crazy boss fights", (rectangle.left + 5), (rectangle.top + 89), ((rectangle.right - 5) - (rectangle.left + 5)), 30, hwnd, MAKEINTRESOURCE(SCALE_RANDOSTRING));
		ScalingContents[SCALE_EXPSTRING - 300] = createTextString(L"- Multiply the EXP rewards from bosses by X amount.", (rectangle.left + 5), (rectangle.top + 122), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SCALE_EXPSTRING));
		ScalingContents[SCALE_JPSTRING - 300] = createTextString(L"- Multiply the JP rewards from bosses by X amount.", (rectangle.left + 5), (rectangle.top + 140), ((rectangle.right - 5) - (rectangle.left + 5)), 15, hwnd, MAKEINTRESOURCE(SCALE_JPSTRING));


		// Hide all scaling options
		for (int i = 0; i < 100; i++) {
			ShowWindow(ScalingContents[i], SW_HIDE);
		}

		// Apply Config Options
		std::wstring pakDir;
		configParser(".\\working\\config.txt", &pakDir);
		SendDlgItemMessage(hwnd, EDITBOX, WM_SETTEXT, 0, (LPARAM)pakDir.c_str());

		// Update the config options by sending push button notifications
		switch (configs[0]) {
		case 0:
			SendMessage(MainBossContents[MAIN_DEFAULTMIX - 100], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(MainBossContents[MAIN_MIX24 - 100], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(MainBossContents[MAIN_MIX14 - 100], BM_CLICK, 0, 0);
			break;
		}
		switch (configs[1]) {
		case 0:
			SendMessage(MainBossContents[MAIN_NOSHRINE - 100], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(MainBossContents[MAIN_RANDOMSHRINE - 100], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(MainBossContents[MAIN_INCLUDESHRINE - 100], BM_CLICK, 0, 0);
			break;
		}
		switch (configs[2]) {
		case 0:
			SendMessage(MainBossContents[MAIN_NOGATE - 100], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(MainBossContents[MAIN_RANDOMGATE - 100], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(MainBossContents[MAIN_INCLUDEGATE - 100], BM_CLICK, 0, 0);
			break;
		}
		configs[3] == 0 ? SendMessage(MainBossContents[MAIN_NOGALDERA - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_INCLUDEGALDERA - 100], BM_CLICK, 0, 0);
		configs[4] == 0 ? SendMessage(MainBossContents[MAIN_NODUPLICATE - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_ALLOWDUPLICATE - 100], BM_CLICK, 0, 0);
		configs[5] == 0 ? SendMessage(MainBossContents[MAIN_PCWIN - 100], BM_CLICK, 0, 0) : SendMessage(MainBossContents[MAIN_GALDERAWIN - 100], BM_CLICK, 0, 0);
		if ((bool)configs[6] != (bool)SendMessage(MainBossContents[MAIN_FULLRANDOM - 100], BM_GETCHECK, 0, 0)) {
			SendMessage(MainBossContents[MAIN_FULLRANDOM - 100], BM_CLICK, 0, 0);
		}
		if ((bool)configs[7] != (bool)SendMessage(MainBossContents[MAIN_SOLORANDOM - 100], BM_GETCHECK, 0, 0)) {
			SendMessage(MainBossContents[MAIN_SOLORANDOM - 100], BM_CLICK, 0, 0);
		}
		if ((bool)configs[8] != (bool)SendMessage(MainBossContents[MAIN_FORCEBOSS - 100], BM_GETCHECK, 0, 0)) {
			SendMessage(MainBossContents[MAIN_FORCEBOSS - 100], BM_CLICK, 0, 0);
		}
		switch (configs[9]) {
		case 0:
			SendMessage(MainBossContents[MAIN_NOFORCEPC - 100], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(MainBossContents[MAIN_FORCEPC - 100], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(MainBossContents[MAIN_SPECIFICPC - 100], BM_CLICK, 0, 0);
			break;
		}
		// Force Boss Options are set when that dialog menu is entered
		SendMessage(GetDlgItem(hwnd, MAIN_SPECIFICPCOPTION), CB_SETCURSEL, configs[17], 0);
		switch (configs[18]) {
		case 0:
			SendMessage(SideBossContents[SIDE_NORING - 200], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(SideBossContents[SIDE_SAMERING - 200], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(SideBossContents[SIDE_MIXRING - 200], BM_CLICK, 0, 0);
			break;
		case 3:
			SendMessage(SideBossContents[SIDE_MIXBOSS - 200], BM_CLICK, 0, 0);
			break;
		}
		switch (configs[19]) {
		case 0:
			SendMessage(SideBossContents[SIDE_NOPREBOSS - 200], BM_CLICK, 0, 0);
			break;
		case 1:
			SendMessage(SideBossContents[SIDE_PREBOSSRANDO - 200], BM_CLICK, 0, 0);
			break;
		case 2:
			SendMessage(SideBossContents[SIDE_PREBOSSMIX - 200], BM_CLICK, 0, 0);
			break;
		}
		if ((bool)configs[22] != (bool)SendMessage(ScalingContents[SCALE_HARDMODE - 300], BM_GETCHECK, 0, 0)) {
			SendMessage(ScalingContents[SCALE_HARDMODE - 300], BM_CLICK, 0, 0);
		}
		if ((bool)configs[26] != (bool)SendMessage(ScalingContents[SCALE_BOSSLOC - 300], BM_GETCHECK, 0, 0)) {
			SendMessage(ScalingContents[SCALE_BOSSLOC - 300], BM_CLICK, 0, 0);
		}
		// Hide options selected by previous statements
		for (int i = 50; i < 100; i++) {
			ShowWindow(SideBossContents[i], SW_HIDE);
			ShowWindow(ScalingContents[i], SW_HIDE);
		}
		ApplyStringBox(0);

	}
	// Set font for child windows
	EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
	break;

	// Executed on windows close
	case WM_DESTROY:
	{
		// Save configs to file
		LRESULT len = SendMessage(GetDlgItem(hwnd, EDITBOX), WM_GETTEXTLENGTH, 0, 0);
		WCHAR* buffer = new WCHAR[len];
		SendMessage(GetDlgItem(hwnd, EDITBOX), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
		configWriter(".\\working\\config.txt", configs, buffer);
		PostQuitMessage(0);
		return 0;
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rectangle;
		RECT mainWindowSize;
		GetClientRect(hwnd, &mainWindowSize);
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		// Draw borders for the description box
		SetRect(&rectangle, (mainWindowSize.left + 10), 62, (mainWindowSize.right / 2 - 35), 440);
		DrawEdge(hdc, &rectangle, EDGE_SUNKEN, BF_RECT | BF_FLAT);
		EndPaint(hwnd, &ps);
	}
	break;

	// Button Color 
	case WM_CTLCOLORSTATIC:
	{
		if ((HWND)lParam == GetDlgItem(hwnd, DESCRIPTIONSTRING)) {
			// Make text a sight gray color
			SetBkMode((HDC)wParam, TRANSPARENT);
			SetTextColor((HDC)wParam, RGB(100, 100, 100));
		}
	}
	[[fallthrough]];
	case WM_CTLCOLORBTN:
	{
		// Make button background same color as window
		HBRUSH hBrushBtn = (HBRUSH)(COLOR_WINDOW + 1);
		return ((LRESULT)hBrushBtn);
	}
	break;

	// Tab menu option switches
	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
		{
			int iPage = TabCtrl_GetCurSel(GetDlgItem(hwnd, TABMENU));
			switch (iPage) {
			case 0:
				// Show the buttons for main options
				for (int i = 0; i < 50; i++) {
					ShowWindow(MainBossContents[i], SW_SHOW);
				}
				// Hide other options
				for (int i = 0; i < 100; i++) {
					ShowWindow(SideBossContents[i], SW_HIDE);
					ShowWindow(ScalingContents[i], SW_HIDE);
				}
				// Apply String based on configs
				ApplyStringBox(0);
				break;
			case 1:
				// Show Side Options
				for (int i = 0; i < 50; i++) {
					ShowWindow(SideBossContents[i], SW_SHOW);
				}
				// Hide Other Otions
				for (int i = 0; i < 100; i++) {
					ShowWindow(MainBossContents[i], SW_HIDE);
					ShowWindow(ScalingContents[i], SW_HIDE);
				}
				// Apply Strings based on configs
				ApplyStringBox(1);
				break;
			case 2:
				// Show Scaling Options
				for (int i = 0; i < 50; i++) {
					ShowWindow(ScalingContents[i], SW_SHOW);
				}

				// Hide other options
				for (int i = 0; i < 100; i++) {
					ShowWindow(MainBossContents[i], SW_HIDE);
					ShowWindow(SideBossContents[i], SW_HIDE);
				}
				// Apply Strings Based on configs
				ApplyStringBox(2);
				break;
			}
		}
		break;
		}
	}
	break;

	// Windows commands
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// Main button, randomizer button
		case RANDOMIZEBUTTON:
		{
			// Get the text from the pak dir box
			LRESULT len = SendMessage(GetDlgItem(hwnd, EDITBOX), WM_GETTEXTLENGTH, 0, 0);
			WCHAR* buffer = new WCHAR[len];
			SendMessage(GetDlgItem(hwnd, EDITBOX), WM_GETTEXT, (WPARAM)len + 1, (LPARAM)buffer);
			// Verify that the pak path does indeed contain the Octopath pak
			// Also allow for not inputing anything, and place folder in exe direcotry
			DWORD pakAttrib = GetFileAttributes((std::wstring(buffer) + L"\\Octopath_Traveler-WindowsNoEditor.pak").c_str());
			if (pakAttrib == INVALID_FILE_ATTRIBUTES && len != 0) {
				MessageBox(hwnd, L"Octopath Pak file not found in pak path.\nView usage for more details", L"Error Randomizing", MB_ICONEXCLAMATION | MB_OK);
			}
			else {
				// set the seed for the randomizer
				std::random_device dev;
				unsigned int seed;
				LRESULT seedLen = SendMessage(GetDlgItem(hwnd, SEEDBOX), WM_GETTEXTLENGTH, 0, 0);
				if (seedLen != 0) {
					WCHAR* seedBuffer = new WCHAR[seedLen];
					SendMessage(GetDlgItem(hwnd, SEEDBOX), WM_GETTEXT, (WPARAM)seedLen + 1, (LPARAM)seedBuffer);
					seed = _wtoi(seedBuffer);
				}
				else {
					seed = dev();
				}
				std::mt19937 rng;
				rng.seed(seed);
				vectorvector RandomizedBosses(7);
				vectorvector mainSideBosses(12);
				RandomizedBosses = uniformBossRandomize(rng, configs);
				// Do the side bosses 
				mainSideBosses = uniformSideInsertRandomize(rng, configs, RandomizedBosses, SideBossContents[SIDE_BOSSCHANCE - 200], SideBossContents[SIDE_PREBOSSCHANCE - 200]);
				// For solo randomization, generate the vector for the characters
				vectorvector soloCharacters(12);
				if (configs[7] == 1) {
					std::uniform_int_distribution<std::mt19937::result_type> soloRandom(1, 8);
					// first ring is always the same, except for Galdera
					mainSideBosses[0][0] == 45 || mainSideBosses[0][0] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(2);
					mainSideBosses[0][1] == 45 || mainSideBosses[0][1] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(8);
					mainSideBosses[0][2] == 45 || mainSideBosses[0][2] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(6);
					mainSideBosses[0][3] == 45 || mainSideBosses[0][3] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(5);
					mainSideBosses[0][4] == 45 || mainSideBosses[0][4] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(1);
					mainSideBosses[0][5] == 45 || mainSideBosses[0][5] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(4);
					mainSideBosses[0][6] == 45 || mainSideBosses[0][6] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(3);
					mainSideBosses[0][7] == 45 || mainSideBosses[0][7] == 46 ? soloCharacters[0].push_back(0) : soloCharacters[0].push_back(7);
					// Randomize the rest, paying attention to if Galdera is in the mix
					for (int i = 0; i < 8; i++) {
						mainSideBosses[1][i] == 45 || mainSideBosses[1][i] == 46 ? soloCharacters[1].push_back(0) : soloCharacters[1].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 8; i++) {
						mainSideBosses[2][i] == 45 || mainSideBosses[2][i] == 46 ? soloCharacters[2].push_back(0) : soloCharacters[2].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 9; i++) {
						mainSideBosses[3][i] == 45 || mainSideBosses[3][i] == 46 ? soloCharacters[3].push_back(0) : soloCharacters[3].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 4; i++) {
						mainSideBosses[4][i] == 45 || mainSideBosses[4][i] == 46 ? soloCharacters[4].push_back(0) : soloCharacters[4].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 8; i++) {
						mainSideBosses[5][i] == 45 || mainSideBosses[5][i] == 46 ? soloCharacters[5].push_back(0) : soloCharacters[5].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 2; i++) {
						mainSideBosses[6][i] == 45 || mainSideBosses[6][i] == 46 ? soloCharacters[6].push_back(0) : soloCharacters[6].push_back(soloRandom(rng));
					}
					for (int i = 0; i < 4; i++) {
						soloCharacters[7].push_back(0);
					}
					for (int i = 0; i < 4; i++) {
						soloCharacters[8].push_back(0);
					}
					for (int i = 0; i < 6; i++) {
						soloCharacters[9].push_back(0);
					}
					for (int i = 0; i < 2; i++) {
						soloCharacters[10].push_back(0);
					}
					for (int i = 0; i < 9; i++) {
						soloCharacters[11].push_back(0);
					}
					// Make spoiler logs with solo characters
					soloSpoiler(mainSideBosses, seed, soloCharacters);
				}
				else {
					// Make spoiler log based on normal inputs
					spoiler(mainSideBosses, seed);
				}
				// Apply to the game files the randomization options
				randomToAssetFile(mainSideBosses);

				// If scaling options are present, include those too
				randomStatsToAssets(rng, configs);

				// Now that all the asset files are done, apply the json event files
				eventFiles(soloCharacters, configs);

				// Now that all the files are in order, create a pak file with them
				try {
					//logFile << "************************************" << std::endl;
					//logFile << "Pak tool" << std::endl;
					//logFile << "************************************" << std::endl;
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
					//logFile.close();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Check if pak file was created, should always pass
				if (GetFileAttributes(L".\\working\\RandomizedBosses_P.pak") == INVALID_FILE_ATTRIBUTES) {
					//logFile << "Pak file not created, This shouldn't happen" << std::endl;
					//logFile.close();
					DisplayErrorMessageBox();
					SendMessage(hwnd, WM_DESTROY, 0, 0);
				}
				// Move pak file depending on entry in pak dir box
				if (len == 0) {
					try {
						if (GetFileAttributes(L".\\BossRandomizer") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\BossRandomizer", NULL);
						}
						if (GetFileAttributes(L".\\BossRandomizer\\Spoilers") == INVALID_FILE_ATTRIBUTES) {
							CreateDirectory(L".\\BossRandomizer\\Spoilers", NULL);
						}
						CopyFile(L".\\working\\RandomizedBosses_P.pak", L".\\BossRandomizer\\RandomizedBosses_P.pak", FALSE);
						CopyFile(L".\\working\\Boss Randomizer Spoilers.txt", L".\\BossRandomizer\\Spoilers\\Boss Randomizer Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Chapter 1 Boss Spoilers.txt", L".\\BossRandomizer\\Spoilers\\Chapter 1 Boss Spoilers.txt", FALSE);
						CopyFile(L".\\working\\Seed.txt", L".\\BossRandomizer\\Spoilers\\Seed.txt", FALSE);
						// Clean up root directory
						DeleteFile(L".\\working\\RandomizedBosses_P.pak");
						DeleteFile(L".\\working\\Boss Randomizer Spoilers.txt");
						DeleteFile(L".\\working\\Chapter 1 Boss Spoilers.txt");
						DeleteFile(L".\\working\\Seed.txt");
						//logFile << "Patch Files Left in Program Directory" << std::endl;
					}
					catch (std::exception& e) {
						std::cout << e.what() << std::endl;
						//logFile << "Error Copying Patch File: " << e.what() << "\n";
						//logFile.close();
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
						//logFile << "Patch File copied successfully" << std::endl;
					}
					catch (std::exception& e) {
						std::cout << e.what() << std::endl;
						//logFile << "Error Copying Patch File: " << e.what() << "\n";
						//logFile.close();
						DisplayErrorMessageBox();
						SendMessage(hwnd, WM_DESTROY, 0, 0);
					}
				}
				//logFile << "Randomization completed without error." << std::endl;
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
		// Config Sync Button
		case CONFIGSYNC:
			// Use a modal dialog box to prevent execution of the next lines until after closing
			DialogBox(NULL, MAKEINTRESOURCE(IDD_CONFIGSYNC), hwnd, (DLGPROC)ConfigDlgProc);
			break;

		// File Browse Button
		case FILEBUTTON:
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
								SendDlgItemMessage(hwnd, EDITBOX, WM_SETTEXT, 0, (LPARAM)pszFilePath);
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

		case SIDE_BOSSCHANCE:
			// Side Boss Percent Chance Edit Control
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
			{
				if (onEditChange == true) {
					configs[20] = editUpdate(SideBossContents[SIDE_BOSSCHANCE - 200], SideBossContents[SIDE_BOSSCHANCESTRING - 200], L"- Side Story Bosses can never replace a Story Boss.", L"- 100% Chance that a Side Story Boss\r  can replace a Story Boss.", L"- ", L"% Chance that a Side Story Boss\r  can replace a Story Boss.", L"0", L"100");
				}
			}
			break;
			}
			break;
		case SIDE_PREBOSSCHANCE:
			// Pre-Boss Percent Chance Edit Control
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
			{
				if (onEditChange == true) {
					configs[21] = editUpdate(SideBossContents[SIDE_PREBOSSCHANCE - 200], SideBossContents[SIDE_PREBOSSCHANCESTRING - 200], L"- Pre-Story Boss Encounters can never replace a Story Boss.", L"- 100% Chance that a Pre-Story Boss Encounter\r  can replace a Story Boss.", L"- ", L"% Chance that a Pre-Story Boss Encounter\r  can replace a Story Boss.", L"0", L"100");
				}
			}
			break;
			}
			break;
		case SCALE_RANDOBOX:
			// Stat Randomizer
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
			{
				if (onEditChange == true) {
					configs[23] = editUpdate(ScalingContents[SCALE_RANDOBOX - 300], ScalingContents[SCALE_RANDOSTRING - 300], L"- Boss Stats will not be randomized", L"- Randomizes each boss stat by up to +/- 100%.\r  Can result in some crazy boss fights", L"- Randomizes each boss stat by up to +/- ", L"%.\r  Can result in some crazy boss fights", L"0", L"100");
				}
			}
			break;
			}
			break;
		case SCALE_EXPBOX:
			// EXP Multiplier
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
			{
				if (onEditChange == true) {
					configs[24] = editUpdate(ScalingContents[SCALE_EXPBOX - 300], ScalingContents[SCALE_EXPSTRING - 300], L"- Do not change the EXP earned from bosses", L"- Multiply the EXP earned from bosses by 100 times", L"- Multiply the EXP earned from bosses by ", L" times", L"1", L"100");
				}
			}
			break;
			}
			break;
		case SCALE_JPBOX:
			// JP Multipler
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
			{
				if (onEditChange == true) {
					configs[25] = editUpdate(ScalingContents[SCALE_JPBOX - 300], ScalingContents[SCALE_JPSTRING - 300], L"- Do not change the JP earned from bosses", L"- Multiply the JP earned from bosses by 100 times", L"- Multiply the JP earned from bosses by ", L" times", L"1", L"100");
				}
			}
			break;
			}
			break;
		case MAIN_DEFAULTMIX:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 1, MAIN_DEFAULTMIX, MAIN_MIX24, MAIN_MIX14, MainBossContents[MAIN_DEFAULTMIXSTRING - 100], MainBossContents[MAIN_MIX24STRING - 100], MainBossContents[MAIN_MIX14STRING - 100]);
				configs[0] = 0;
			}
			break;
		case MAIN_MIX24:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 2, MAIN_DEFAULTMIX, MAIN_MIX24, MAIN_MIX14, MainBossContents[MAIN_DEFAULTMIXSTRING - 100], MainBossContents[MAIN_MIX24STRING - 100], MainBossContents[MAIN_MIX14STRING - 100]);
				configs[0] = 1;
			}
			break;
		case MAIN_MIX14:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 3, MAIN_DEFAULTMIX, MAIN_MIX24, MAIN_MIX14, MainBossContents[MAIN_DEFAULTMIXSTRING - 100], MainBossContents[MAIN_MIX24STRING - 100], MainBossContents[MAIN_MIX14STRING - 100]);
				configs[0] = 2;
			}
			break;
		case MAIN_NOSHRINE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 1, MAIN_NOSHRINE, MAIN_RANDOMSHRINE, MAIN_INCLUDESHRINE, MainBossContents[MAIN_NOSHRINESTRING - 100], MainBossContents[MAIN_RANDOMSHRINESTRING - 100], MainBossContents[MAIN_INCLUDESHRINESTRING - 100]);
				configs[1] = 0;
			}
			break;
		case MAIN_RANDOMSHRINE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 2, MAIN_NOSHRINE, MAIN_RANDOMSHRINE, MAIN_INCLUDESHRINE, MainBossContents[MAIN_NOSHRINESTRING - 100], MainBossContents[MAIN_RANDOMSHRINESTRING - 100], MainBossContents[MAIN_INCLUDESHRINESTRING - 100]);
				configs[1] = 1;
			}
			break;
		case MAIN_INCLUDESHRINE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 3, MAIN_NOSHRINE, MAIN_RANDOMSHRINE, MAIN_INCLUDESHRINE, MainBossContents[MAIN_NOSHRINESTRING - 100], MainBossContents[MAIN_RANDOMSHRINESTRING - 100], MainBossContents[MAIN_INCLUDESHRINESTRING - 100]);
				configs[1] = 2;
			}
			break;
		case MAIN_NOGATE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 1, MAIN_NOGATE, MAIN_RANDOMGATE, MAIN_INCLUDEGATE, MainBossContents[MAIN_NOGATESTRING - 100], MainBossContents[MAIN_RANDOMGATESTRING - 100], MainBossContents[MAIN_INCLUDEGATESTRING - 100]);
				configs[2] = 0;
			}
			break;
		case MAIN_RANDOMGATE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 2, MAIN_NOGATE, MAIN_RANDOMGATE, MAIN_INCLUDEGATE, MainBossContents[MAIN_NOGATESTRING - 100], MainBossContents[MAIN_RANDOMGATESTRING - 100], MainBossContents[MAIN_INCLUDEGATESTRING - 100]);
				configs[2] = 1;
			}
			break;

		case MAIN_INCLUDEGATE:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 3, MAIN_NOGATE, MAIN_RANDOMGATE, MAIN_INCLUDEGATE, MainBossContents[MAIN_NOGATESTRING - 100], MainBossContents[MAIN_RANDOMGATESTRING - 100], MainBossContents[MAIN_INCLUDEGATESTRING - 100]);
				configs[2] = 2;
			}
			break;
		case MAIN_NOGALDERA:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 1, MAIN_NOGALDERA, MAIN_INCLUDEGALDERA, MainBossContents[MAIN_NOGALDERASTRING - 100], MainBossContents[MAIN_INCLUDEGALDERASTRING - 100]);
				configs[3] = 0;
			}
			break;
		case MAIN_INCLUDEGALDERA:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 2, MAIN_NOGALDERA, MAIN_INCLUDEGALDERA, MainBossContents[MAIN_NOGALDERASTRING - 100], MainBossContents[MAIN_INCLUDEGALDERASTRING - 100]);
				configs[3] = 1;
			}
			break;
		case MAIN_NODUPLICATE:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 1, MAIN_NODUPLICATE, MAIN_ALLOWDUPLICATE, MainBossContents[MAIN_NODUPLICATESTRING - 100], MainBossContents[MAIN_ALLOWDUPLICATESTRING - 100]);
				configs[4] = 0;
			}
			break;
		case MAIN_ALLOWDUPLICATE:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 2, MAIN_NODUPLICATE, MAIN_ALLOWDUPLICATE, MainBossContents[MAIN_NODUPLICATESTRING - 100], MainBossContents[MAIN_ALLOWDUPLICATESTRING - 100]);
				configs[4] = 1;
			}
			break;
		case MAIN_PCWIN:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 1, MAIN_PCWIN, MAIN_GALDERAWIN, MainBossContents[MAIN_PCWINSTRING - 100], MainBossContents[MAIN_GALDERAWINSTRING - 100]);
				configs[5] = 0;
			}
			break;
		case MAIN_GALDERAWIN:
			if (HIWORD(wParam) == BN_CLICKED) {
				duoStateCheck(hwnd, 2, MAIN_PCWIN, MAIN_GALDERAWIN, MainBossContents[MAIN_PCWINSTRING - 100], MainBossContents[MAIN_GALDERAWINSTRING - 100]);
				configs[5] = 1;
			}
			break;
		case MAIN_FULLRANDOM:
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((bool)IsDlgButtonChecked(hwnd, MAIN_FULLRANDOM) == true) {
					CheckDlgButton(hwnd, MAIN_FULLRANDOM, BST_UNCHECKED);
					ShowWindow(MainBossContents[MAIN_NOFULLRANDOMSTRING - 100], SW_SHOW);
					ShowWindow(MainBossContents[MAIN_FULLRANDOMSTRING - 100], SW_HIDE);
					configs[6] = 0;

				}
				else {
					CheckDlgButton(hwnd, MAIN_FULLRANDOM, BST_CHECKED);
					ShowWindow(MainBossContents[MAIN_NOFULLRANDOMSTRING - 100], SW_HIDE);
					ShowWindow(MainBossContents[MAIN_FULLRANDOMSTRING - 100], SW_SHOW);
					configs[6] = 1;
				}
			}
			break;
		case MAIN_SOLORANDOM:
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((bool)IsDlgButtonChecked(hwnd, MAIN_SOLORANDOM) == true) {
					CheckDlgButton(hwnd, MAIN_SOLORANDOM, BST_UNCHECKED);
					ShowWindow(MainBossContents[MAIN_NOSOLORANDOMSTRING - 100], SW_SHOW);
					ShowWindow(MainBossContents[MAIN_SOLORANDOMSTRING - 100], SW_HIDE);
					configs[7] = 0;

				}
				else {
					CheckDlgButton(hwnd, MAIN_SOLORANDOM, BST_CHECKED);
					ShowWindow(MainBossContents[MAIN_NOSOLORANDOMSTRING - 100], SW_HIDE);
					ShowWindow(MainBossContents[MAIN_SOLORANDOMSTRING - 100], SW_SHOW);
					configs[7] = 1;
				}
			}
			break;
		case MAIN_FORCEBOSS:
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((bool)IsDlgButtonChecked(hwnd, MAIN_FORCEBOSS) == true) {
					CheckDlgButton(hwnd, MAIN_FORCEBOSS, BST_UNCHECKED);
					ShowWindow(MainBossContents[MAIN_NOFORCESTRING - 100], SW_SHOW);
					ShowWindow(MainBossContents[MAIN_FORCESTRING - 100], SW_HIDE);
					configs[8] = 0;

				}
				else {
					CheckDlgButton(hwnd, MAIN_FORCEBOSS, BST_CHECKED);
					ShowWindow(MainBossContents[MAIN_NOFORCESTRING - 100], SW_HIDE);
					ShowWindow(MainBossContents[MAIN_FORCESTRING - 100], SW_SHOW);
					configs[8] = 1;
				}
			}
			break;
		case MAIN_NOFORCEPC:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 1, MAIN_NOFORCEPC, MAIN_FORCEPC, MAIN_SPECIFICPC, MainBossContents[MAIN_NOFORCEPCSTRING - 100], MainBossContents[MAIN_FORCEPCSTRING - 100], MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100]);
				configs[9] = 0;
			}
			break;
		case MAIN_FORCEPC:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 2, MAIN_NOFORCEPC, MAIN_FORCEPC, MAIN_SPECIFICPC, MainBossContents[MAIN_NOFORCEPCSTRING - 100], MainBossContents[MAIN_FORCEPCSTRING - 100], MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100]);
				configs[9] = 1;
			}
			break;
		case MAIN_SPECIFICPC:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 3, MAIN_NOFORCEPC, MAIN_FORCEPC, MAIN_SPECIFICPC, MainBossContents[MAIN_NOFORCEPCSTRING - 100], MainBossContents[MAIN_FORCEPCSTRING - 100], MainBossContents[MAIN_FORCESPECIFICPCSTRING - 100]);
				configs[9] = 2;
			}
			break;
		case MAIN_FORCEBOSSOPTION:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_FORCEBOSS), hwnd, (DLGPROC)OptionDlgProc);
			break;
		case MAIN_SPECIFICPCOPTION:
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				configs[17] = (int)SendMessage(MainBossContents[MAIN_SPECIFICPCOPTION - 100], CB_GETCURSEL, 0, 0);
			}
			break;
		case SIDE_NORING:
			if (HIWORD(wParam) == BN_CLICKED) {
				quadStateCheck(hwnd, 1, SIDE_NORING, SIDE_SAMERING, SIDE_MIXRING, SIDE_MIXBOSS, SideBossContents[SIDE_NORINGSTRING - 200], SideBossContents[SIDE_SAMERINGSTRING - 200], SideBossContents[SIDE_MIXRINGSTRING - 200], SideBossContents[SIDE_MIXBOSSSTRING - 200]);
				configs[18] = 0;
			}
			break;
		case SIDE_SAMERING:
			if (HIWORD(wParam) == BN_CLICKED) {
				quadStateCheck(hwnd, 2, SIDE_NORING, SIDE_SAMERING, SIDE_MIXRING, SIDE_MIXBOSS, SideBossContents[SIDE_NORINGSTRING - 200], SideBossContents[SIDE_SAMERINGSTRING - 200], SideBossContents[SIDE_MIXRINGSTRING - 200], SideBossContents[SIDE_MIXBOSSSTRING - 200]);
				configs[18] = 1;
			}
			break;
		case SIDE_MIXRING:
			if (HIWORD(wParam) == BN_CLICKED) {
				quadStateCheck(hwnd, 3, SIDE_NORING, SIDE_SAMERING, SIDE_MIXRING, SIDE_MIXBOSS, SideBossContents[SIDE_NORINGSTRING - 200], SideBossContents[SIDE_SAMERINGSTRING - 200], SideBossContents[SIDE_MIXRINGSTRING - 200], SideBossContents[SIDE_MIXBOSSSTRING - 200]);
				configs[18] = 2;
			}
			break;
		case SIDE_MIXBOSS:
			if (HIWORD(wParam) == BN_CLICKED) {
				quadStateCheck(hwnd, 4, SIDE_NORING, SIDE_SAMERING, SIDE_MIXRING, SIDE_MIXBOSS, SideBossContents[SIDE_NORINGSTRING - 200], SideBossContents[SIDE_SAMERINGSTRING - 200], SideBossContents[SIDE_MIXRINGSTRING - 200], SideBossContents[SIDE_MIXBOSSSTRING - 200]);
				configs[18] = 3;
			}
			break;
		case SIDE_NOPREBOSS:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 1, SIDE_NOPREBOSS, SIDE_PREBOSSRANDO, SIDE_PREBOSSMIX, SideBossContents[SIDE_NOPREBOSSSTRING - 200], SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SideBossContents[SIDE_PREBOSSMIXSTRING - 200]);
				configs[19] = 0;
			}
			break;
		case SIDE_PREBOSSRANDO:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 2, SIDE_NOPREBOSS, SIDE_PREBOSSRANDO, SIDE_PREBOSSMIX, SideBossContents[SIDE_NOPREBOSSSTRING - 200], SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SideBossContents[SIDE_PREBOSSMIXSTRING - 200]);
				configs[19] = 1;
			}
			break;
		case SIDE_PREBOSSMIX:
			if (HIWORD(wParam) == BN_CLICKED) {
				triStateCheck(hwnd, 3, SIDE_NOPREBOSS, SIDE_PREBOSSRANDO, SIDE_PREBOSSMIX, SideBossContents[SIDE_NOPREBOSSSTRING - 200], SideBossContents[SIDE_PREBOSSRANDOSTRING - 200], SideBossContents[SIDE_PREBOSSMIXSTRING - 200]);
				configs[19] = 2;
			}
			break;
		case SCALE_HARDMODE:
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((bool)IsDlgButtonChecked(hwnd, SCALE_HARDMODE) == true) {
					CheckDlgButton(hwnd, SCALE_HARDMODE, BST_UNCHECKED);
					ShowWindow(ScalingContents[SCALE_NOHARDMODESTRING - 300], SW_SHOW);
					ShowWindow(ScalingContents[SCALE_HARDMODESTRING - 300], SW_HIDE);
					configs[22] = 0;

				}
				else {
					CheckDlgButton(hwnd, SCALE_HARDMODE, BST_CHECKED);
					ShowWindow(ScalingContents[SCALE_NOHARDMODESTRING - 300], SW_HIDE);
					ShowWindow(ScalingContents[SCALE_HARDMODESTRING - 300], SW_SHOW);
					configs[22] = 1;
				}
			}
			break;
		case SCALE_BOSSLOC:
			if (HIWORD(wParam) == BN_CLICKED) {
				if ((bool)IsDlgButtonChecked(hwnd, SCALE_BOSSLOC) == true) {
					CheckDlgButton(hwnd, SCALE_BOSSLOC, BST_UNCHECKED);
					ShowWindow(ScalingContents[SCALE_NOBOSSLOCSTRING - 300], SW_SHOW);
					ShowWindow(ScalingContents[SCALE_BOSSLOCSTRING - 300], SW_HIDE);
					configs[26] = 0;

				}
				else {
					CheckDlgButton(hwnd, SCALE_BOSSLOC, BST_CHECKED);
					ShowWindow(ScalingContents[SCALE_NOBOSSLOCSTRING - 300], SW_HIDE);
					ShowWindow(ScalingContents[SCALE_BOSSLOCSTRING - 300], SW_SHOW);
					configs[26] = 1;
				}
			}
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}