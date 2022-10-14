//WINAPI
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>

//POSIX
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define NAME_MAXLEN 20
typedef struct
{
	char str[NAME_MAXLEN+1];
	unsigned char len;
}
Name_t;

#define LINE_HEIGHT 20

#define GETNAMEWND_EDIT_WIDTH 100
#define GETNAMEWND_BUTTON_WIDTH 50
#define GETNAMEWND_STATIC_WIDTH (GETNAMEWND_EDIT_WIDTH+GETNAMEWND_BUTTON_WIDTH+20)

#define GETNAMEWND_STATIC_HEIGHT LINE_HEIGHT
#define GETNAMEWND_EDIT_HEIGHT LINE_HEIGHT
#define GETNAMEWND_BUTTON_HEIGHT LINE_HEIGHT

#define GETNAMEWND_STATIC_X 0
#define GETNAMEWND_EDIT_X 10
#define GETNAMEWND_BUTTON_X (GETNAMEWND_EDIT_X+GETNAMEWND_EDIT_WIDTH)

#define GETNAMEWND_STATIC_Y 0
#define GETNAMEWND_EDIT_Y LINE_HEIGHT
#define GETNAMEWND_BUTTON_Y GETNAMEWND_EDIT_Y

#define GETNAMEWND_STATIC_ID ((HMENU)1)
#define GETNAMEWND_EDIT_ID ((HMENU)2)
#define GETNAMEWND_BUTTON_ID ((HMENU)3)

#define GETNAMEWND_X 100
#define GETNAMEWND_Y 200
#define GETNAMEWND_WIDTH GETNAMEWND_STATIC_WIDTH
#define GETNAMEWND_HEIGHT (LINE_HEIGHT*3)

LRESULT CALLBACK GetNameWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic;
	static HWND hEdit;
	static HWND hButton;
	static HINSTANCE hInstance;
	static Name_t*name;
	switch(Msg)
	{
		case WM_COMMAND:
			if(LOWORD(wParam) == GETNAMEWND_BUTTON_ID)
			{
				if(Edit_GetTextLength(hEdit) < 5)
				{
					MessageBox(hWnd,"Cadena cuya longitud es menor a 5 caracteres.","ERROR",MB_OK|MB_DEFBUTTON1|MB_SYSTEMMODAL|MB_ICONERROR);
				}
				else
				{
					Edit_GetText(hEdit,name->str,NAME_MAXLEN+1);
					name->len = Edit_GetTextLength(hEdit);
					PostQuitMessage(0);
				}
			}
			break;
		case WM_CREATE:
			hInstance = ((CREATESTRUCT*)lParam)->hInstance;
			name = ((CREATESTRUCT*)lParam)->lpCreateParams;

			hStatic = CreateWindowEx(
					0,
					WC_STATIC,
					"Ingrese el nombre: ",
					WS_VISIBLE|WS_CHILD|SS_CENTER,
					GETNAMEWND_STATIC_X,
					GETNAMEWND_STATIC_Y,
					GETNAMEWND_STATIC_WIDTH,
					GETNAMEWND_STATIC_HEIGHT,
					hWnd,
					GETNAMEWND_STATIC_ID,
					hInstance,
					NULL
				);

			hEdit = CreateWindowEx(
					0,
					WC_EDIT,
					"",
					WS_VISIBLE|WS_CHILD|WS_BORDER|ES_LEFT|ES_LOWERCASE,
					GETNAMEWND_EDIT_X,
					GETNAMEWND_EDIT_Y,
					GETNAMEWND_EDIT_WIDTH,
					GETNAMEWND_EDIT_HEIGHT,
					hWnd,
					GETNAMEWND_EDIT_ID,
					hInstance,
					NULL
				);

			hButton = CreateWindowEx(
						0,
						WC_BUTTON,
						"Seguir",
						WS_VISIBLE|WS_CHILD|BS_CENTER|BS_PUSHBUTTON,
						GETNAMEWND_BUTTON_X,
						GETNAMEWND_BUTTON_Y,
						GETNAMEWND_BUTTON_WIDTH,
						GETNAMEWND_BUTTON_HEIGHT,
						hWnd,
						GETNAMEWND_BUTTON_ID,
						hInstance,
						NULL
					);
			break;
		default:
			return DefWindowProc(hWnd,Msg,wParam,lParam);
	}
	return 0;
}

