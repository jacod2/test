// STUDYAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "STUDYAPI.h"

#define MAX_LOADSTRING 100

typedef struct _tagRectangle
{
    float l, t, r, b;
}RECTANGLE, *PRECTANGLE;
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;
HDC g_hDC;
bool g_bloop = true;
RECTANGLE    g_tPlayerRC = { 100,100 ,200,200};

//시간을 구하기 위한 변수들

LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float delta_time;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Run();
struct _tagArea
{
    bool bStart;
    POINT ptStart;
    POINT ptEnd;
};

_tagArea g_tArea;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STUDYAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    g_hDC = GetDC(g_hWnd);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STUDYAPI));

    MSG msg;

    QueryPerformanceFrequency(&g_tSecond);
    QueryPerformanceCounter(&g_tTime);

    // 기본 메시지 루프입니다:
    while (g_bloop)
    {
        //peek메세지는 메시지가 메세지 큐에 없어도 바로 빠져
        // 나온다. 메시지가 있을경우 true, 없을 경우 false;
        // 가된다. 메세지가 없는 시간이 윈도ㅜ의 데드타임이다.

        if (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // 윈도우 데드타임일 경우
        else
        {
            Run();
        }
    }

    ReleaseDC(g_hWnd, g_hDC);
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STUDYAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_STUDYAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
   //실제 윈도우 타이틀바나 메뉴를 포함한 윈도의 크기를
   //구해준다

   RECT rc = { 0,0,800,600 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
   //위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를
   // 원하는 크기로 맞춰줘야 한다.
   // TOPMOST 나중에 나오는창이 먼저나오는창을 덮어버림 제일 최상단에 띄우겠다
   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left,
       rc.bottom - rc.top, SWP_NOMOVE| SWP_NOZORDER);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            //TextOut(hdc, 50, 50, L"Hello World", 12);
            //TCHAR strMouse[64] = {  };
            ////wsprintf : 유니코드 문자열을 만들어주는 함수이다 %d 에는 정수가 대입된다.

            //wsprintf(strMouse, TEXT("x : %d y %d"), g_tArea.ptStart.x,g_tArea.ptStart.y);
            ////lstrlen : 유니코드 문자열의 길이를 알려주는 메시지이다

            //TextOut(hdc, 600, 30, strMouse, lstrlen(strMouse));
            //
            //if (g_tArea.bStart)
            //{
            //    Rectangle(hdc, g_tArea.ptStart.x, g_tArea.ptStart.y
            //        , g_tArea.ptEnd.x, g_tArea.ptEnd.y);
            //}
            EndPaint(hWnd, &ps);
        }
        break;
        //마우스 왼쪽버튼을 눌렀을때 들어온다
    //case WM_LBUTTONDOWN://왼쪽버튼 클릭을 의미 다운은 누를때 
    //    //마우스 위치는 lParam 에 들어오게되는 16비트로 쪼개서 x,y 값이 
    //    //32비트 변수에 들어오게된다
    //    //LOWRD,HIWORD 매크로를 이용해서 상하위 16비트의 값을 얻어올수있다.
    //    if (!g_tArea.bStart)
    //    {
    //        g_tArea.bStart = true;
    //        g_tArea.ptStart.x = lParam & 0x0000ffff;
    //        g_tArea.ptStart.y = lParam >> 16;
    //        //이함수는 강제로 WM_PAINT메시지를 호츨해준다
    //        //1번인자는 윈도우 핸들 2번인자는 초기화할 영역
    //        //널을 넣어준 경우 전체 화면을 대사응로 갱싱
    //        //3번인자는 참일경우 현재 화면을 지우고 갱신
    //        InvalidateRect(hWnd, NULL, TRUE);
    //    }
    //case WM_MOUSEMOVE://마우스가 움질일떄 들어오는 메시지
    //    if (g_tArea.bStart)
    //    {
    //        g_tArea.ptEnd.x = lParam & 0x0000ffff;
    //        g_tArea.ptEnd.y = lParam >> 16;
    //        InvalidateRect(hWnd, NULL, TRUE);
    //    }
    //    break;
    //case WM_LBUTTONUP:
    //    if (g_tArea.bStart)
    //    {
    //        g_tArea.bStart = false;
    //        g_tArea.ptStart.x = lParam & 0x0000ffff;
    //        g_tArea.ptStart.y = lParam >> 16;
    //        //이함수는 강제로 WM_PAINT메시지를 호츨해준다
    //        //1번인자는 윈도우 핸들 2번인자는 초기화할 영역
    //        //널을 넣어준 경우 전체 화면을 대사응로 갱싱
    //        //3번인자는 참일경우 현재 화면을 지우고 갱신
    //        InvalidateRect(hWnd, NULL, TRUE);
    //    }
    //    break;
    case WM_KEYDOWN://이메시지가 들어올경우 WPARAM에 어떤키가 들어왔는지가 들아옴
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
        }
    }
    break;
    case WM_DESTROY:
        g_bloop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Run()
{

    LARGE_INTEGER tTime;
    QueryPerformanceCounter(&tTime);
    delta_time = (tTime.QuadPart - g_tTime.QuadPart) /
        (float)g_tSecond.QuadPart;
    g_tTime = tTime;


    //플레이어 초당 이동속도 : 300
    float fSPeed = 300 * delta_time;

    RECT rcWindow;
    //창의 정보를 읽어와서 설정해줄수 있음
    GetClientRect(g_hWnd, &rcWindow);
   
    if (GetAsyncKeyState('D') & 0x8000 && g_tPlayerRC.r < rcWindow.right)
    {
        g_tPlayerRC.l += fSPeed;
        g_tPlayerRC.r += fSPeed;
    }
    if (GetAsyncKeyState('A') & 0x8000 && g_tPlayerRC.l > rcWindow.left)
    {
        g_tPlayerRC.l -= fSPeed;
        g_tPlayerRC.r -= fSPeed;
    }
    if (GetAsyncKeyState('W') & 0x8000 && g_tPlayerRC.t > rcWindow.top)
    {
        g_tPlayerRC.t -= fSPeed;
        g_tPlayerRC.b -= fSPeed;
    }
    if (GetAsyncKeyState('S') & 0x8000 && g_tPlayerRC.b < rcWindow.bottom)
    {
        g_tPlayerRC.t += fSPeed;
        g_tPlayerRC.b += fSPeed;
    }
    //Rectangle(g_hDC, rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom);
    Rectangle(g_hDC, g_tPlayerRC.l, g_tPlayerRC.t, g_tPlayerRC.r,
        g_tPlayerRC.b);
}