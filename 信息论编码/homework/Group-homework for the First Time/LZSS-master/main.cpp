#include "utility.hpp"
#include "coding.hpp"
#include "decoding.hpp"

#define ID_CODING_BUTTON 500
#define ID_DECODING_BUTTON 501
#define ID_FILE_OPEN_BUTTON 502



HWND g_Checkbox;
HWND g_DataPathTextBox;
HWND g_CodedOutputPathTextBox;
HWND g_DecodedOutputPathTextBox;
HWND g_DictionarySizeTextBox;
HWND g_BufforSizeTextBox;
HWND g_ProgressBar;


using namespace std;

void SetDataPath(HWND hwnd)
{
	OPENFILENAME ofn;
	char fileName[ MAX_PATH ] = ".\data.txt";
				   
	ZeroMemory( & ofn, sizeof( ofn ) );
	ofn.lStructSize = sizeof( ofn );
	ofn.lpstrFilter = "Text files (*.txt)\0*.txt\0Compressed filles (*.lzsstk)\0*.lzsstk\0All files\0*.*\0";
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = fileName;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	
	
	
	if(GetOpenFileName( & ofn )) SetWindowText(g_DataPathTextBox,fileName);
}


/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND: 
		{
			switch (wParam)
			{
				case ID_CODING_BUTTON: {
					//coding
					//TODO dialog box
					if(Coding(
					hwnd,
					g_ProgressBar,
					g_DataPathTextBox,
					g_BufforSizeTextBox,
					g_DictionarySizeTextBox,
					g_CodedOutputPathTextBox)) MessageBox( hwnd, "Coding process succes!", "Result Window", MB_ICONINFORMATION );
					break;
				}
				case ID_DECODING_BUTTON:
				{
					//TODO dialog box
					//decoding
					if(Decoding(hwnd,
					g_ProgressBar,
					g_DataPathTextBox,
					g_BufforSizeTextBox,
					g_DictionarySizeTextBox,
					g_DecodedOutputPathTextBox)) MessageBox( hwnd, "Decoding procces succes!", "Result Window", MB_ICONINFORMATION );
					break;
				}
				case ID_FILE_OPEN_BUTTON:
				{
					SetDataPath(hwnd);
					break;
				}
				case ID_CHECKBOX:
				{
					BOOL bChecked =( IsDlgButtonChecked( hwnd, ID_CHECKBOX ) == BST_CHECKED );
					if(bChecked)
					{
						CheckDlgButton( hwnd, ID_CHECKBOX, BST_UNCHECKED );
					}
					else
					{
						CheckDlgButton( hwnd, ID_CHECKBOX, BST_CHECKED );
					}
					break;
				}
				
				default:
				{
					MessageBox( hwnd, "Default", "Test", MB_ICONINFORMATION );
					//something else
					break;
				}
			}
			break;
		}
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