Name_t GetName(HINSTANCE hCurInstance)
{
	Name_t name = {};
	HWND hWnd;
	MSG Msg;


	hWnd = CreateWindowEx(
				0,
				"GETNAMEWND",
				"",
				WS_VISIBLE|WS_POPUPWINDOW|WS_BORDER,
				GETNAMEWND_X,
				GETNAMEWND_Y,
				GETNAMEWND_WIDTH,
				GETNAMEWND_HEIGHT,
				NULL,
				NULL,
				hCurInstance,
				&name
			);

	while(GetMessage(&Msg,NULL,0,0) == TRUE)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	DestroyWindow(hWnd);

	return name;
}

#define CHATWND_X CW_USEDEFAULT
#define CHATWND_Y CW_USEDEFAULT
#define CHATWND_WIDTH CW_USEDEFAULT
#define CHATWND_HEIGHT CW_USEDEFAULT

RECT GetWndRect(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd,&Rect);
	return Rect;
}

/*
LONG GetWndWidth(HWND hWnd)
{
	RECT rect = GetWndRect(hWnd);
	return rect.right-rect.left;
}
*/

LONG GetWndHeight(HWND hWnd)
{
	RECT rect = GetWndRect(hWnd);
	return rect.bottom-rect.top;
}

#define CHATWND_STATIC_WIDTH 530
#define CHATWND_LB_WIDTH CHATWND_STATIC_WIDTH
#define CHATWND_EDIT_WRMSG_WIDTH 610
#define CHATWND_BUTTON_WIDTH 80
#define CHATWND_EDIT_RDONLY_WIDTH (CHATWND_EDIT_WRMSG_WIDTH+10+CHATWND_BUTTON_WIDTH)

#define CHATWND_EDIT_WRMSG_HEIGHT (LINE_HEIGHT*3)
#define CHATWND_EDIT_RDONLY_HEIGHT (GetWndHeight(hWnd)-CHATWND_EDIT_WRMSG_HEIGHT-20)
#define CHATWND_BUTTON_HEIGHT CHATWND_EDIT_WRMSG_HEIGHT
#define CHATWND_STATIC_HEIGHT LINE_HEIGHT
#define CHATWND_LB_HEIGHT (CHATWND_EDIT_RDONLY_HEIGHT+CHATWND_EDIT_WRMSG_HEIGHT-CHATWND_STATIC_HEIGHT)

#define CHATWND_EDIT_RDONLY_X 20
#define CHATWND_EDIT_WRMSG_X 20
#define CHATWND_BUTTON_X (CHATWND_EDIT_WRMSG_X+CHATWND_EDIT_WRMSG_WIDTH+10)
#define CHATWND_STATIC_X (CHATWND_EDIT_RDONLY_X+CHATWND_EDIT_RDONLY_WIDTH+30)
#define CHATWND_LB_X CHATWND_STATIC_X

#define CHATWND_EDIT_RDONLY_Y 0
#define CHATWND_EDIT_WRMSG_Y (CHATWND_EDIT_RDONLY_Y+CHATWND_EDIT_RDONLY_HEIGHT)
#define CHATWND_BUTTON_Y CHATWND_EDIT_WRMSG_Y
#define CHATWND_STATIC_Y  CHATWND_EDIT_RDONLY_Y
#define CHATWND_LB_Y (CHATWND_STATIC_Y+CHATWND_STATIC_HEIGHT)

#define CHATWND_STATIC_ID ((HMENU)1)
#define CHATWND_LB_ID ((HMENU)2)
#define CHATWND_EDIT_RDONLY_ID ((HMENU)3)
#define CHATWND_EDIT_WRMSG_ID ((HMENU)4)
#define CHATWND_BUTTON_ID ((HMENU)5)

