/********************************************************
*    Author: Tianhao Wang
*    E-mail: wanglingdebian@gmail.com   
*    Date:   2015-03-05   
*    Desc:   一个足够简单的画线程序，没有任何额外功能
*********************************************************/   

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

LPDIRECT3D9 g_pd3dObject       = 0;
LPDIRECT3DDEVICE9 g_pd3dDevice = 0;
D3DXVECTOR2* g_pLineArr        = 0;
// 记录ID3DLine对象所需的顶点坐标
ID3DXLine* g_pLine             = 0;

// 初始化Direct3D
void InitD3D(HWND hwnd)
{
    // 第一步，创建一个IDirect3D9对象
    if (FAILED(g_pd3dObject = Direct3DCreate9(D3D_SDK_VERSION))) {
        MessageBox(0, "Direct3DCreate9 FAILED", 0, 0);
        PostQuitMessage(0);
    }

    // 第二步，创建并填写一个D3DPRESENT_PARAMETERS结构体
    // 该结构体将用于创建IDirect3DDevice对象
    D3DPRESENT_PARAMETERS d3dPP;
    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.BackBufferCount    = 1;
    d3dPP.BackBufferFormat   = D3DFMT_UNKNOWN;
    d3dPP.MultiSampleType    = D3DMULTISAMPLE_NONE;
    d3dPP.MultiSampleQuality = 0;
    d3dPP.SwapEffect         = D3DSWAPEFFECT_DISCARD;
    d3dPP.hDeviceWindow      = hwnd;
    d3dPP.Windowed           = true;

    // 第三步，创建IDirect3DDevice对象
    if (FAILED(g_pd3dObject->CreateDevice(D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          hwnd,
                                          D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                          &d3dPP,
                                          &g_pd3dDevice))) {
        MessageBox(0, "CreateDevice FAILED", 0, 0);
        PostQuitMessage(0);
    }

    // 创建ID3DLine对象
    if (FAILED(D3DXCreateLine(g_pd3dDevice, &g_pLine))) {
        MessageBox(0, "D3DXCreateLine FAILED", 0, 0);
        PostQuitMessage(0);
    }

    // 填充定点坐标
    g_pLineArr = new D3DXVECTOR2[4];
    g_pLineArr[0].x = g_pLineArr[0].y = 100;
    g_pLineArr[1].x = g_pLineArr[1].y = 400;
    g_pLineArr[2].x = 400; g_pLineArr[2].y = 100;
    g_pLineArr[3].x = 100; g_pLineArr[3].y = 400;
}

// 回调函数，负责消息处理
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 绘制图形
void DrawScene()
{
    // 清理屏幕
    g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // 准备绘制
    if (FAILED(g_pd3dDevice->BeginScene())) {
        MessageBox(0, "BeginScene FAILED", 0, 0);
        PostQuitMessage(0);
    }

    // 开始绘制
    g_pLine->Draw(g_pLineArr, 4, D3DCOLOR_XRGB(255, 255, 255));

    // 结束绘制
    g_pd3dDevice->EndScene();

    // 提交后台缓存
    g_pd3dDevice->Present(0, 0, 0, 0);
}

// 清理创建的对象
void ReleaseALL()
{
    if (g_pd3dObject)
        g_pd3dObject->Release();
    if (g_pd3dDevice)
        g_pd3dDevice->Release();
    if (g_pLine)
        g_pLine->Release();
    if (g_pLineArr)
        delete g_pLineArr;
}

// 窗口初始化
void InitWindow(HINSTANCE hInstance, HWND &hwnd)
{
    std::string winClass("MainWindows");    // 窗口类名
    std::string winCap("DrawLines");        // 窗口标题
    WNDCLASS wc;
    wc.style         = 0;       // 窗口风格，此处为缺省类型
    wc.lpfnWndProc   = WndProc; // 选择该窗口的消息处理函数
    wc.cbClsExtra    = 0;       // 窗口类无扩展
    wc.cbWndExtra    = 0;       // 窗口实例无扩展
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);       // 窗口的最小化图标为缺省
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);           // 采用箭头光标
    wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH)); // 窗口背景为白色
    wc.lpszMenuName  = NULL;                                  // 窗口无菜单
    wc.lpszClassName = winClass.c_str();                      // 窗口类名

    // 注册窗口
    if (FAILED(RegisterClass(&wc))) {
        MessageBox(0, "RegisterClass FAILED", 0, 0);
        PostQuitMessage(0);
    }
    // 创建窗口实例
    hwnd = CreateWindow(winClass.c_str(),    // 窗口类名
                        winCap.c_str(),      // 窗口标题
                        WS_OVERLAPPEDWINDOW, // 窗口风格
                        CW_USEDEFAULT,       // 左上角坐标
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,       // 高和宽
                        CW_USEDEFAULT,
                        NULL,                // 无父窗口
                        NULL,                // 无子菜单
                        hInstance,           // 创建该窗口的程序句柄
                        NULL);               // 不使用该值
    if (!hwnd) {
        MessageBox(0, "CreateWindow FAILED", 0, 0);
        PostQuitMessage(0);
    }

    ShowWindow(hwnd, SW_SHOW); // 显示窗口
    UpdateWindow(hwnd);        // 绘制用户区
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int)
{
    HWND hwnd;
    InitWindow(hInstance, hwnd);
    InitD3D(hwnd);
    
    // 开始消息循环
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        // PeekMessage是一个消息检查线程消息队列，以查看的方式获取消息
        // 这里之所以不用GetMessage，是因为GetMessage是阻塞函数
        // PM_REMOVE表示消息处理后会从队列中去除
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            // 转化消息对，如将键盘中WM_KEYUP/WM_KEYDOWN消息对转化为WM_CHAR消息
            // 并将转化后获得的新消息传递到消息队列中，它不影响原消息，只会产生新消息
            TranslateMessage(&msg);
            // 将取出的消息传递给该窗口的回调函数处理
            DispatchMessage(&msg);
        } else {
            DrawScene();
        }
    }

    exit (0);
}