void CreateGUI(HWND hwnd,HINSTANCE hInstance)
{

	//Button properites
	int buttonHorizontal = 10;
	
	int buttonWidth = 180;
	int buttonHeight = 30;
	
	//Text Box path properties
	int textBoxHorizontalPath = 130;
	int textBoxHeightPath = 20;
	int textBoxWidthPath = 250;
	
	//Labels path properties
	int labelHorizontalPath = 10;
	int labelWidthPath = 110;
	
	//Progres Bar
	g_ProgressBar = CreateWindowEx( 0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH ,
	10,280, 600, 30, hwnd,( HMENU ) 200, hInstance, NULL );
	//Range!
	SendMessage( g_ProgressBar, PBM_SETRANGE, 0,( LPARAM ) MAKELONG( 0, 100 ) );
	
	
	//Creating Buttons
	CreateWindowEx(0,"BUTTON","Coding",WS_CHILD | WS_VISIBLE | WS_BORDER,buttonHorizontal,80,buttonWidth,buttonHeight, hwnd,(HMENU)ID_CODING_BUTTON,hInstance,NULL);
	CreateWindowEx(0,"BUTTON","Decoding",WS_CHILD | WS_VISIBLE | WS_BORDER,buttonHorizontal,120,buttonWidth,buttonHeight, hwnd,(HMENU)ID_DECODING_BUTTON,hInstance,NULL);
	CreateWindowEx(0,"BUTTON","Data File",WS_CHILD | WS_VISIBLE | WS_BORDER,buttonHorizontal,160,buttonWidth,buttonHeight, hwnd,(HMENU)ID_FILE_OPEN_BUTTON,hInstance,NULL);
	
	HWND lzssPropertiesGroupBox = CreateWindowEx(0,"BUTTON","Properties",WS_CHILD | WS_VISIBLE | BS_GROUPBOX,200,80,400,150, hwnd,(HMENU)ID_CODING_BUTTON,hInstance,NULL);

	HWND lzssDataPath = CreateWindowEx(0,"BUTTON","Data path",WS_CHILD | WS_VISIBLE | BS_GROUPBOX,10,10,600,50, hwnd,(HMENU)ID_CODING_BUTTON,hInstance,NULL);
	//Data GUI
	g_DataPathTextBox = CreateWindowEx(0,"EDIT","",WS_CHILD | WS_VISIBLE ,10,20,580,20, lzssDataPath,NULL,hInstance,NULL);
	
	g_Checkbox= CreateWindowEx( 0, "BUTTON", "Generate log", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,labelHorizontalPath, 200, 105, 30, hwnd, (HMENU)ID_CHECKBOX, hInstance, NULL );
	
	//Code GUI
	g_CodedOutputPathTextBox = CreateWindowEx(0,"EDIT","codeOutput",WS_CHILD | WS_VISIBLE ,textBoxHorizontalPath,20,textBoxWidthPath,textBoxHeightPath, lzssPropertiesGroupBox,NULL,hInstance,NULL);
	CreateWindowEx( 0, "STATIC", "Code file name:", WS_CHILD | WS_VISIBLE | SS_RIGHT, labelHorizontalPath, 20, labelWidthPath, 20, lzssPropertiesGroupBox, NULL, hInstance, NULL );
	
	//Decode GUI
	g_DecodedOutputPathTextBox = CreateWindowEx(0,"EDIT","decodedOutput.txt",WS_CHILD | WS_VISIBLE ,textBoxHorizontalPath,50,textBoxWidthPath,textBoxHeightPath, lzssPropertiesGroupBox,NULL,hInstance,NULL);
	CreateWindowEx( 0, "STATIC", "Decod file name:", WS_CHILD | WS_VISIBLE | SS_RIGHT, labelHorizontalPath, 50, labelWidthPath, 20, lzssPropertiesGroupBox, NULL, hInstance, NULL );

	
	//Dictionary
	g_DictionarySizeTextBox = CreateWindowEx(0,"EDIT","16",WS_CHILD | WS_VISIBLE ,textBoxHorizontalPath,80,textBoxWidthPath,textBoxHeightPath, lzssPropertiesGroupBox,NULL,hInstance,NULL);
	CreateWindowEx( 0, "STATIC", "Dictionary Size:", WS_CHILD | WS_VISIBLE | SS_RIGHT, labelHorizontalPath, 80, labelWidthPath, 20, lzssPropertiesGroupBox, NULL, hInstance, NULL );
	
	//Buffor
	g_BufforSizeTextBox = CreateWindowEx(0,"EDIT","8",WS_CHILD | WS_VISIBLE ,textBoxHorizontalPath,110,textBoxWidthPath,textBoxHeightPath, lzssPropertiesGroupBox,NULL,hInstance,NULL);
	CreateWindowEx( 0, "STATIC", "Buffor Size:", WS_CHILD | WS_VISIBLE | SS_RIGHT, labelHorizontalPath, 110, labelWidthPath, 20, lzssPropertiesGroupBox, NULL, hInstance, NULL );
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof( INITCOMMONCONTROLSEX );
	icc.dwICC = ICC_BAR_CLASSES; // toolbars, statusbars, tooltips, progressbars
	InitCommonControlsEx( & icc );

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","LZSS",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		350, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	
	CreateGUI(hwnd,hInstance);
	
	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