LRESULT CALLBACK ChatWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditRDOnly;
	static HWND hEditWRMsg;
	static HWND hButton;
	static HWND hStatic;
	static HWND hLB;

	static int sockfd;
	static HINSTANCE hInstance;

	switch(Msg)
	{
		case WM_CREATE:
			sockfd = ((CREATESTRUCT*)lParam)->lpCreateParams;
			hInstance = ((CREATESTRUCT*)lParam)->hInstance;

			hEditRDOnly = CreateWindowEx(
					0,
					WC_EDIT,
					"",
					WS_CHILD|WS_VISIBLE|WS_BORDER|WS_THICKFRAME|ES_LEFT|ES_READONLY|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL,
					CHATWND_EDIT_RDONLY_X,
					CHATWND_EDIT_RDONLY_Y,
					CHATWND_EDIT_RDONLY_WIDTH,
					CHATWND_EDIT_RDONLY_HEIGHT,
					hWnd,
					CHATWND_EDIT_RDONLY_ID,
					hInstance,
					NULL
				);
			hEditWRMsg = CreateWindowEx(
					0,
					WC_EDIT,
					"",
					WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|ES_LEFT,
					CHATWND_EDIT_WRMSG_X,
					CHATWND_EDIT_WRMSG_Y,
					CHATWND_EDIT_WRMSG_WIDTH,
					CHATWND_EDIT_WRMSG_HEIGHT,
					hWnd,
					CHATWND_EDIT_WRMSG_ID,
					hInstance,
					NULL
				);

			hButton = CreateWindowEx(
					0,
					WC_BUTTON,
					"Enviar",
					WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_CENTER|BS_VCENTER,
					CHATWND_BUTTON_X,
					CHATWND_BUTTON_Y,
					CHATWND_BUTTON_WIDTH,
					CHATWND_BUTTON_HEIGHT,
					hWnd,
					CHATWND_BUTTON_ID,
					hInstance,
					NULL
				);

			hStatic = CreateWindowEx(
					0,
					WC_STATIC,
					"Lista de espera:",
					WS_CHILD|WS_VISIBLE|SS_CENTER,
					CHATWND_STATIC_X,
					CHATWND_STATIC_Y,
					CHATWND_STATIC_WIDTH,
					CHATWND_STATIC_HEIGHT,
					hWnd,
					CHATWND_STATIC_ID,
					hInstance,
					NULL
				);

			hLB = CreateWindowEx(
					0,
					WC_LISTBOX,
					"",
					WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_STANDARD,
					CHATWND_LB_X,
					CHATWND_LB_Y,
					CHATWND_LB_HEIGHT,
					CHATWND_LB_WIDTH,
					hWnd,
					CHATWND_LB_ID,
					hInstance,
					NULL
				);
			break;
		case WM_DESTROY:
			//close(sockfd);
			PostQuitMessage(0);
			return DefWindowProc(hWnd,Msg,wParam,lParam);
		default:
			return DefWindowProc(hWnd,Msg,wParam,lParam);
	}
	return 0;
}

