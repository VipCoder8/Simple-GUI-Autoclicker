#define LEFT_CAPTURE_BUTTON_PRESSED 0
#define RIGHT_CAPTURE_BUTTON_PRESSED 1
#define ACTIVATE_BUTTON 2
#define STOP_BUTTON 3

#include <windows.h>
#include <iostream>
#include <string>
#include <windowsx.h>
#include <cctype>

DWORD WINAPI autoclick(LPVOID lpParam);

HWND rightCaptureButton;
HWND rightCapturedInputText;
HWND leftCaptureButton;
HWND leftCapturedInputText;
HWND delayText;
HWND activateButton;
HWND stopButton;

LPCSTR inputText;
char delay[100];

bool left_capturing = false;
bool right_capturing = false;
bool selected_left_key = false;
bool selected_right_key = false;
bool checking_input_to_autoclick = false;

int rightMouseButtonKey;
int leftMouseButtonKey;

RECT windowSize;

HANDLE hThread;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
		case WM_CREATE:
			leftCaptureButton = CreateWindow(
                    	"BUTTON",
                    	"Click to Capture Input(Left Mouse Button)",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	0,
                    	0,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)LEFT_CAPTURE_BUTTON_PRESSED,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		leftCapturedInputText = CreateWindow(
                    	"EDIT",
                    	"Captured Input Here",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	0,
                    	33,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)-2,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		rightCaptureButton = CreateWindow(
                    	"BUTTON",
                    	"Click to Capture Input(Right Mouse Button)",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	310,
                    	0,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)RIGHT_CAPTURE_BUTTON_PRESSED,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		rightCapturedInputText = CreateWindow(
                    	"EDIT",
                    	"Captured Input Here",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	310,
                    	33,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)-1,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		delayText = CreateWindow(
                    	"EDIT",
                    	"Delay(Don't forget to change)",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	310/2,
                    	66,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)-1,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		activateButton = CreateWindow(
                    	"BUTTON",
                    	"Activate",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	310/2,
                    	132,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)ACTIVATE_BUTTON,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		activateButton = CreateWindow(
                    	"BUTTON",
                    	"Stop",
                    	WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    	310/2,
                    	164,
                    	300,
                    	30,
                    	hwnd,
                    	(HMENU)STOP_BUTTON,
                    	((LPCREATESTRUCT)lParam)->hInstance,
                    	NULL
                );
		break;
		case WM_COMMAND:
			switch(wParam) {
				case LEFT_CAPTURE_BUTTON_PRESSED:
					left_capturing = true;
					SetFocus(hwnd);
				break;
				case RIGHT_CAPTURE_BUTTON_PRESSED:
					right_capturing = true;
					SetFocus(hwnd);
				break;
				case STOP_BUTTON:
					checking_input_to_autoclick = false;
				break;
				case ACTIVATE_BUTTON:
					GetWindowText(delayText, delay, 100);
					int intDelay = std::stoi(delay);
					checking_input_to_autoclick = true;
					hThread = CreateThread(
        					NULL,                   // Default security attributes
						0,                      // Default stack size
						autoclick,       // Thread function to be executed
						reinterpret_cast<LPVOID>(&intDelay),        // Parameter to be passed to the thread function
						0,                      // Default creation flags
						NULL                    // Pointer to a variable to receive the thread identifier
    					);
				break;
			}
		break;
		case WM_CHAR:
			if(left_capturing) {
				leftMouseButtonKey = wParam;
				std::string capturedInput;
				capturedInput+=(char)wParam;
				inputText = static_cast<LPCSTR>(capturedInput.c_str());
				SetWindowText(leftCapturedInputText, inputText);
				selected_left_key = true;
				left_capturing = false;
			}
			if(right_capturing) {
				rightMouseButtonKey = wParam;
				std::string capturedInput;
				capturedInput+=(char)wParam;
				inputText = static_cast<LPCSTR>(capturedInput.c_str());
				SetWindowText(rightCapturedInputText, inputText);
				selected_right_key=true;
				right_capturing = false;
			}
		break;
		case WM_KEYDOWN:
			if(left_capturing) {
				if((!std::isalpha((char)wParam)||!std::isdigit((char)wParam)) && wParam != VK_SHIFT) {
					leftMouseButtonKey = wParam;
					std::string capturedInput;
					if(wParam > 111 && wParam <= 123) {
						capturedInput+='F';
						capturedInput+=std::to_string((int)wParam-111);
					} else {
						capturedInput+=(char)wParam;
					}
					inputText = static_cast<LPCSTR>(capturedInput.c_str());
					SetWindowText(leftCapturedInputText, inputText);
					selected_left_key=true;
					left_capturing = false;
				}
			}
			if(right_capturing) {
				if((!std::isalpha((char)wParam)||!std::isdigit((char)wParam)) && wParam != VK_SHIFT) {
					rightMouseButtonKey = wParam;
					std::string capturedInput;
					if(wParam > 111 && wParam <= 123) {
						capturedInput+='F';
						capturedInput+=std::to_string((int)wParam-111);
					} else {
						capturedInput+=(char)wParam;
					}
					inputText = static_cast<LPCSTR>(capturedInput.c_str());
					SetWindowText(rightCapturedInputText, inputText);
					selected_right_key=true;
					right_capturing = false;
				}
			}
		break;	
		case WM_SYSKEYDOWN:
		if(left_capturing) {
				if((!std::isalpha((char)wParam)||!std::isdigit((char)wParam)) && wParam != VK_SHIFT) {
					leftMouseButtonKey = wParam;
					std::string capturedInput;
					if(wParam > 111 && wParam <= 123) {
						capturedInput+='F';
						capturedInput+=std::to_string((int)wParam-111);
					} else {
						capturedInput+=(char)wParam;
					}
					inputText = static_cast<LPCSTR>(capturedInput.c_str());
					SetWindowText(leftCapturedInputText, inputText);
					selected_left_key=true;
					left_capturing = false;
				}
			}
			if(right_capturing) {
				if((!std::isalpha((char)wParam)||!std::isdigit((char)wParam)) && wParam != VK_SHIFT) {
					rightMouseButtonKey = wParam;
					std::string capturedInput;
					if(wParam > 111 && wParam <= 123) {
						capturedInput+='F';
						capturedInput+=std::to_string((int)wParam-111);
					} else {
						capturedInput+=(char)wParam;
					}
					inputText = static_cast<LPCSTR>(capturedInput.c_str());
					SetWindowText(rightCapturedInputText, inputText);
					selected_right_key=true;
					right_capturing = false;
				}
			}
		break;
		case WM_XBUTTONUP:
			std::cout << left_capturing << std::endl;
			if(left_capturing) {
				if(wParam==131072) {
					leftMouseButtonKey=VK_XBUTTON1;
					SetWindowText(leftCapturedInputText, "Fourth Mouse Button\0");
					selected_left_key=true;
					left_capturing = false;
				} else if(wParam==65536) {
					leftMouseButtonKey=VK_XBUTTON2;
					SetWindowText(leftCapturedInputText, "Fifth Mouse Button\0");
					selected_left_key=true;
					left_capturing = false;
				}
			}
			if(right_capturing) {
				if(wParam==131072) {
					rightMouseButtonKey=VK_XBUTTON1;
					SetWindowText(rightCapturedInputText, "Fourth Mouse Button\0");
					selected_right_key=true;
					right_capturing=false;
				} else if(wParam==65536) {
					rightMouseButtonKey=VK_XBUTTON2;
					SetWindowText(rightCapturedInputText, "Fifth Mouse Button\0");
					selected_right_key=true;
					right_capturing=false;
				}
			}
		break;
		case WM_DESTROY:
	            PostQuitMessage(0);
	            return 0;
	        default:
	            return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    	// Step 1: Register Window Class
	WNDCLASS wc = {0};
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    	wc.lpfnWndProc = WindowProc;
    	wc.hInstance = hInstance;
    	wc.lpszClassName = "MyWindowClass";

    	RegisterClass(&wc);

    	// Step 2: Create Window
    	HWND hwnd = CreateWindowEx(
        	0,
        	"MyWindowClass",
        	NULL,
        	WS_OVERLAPPEDWINDOW,
        	0, 0,  //X, Y position
        	626, 310,           //width, height
        	NULL, NULL, hInstance, NULL);

    	if (hwnd == NULL)
    	{
        	return 0;
    	}
	SetWindowText(hwnd, "AutoGLicker");
    	// Step 3: Show and Update the Window
    	ShowWindow(hwnd, nCmdShow);
    	UpdateWindow(hwnd);

    	// Step 4: Message Loop
    	MSG msg;
    	while (GetMessage(&msg, NULL, 0, 0))
    	{
        	TranslateMessage(&msg);
        	DispatchMessage(&msg);
    	}

    	return msg.wParam;
}
void leftClick() {
	INPUT input = {0};
    	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1, &input, sizeof(INPUT));

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}
void rightClick() {
	INPUT input = {0};
    	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    	SendInput(1, &input, sizeof(INPUT));

    	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    	SendInput(1, &input, sizeof(INPUT));
}
bool isKeyPressed(int key) {
	return GetAsyncKeyState(key) & 0x8000;
}
DWORD WINAPI autoclick(LPVOID lpParam) {
    	int* delayPtr = reinterpret_cast<int*>(lpParam);
	int delay = *delayPtr;
	while(checking_input_to_autoclick) {
		if(isKeyPressed(leftMouseButtonKey)) {
			leftClick();
			Sleep(delay);
		}
		if(isKeyPressed(rightMouseButtonKey)) {
			rightClick();
			Sleep(delay);
		}
	}
    	return 0;
}
