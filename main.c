//WINAPI
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>

//POSIX
#include <stdio.h>
#include <stdlib.h>

#define MAINWND_STATIC_WIDTH 200
#define MAINWND_CB_WIDTH MAINWND_STATIC_WIDTH
#define MAINWND_BUTTON_WIDTH 80

#define LINE_HEIGHT 20

#define MAINWND_STATIC_HEIGHT LINE_HEIGHT
#define MAINWND_CB_HEIGHT (LINE_HEIGHT*4)
#define MAINWND_BUTTON_HEIGHT LINE_HEIGHT

#define MAINWND_STATIC_X 20
#define MAINWND_CB_X MAINWND_STATIC_X
#define MAINWND_BUTTON_X (MAINWND_STATIC_X+MAINWND_CB_WIDTH+10)

#define MAINWND_STATIC_Y 20
#define MAINWND_CB_Y (MAINWND_STATIC_Y+MAINWND_STATIC_HEIGHT+10)
#define MAINWND_BUTTON_Y MAINWND_CB_Y

#define MAINWND_WIDTH (MAINWND_BUTTON_X+MAINWND_BUTTON_WIDTH+20)
#define MAINWND_HEIGHT (MAINWND_CB_Y+MAINWND_CB_HEIGHT+20)

#define MAINWND_STATIC_ID ((HMENU)1)
#define MAINWND_CB_ID ((HMENU)2)
#define MAINWND_BUTTON_ID ((HMENU)3)

#define CACHE_NAME "addrs_cache"

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic;
	static HWND hComboBox;
	static HWND hButton;
	static HINSTANCE hInstance;

	switch(Msg)
	{
		case WM_COMMAND:
			{
				if(LOWORD(wParam) == (size_t)MAINWND_BUTTON_ID && HIWORD(wParam) == BN_CLICKED)
				{
					size_t addr_len = ComboBox_GetTextLength(hComboBox);
					char* addr = malloc(addr_len+1);ComboBox_GetText(hComboBox,addr,addr_len+1);

					ssize_t start_index = -1;

					while((start_index = ComboBox_FindString(hComboBox,start_index,addr)) != CB_ERR)
					{
						size_t cur_index = start_index;
						if(ComboBox_GetLBTextLen(hComboBox,cur_index) == addr_len)
						{
							break;
						}
					}

					if(start_index == CB_ERR)
					{
						ComboBox_AddString(hComboBox,addr);

						FILE* stream = fopen(CACHE_NAME,"r+");

						size_t naddrs;
						fread(&naddrs,sizeof(size_t),1,stream);
						naddrs++;
						rewind(stream);
						fwrite(&naddrs,sizeof(size_t),1,stream);

						fseek(stream,0,SEEK_END);
						fwrite(&addr_len,sizeof(size_t),1,stream);
						fwrite(addr,sizeof(char),addr_len+1,stream);

						fclose(stream);
					}

					free(addr);

					//
				}
			}
			break;
		case WM_CREATE:
			hInstance = ((CREATESTRUCT*)lParam)->hInstance;

			hStatic = CreateWindowEx(
					0,
					WC_STATIC,
					"Ingrese el servidor:",
					WS_CHILD|WS_VISIBLE|SS_CENTER,
					MAINWND_STATIC_X,MAINWND_STATIC_Y,MAINWND_STATIC_WIDTH,MAINWND_STATIC_HEIGHT,
					hWnd,
					MAINWND_STATIC_ID,
					hInstance,
					NULL
				);

			hComboBox = CreateWindowEx(
					0,
					WC_COMBOBOX,
					"",
					WS_VISIBLE|WS_CHILD|WS_HSCROLL|WS_VSCROLL|CBS_HASSTRINGS|CBS_DROPDOWN|CBS_AUTOHSCROLL,
					MAINWND_CB_X,MAINWND_CB_Y,MAINWND_CB_WIDTH,MAINWND_CB_HEIGHT,
					hWnd,
					MAINWND_CB_ID,
					hInstance,
					NULL
				);

			hButton = CreateWindowEx(
					0,
					WC_BUTTON,
					"Continuar",
					WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON|BS_CENTER,
					MAINWND_BUTTON_X,MAINWND_BUTTON_Y,MAINWND_BUTTON_WIDTH,MAINWND_BUTTON_HEIGHT,
					hWnd,
					MAINWND_BUTTON_ID,
					hInstance,
					NULL
				);

			{
				FILE* stream = fopen(CACHE_NAME,"r");

				if(stream == NULL)
				{
					FILE* stream = fopen(CACHE_NAME,"w");
					size_t naddrs = 0;
					fwrite(&naddrs,sizeof(size_t),1,stream);
					fclose(stream);
					break;
				}

				size_t naddrs;
				fread(&naddrs,sizeof(size_t),1,stream);

				size_t cur_addr_len;
				char* cur_addr;

				while(naddrs != 0)
				{
					fread(&cur_addr_len,sizeof(size_t),1,stream);

					cur_addr = malloc(cur_addr_len+1);

					fread(cur_addr,sizeof(char),cur_addr_len+1,stream);

					ComboBox_AddString(hComboBox,cur_addr);

					free(cur_addr);
					naddrs--;
				}
				fclose(stream);
			}

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd,Msg,wParam,lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hCurInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iShowCmd)
{
	WNDCLASSEX WndClass;
	HWND hWnd;
	MSG Msg;

	WndClass = (WNDCLASSEX){
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW|CS_VREDRAW,
		.lpfnWndProc = MainWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hCurInstance,
		.hIcon = LoadIcon(NULL,IDI_APPLICATION),
		.hCursor = LoadCursor(NULL,IDC_ARROW),
		.hbrBackground = GetSysColorBrush(COLOR_WINDOW),
		.lpszMenuName=NULL,
		.lpszClassName="MAINWND",
		.hIconSm = LoadIcon(NULL,IDI_APPLICATION)
		};

	RegisterClassEx(&WndClass);

	hWnd = CreateWindowEx(
			0,
			"MAINWND",
			"Isa-Chat",
			WS_VISIBLE|WS_POPUPWINDOW|WS_CAPTION,
			100,200,MAINWND_WIDTH,MAINWND_HEIGHT,
			HWND_DESKTOP,
			NULL,
			hCurInstance,
			NULL
		);

	while(GetMessage(&Msg,NULL,0,0) == TRUE)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return 0;
}
