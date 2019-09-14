// CaesarCipherGUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CaesarCipherGUI.h"
#include <ShObjIdl.h>
#include <string>
#include <algorithm>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hMTBE;										// Message to be encrypted handle
HWND hMTBER;									// Message to be encrypted response handle
HWND hLS;										// Letter shift handle
HWND hLSC;										// Letter shift combobox handle
HWND hS;										// Spacing handle
HWND hSR;										// Spacing response handle
HWND hNM;										// New Message handle
wchar_t bto_be_encrypted[32768];
std::wstring to_be_encrypted = std::wstring();	// Gets text from hMTBER
wchar_t bspacing[6];						
std::wstring spacing = std::wstring();			// Gets text from hSR
std::wstring encrypted = std::wstring();		// Encrypted text
int shift = 1;									// Letter shift
unsigned long check = 0UL;						// Used to check whether spacing is valid
wchar_t Numbers[26][26] =
{
	TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"),
	TEXT("5"), TEXT("6"), TEXT("7"), TEXT("8"),
	TEXT("9"), TEXT("10"), TEXT("11"), TEXT("12"),
	TEXT("13"), TEXT("14"), TEXT("15"), TEXT("16"),
	TEXT("17"), TEXT("18"), TEXT("19"), TEXT("20"),
	TEXT("21"), TEXT("22"), TEXT("23"), TEXT("24"),
	TEXT("25"), TEXT("26")
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CAESARCIPHERGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CAESARCIPHERGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CAESARCIPHERGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


std::wstring removeSpace(std::wstring trim)
{
	trim.erase(remove_if(trim.begin(), trim.end(), isspace), trim.end());
	return trim;
}

std::wstring addSpace(std::wstring add_space, unsigned long spacing)
{
	std::wstring withSpacing = std::wstring();
	add_space = add_space + L"";
	if (add_space.length() % spacing != 0)
	{
		std::wstring x = std::wstring();
		for (unsigned long i = 0; i < spacing - add_space.length() % spacing; i++)
		{
			x = x + L"x";
		}
		add_space = add_space + x;
	}
	while (add_space.length() > 0)
	{
		std::wstring subString = add_space.substr(0, spacing);
		withSpacing = withSpacing + subString + L" ";
		add_space = add_space.substr(spacing);
	}
	return withSpacing;
}


std::wstring Convert(wchar_t letter, unsigned long number)
{
	std::wstring ABC = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::wstring newletter = std::wstring();
	bool isletter = false;
	int i;
	for (i = 0; i < 26; i++)
	{
		if (ABC[i] == toupper(letter))
		{
			std::wstring a(1, ABC[(i + number) % 26]);
			newletter = a;
			isletter = true;
		}
	}
	if (!isletter)
	{
		std::wstring a(1, letter);
		newletter = a;
	}
	return newletter;
}

std::wstring Encrypt(std::wstring encrypt, unsigned long num)
{
	std::wstring encrypted = std::wstring();
	unsigned long i = 0;
	while (i < encrypt.length())
	{
		encrypted = encrypted + Convert(encrypt[i], num);
		i++;
	}
	return encrypted;
}

unsigned long pow(unsigned long base, unsigned long exp)
{
	unsigned long return_value = 1;
	if (exp == 0 || exp < 0)
	{
		return 1;
	}
	for (unsigned long i = 0; i < exp; i = i + 1)
	{
		return_value = return_value * base;
	}
	return return_value;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetClientRect(hWnd, &window);
    switch (message)
    {
    case WM_COMMAND:
        {
			if(HIWORD(wParam) == CBN_SELCHANGE)
	        { 
				shift = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
				shift++;
				break;
			}
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_ENCRYPT:
				GetWindowTextW(hMTBER, bto_be_encrypted, 32768);
				GetWindowTextW(hSR, bspacing, 6);
				to_be_encrypted = std::wstring(bto_be_encrypted);
				spacing = std::wstring(bspacing);
				if (spacing == L"")
				{
					check = 0UL;
					SetWindowTextW(hSR, L"0");
				}
				else
				{
					check = std::stoul(spacing);
				}
				if (check > to_be_encrypted.length())
				{
					MessageBoxW(hWnd, L"Spacing must be less than the amount of non-space characters in message.", L"Spacing too large", MB_ICONERROR | MB_OK);
					break;
				}
				if (check == 0)
				{
					encrypted = Encrypt(to_be_encrypted, shift);
				}
				else
				{
					std::wstring spaced_out = Encrypt(removeSpace(to_be_encrypted), shift);
					encrypted = addSpace(spaced_out, std::stoi(spacing));
				}
				SetWindowTextW(hNM, encrypted.c_str());
				break;
			case IDM_DEMO:
				SetWindowTextW(hMTBER, L"Thank you for using Adeethyia Shankar's Caesar Cipher");
				SetWindowTextW(hSR, L"4");
				SendMessage((HWND)hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_ENCRYPT, (LPARAM)0);
				break;
			case IDM_HELP:
			{
				int choice = MessageBoxW(hWnd, L"Encrypt a message in the Caesar Cipher method\n\n1. Enter a message in the first box (e.g. \"Adeethyia Shankar is great\")\n2. Select an amount to shift all letters in the message down the alphabet (e.g. 1 would change 'a' to 'b')\n3. Enter a custom frequency of spacing for added encryptness, or retain the original spacing (e.g. 3 would mean a space every third letter)\n\nPress OK for a demo", L"Help", MB_OKCANCEL);
				if (choice == IDOK)
				{
					SendMessage(hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_DEMO, (LPARAM)0);
				}
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		hMTBE = CreateWindowExW(0L, L"static", L"Message to be Encrypted:", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, window.right / 3, window.bottom / 6, hWnd, NULL, NULL, NULL);
		hMTBER = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL, window.right / 3, 0, 2 * window.right / 3, window.bottom / 6, hWnd, NULL, NULL, NULL);
		hLS = CreateWindowExW(0L, L"static", L"Letter Shift:", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, window.bottom / 6, window.right / 3, window.bottom / 6, hWnd, NULL, NULL, NULL);
		hLSC = CreateWindowExW(0L, L"ComboBox", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, window.right / 3, window.bottom / 6, 2 * window.right / 3, window.bottom / 6, hWnd, NULL, NULL, NULL);
		TCHAR A[16];
		memset(&A, 0, sizeof(A));
		for (int k = 0; k < 26; k ++)
		{
			wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Numbers[k]);
			// Add string to combobox.
			SendMessage(hLSC, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
		}
		// Send the CB_SETCURSEL message to display an initial item in the selection field  
		SendMessage(hLSC, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		hS = CreateWindowExW(0L, L"static", L"Spacing per how many letters:\n(Put 0 for original spacing)", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 2 * window.right / 3, window.bottom / 5, window.bottom / 5, hWnd, NULL, NULL, NULL);
		hSR = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, window.right / 3, 2 * window.bottom / 6, window.right, 2 * window.bottom / 6, hWnd, NULL, NULL, NULL);
		hNM = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL, 0, 3 * window.bottom / 6, window.right, 3 * window.bottom / 6, hWnd, NULL, NULL, NULL);
	case WM_PAINT:
        {
			GetWindowTextW(hMTBER, bto_be_encrypted, 32768);
			GetWindowTextW(hSR, bspacing, 6);
			to_be_encrypted = std::wstring(bto_be_encrypted);
			spacing = std::wstring(bspacing);
			MoveWindow(hMTBE, 0, 0, window.right / 3, window.bottom / 6, true);
			MoveWindow(hMTBER, window.right / 3, 0, 2 * window.right / 3, window.bottom / 6, true);
			MoveWindow(hLS, 0, window.bottom / 6, window.right / 3, window.bottom / 6, true);
			MoveWindow(hLSC, window.right / 3, window.bottom / 6, 2 * window.right / 3, window.bottom / 6, true);
			MoveWindow(hS, 0, 2 * window.bottom / 6, window.right / 3, window.bottom / 6, true);
			MoveWindow(hSR, window.right / 3, 2 * window.bottom / 6, window.right, 2 * window.bottom / 6, true);
			MoveWindow(hNM, 0, 3 * window.bottom / 6, window.right, 3 * window.bottom / 6, true);
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