void StartChat(int sockfd, const char* server_name, HINSTANCE hCurInstance)
{
	HWND hWnd;
	MSG Msg;

	hWnd = CreateWindowEx(
			0,
			"CHATWND",
			server_name,
			WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_MAXIMIZE,
			CHATWND_X,
			CHATWND_Y,
			CHATWND_WIDTH,
			CHATWND_HEIGHT,
			HWND_DESKTOP,
			NULL,
			hCurInstance,
			sockfd
		);

	printf("%zu,%zu",CHATWND_EDIT_RDONLY_HEIGHT);

	while(GetMessage(&Msg,NULL,0,0) == TRUE)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

#define MAINWND_STATIC_WIDTH 200
#define MAINWND_CB_WIDTH MAINWND_STATIC_WIDTH
#define MAINWND_BUTTON_WIDTH 80

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

//Progress bar that is only shown after the user has given the username and the server address.
#define MAINWND_PB_WIDTH ((MAINWND_BUTTON_X+MAINWND_BUTTON_WIDTH)-MAINWND_CB_X)
#define MAINWND_PB_HEIGHT (LINE_HEIGHT+10)
#define MAINWND_PB_X MAINWND_CB_X
#define MAINWND_PB_Y MAINWND_CB_Y
#define MAINWND_PB_ID ((HMENU)4)
//Steps values
#define STEP1_CREATESOCK 1
#define STEP2_CONNECT 5
#define STEP3_LOGGEDIN 6

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic;
	static HWND hComboBox;
	static HWND hButton;
	static HWND hPB;//progress bar
	static HINSTANCE hInstance;

	switch(Msg)
	{
		case WM_COMMAND:
			{
				if(LOWORD(wParam) == (size_t)MAINWND_BUTTON_ID && HIWORD(wParam) == BN_CLICKED)
				{
					size_t addr_len = ComboBox_GetTextLength(hComboBox);
					char* addr = malloc(addr_len+1);ComboBox_GetText(hComboBox,addr,addr_len+1);

					bool addr_found = false;
					ssize_t start_index = ComboBox_FindString(hComboBox,-1,addr);

					if(start_index != CB_ERR)
					{
						size_t cur_index = start_index;
						do
						{
							if(ComboBox_GetLBTextLen(hComboBox,cur_index) == addr_len)
							{
								addr_found = true;
								break;
							}
							cur_index = ComboBox_FindString(hComboBox,cur_index,addr);
						}
						while(cur_index != start_index);
					}


					if(!addr_found)
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

					//Use the GetNameWnd to get the username
					ShowWindow(hWnd,SW_HIDE);//This shall hide the main wnd so the GetNameWnd is shown
					Name_t name = GetName(hInstance);
					ShowWindow(hWnd,SW_RESTORE);

					//Connect to the server
					int sockfd;
					ShowWindow(hButton,SW_HIDE);//Hide and then restore the ctrl so the progress bar is shown
					ShowWindow(hComboBox,SW_HIDE);
					{
						ShowWindow(hPB,SW_NORMAL);

						//Create socket
						Static_SetText(hStatic,"Creando socket.");
						Sleep(2000);
						
						SendMessage(hPB,PBM_DELTAPOS,STEP1_CREATESOCK,0);

						//Connect
						Static_SetText(hStatic,"Connectando.");
						Sleep(2000);
						
						SendMessage(hPB,PBM_DELTAPOS,STEP2_CONNECT,0);

						//Log in
						Static_SetText(hStatic,"Ingresando.");
						Sleep(2000);
						
						SendMessage(hPB,PBM_DELTAPOS,STEP3_LOGGEDIN,0);

						Sleep(2000);

						ShowWindow(hPB,SW_HIDE);
						SendMessage(hPB,PBM_SETPOS,0,0);
					}
					Static_SetText(hStatic,"Ingrese el servidor:");//Restore the original text of the static
					ShowWindow(hButton,SW_RESTORE);
					ShowWindow(hComboBox,SW_RESTORE);

					//Show ChatWnd
					ShowWindow(hWnd,SW_HIDE);
					StartChat(sockfd,addr,hInstance);
					ShowWindow(hWnd,SW_RESTORE);

					free(addr);
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

			hPB = CreateWindowEx(
					0,
					PROGRESS_CLASS,
					"",
					WS_CHILD|WS_BORDER,
					MAINWND_PB_X,
					MAINWND_PB_Y,
					MAINWND_PB_WIDTH,
					MAINWND_PB_HEIGHT,
					hWnd,
					MAINWND_PB_ID,
					hInstance,
					NULL
				);
			SendMessage(hPB,PBM_SETRANGE,0,MAKELPARAM(0,STEP1_CREATESOCK+STEP2_CONNECT+STEP3_LOGGEDIN));

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

	/*REGISTER WND CLASSES*/

	//MainWnd
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

	//GetNameWnd
	WndClass = (WNDCLASSEX){
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW|CS_VREDRAW,
		.lpfnWndProc = GetNameWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hCurInstance,
		.hIcon = LoadIcon(NULL,IDI_APPLICATION),
		.hCursor = LoadCursor(NULL,IDC_ARROW),
		.hbrBackground = GetSysColorBrush(COLOR_WINDOW),
		.lpszMenuName=NULL,
		.lpszClassName="GETNAMEWND",
		.hIconSm = LoadIcon(NULL,IDI_APPLICATION)
		};
	RegisterClassEx(&WndClass);

	//ChatWnd
	WndClass = (WNDCLASSEX){
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW|CS_VREDRAW,
		.lpfnWndProc = ChatWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hCurInstance,
		.hIcon = LoadIcon(NULL,IDI_APPLICATION),
		.hCursor = LoadCursor(NULL,IDC_ARROW),
		.hbrBackground = GetSysColorBrush(COLOR_WINDOW),
		.lpszMenuName=NULL,
		.lpszClassName="CHATWND",
		.hIconSm = LoadIcon(NULL,IDI_APPLICATION)
		};
	RegisterClassEx(&WndClass);

	/*REGISTER WND CLASSES*/

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
