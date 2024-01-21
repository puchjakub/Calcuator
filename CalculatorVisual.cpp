
#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "framework.h"
#include "CalculatorVisual.h"
#include <string>
#include "Calculator.h"


#define MAX_LOADSTRING 100

#define IDB_BUTTON1 201
#define IDB_BUTTON2 202
#define IDB_BUTTON3 203
#define IDB_BUTTON4 204
#define IDB_BUTTON5 205
#define IDB_BUTTON6 206
#define IDB_BUTTON7 207
#define IDB_BUTTON8 208
#define IDB_BUTTON9 209

#define IDC_INPUT_EDIT 120
#define IDC_EQUALS_BUTTON 121
#define IDC_DECIMAL_BUTTON 122 

#define IDC_ADD_BUTTON 301
#define IDC_SUBTRACT_BUTTON 302
#define IDC_MULTIPLY_BUTTON 303
#define IDC_DIVIDE_BUTTON 304
#define IDC_POWER_BUTTON 305

#define IDC_BIN_BUTTON 401
#define IDC_OCT_BUTTON 402
#define IDC_HEX_BUTTON 403
#define IDC_FLOAT_BUTTON 404
#define IDC_MODE_INDICATOR 405

#define IDB_BUTTON_A 501
#define IDB_BUTTON_B 502
#define IDB_BUTTON_C 503
#define IDB_BUTTON_D 504
#define IDB_BUTTON_E 505
#define IDB_BUTTON_F 506

HINSTANCE hInst;                              
WCHAR szTitle[MAX_LOADSTRING];                
WCHAR szWindowClass[MAX_LOADSTRING];          

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Calculator calculator;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CALCULATORVISUAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALCULATORVISUAL));

    MSG msg;

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCULATORVISUAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CALCULATORVISUAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

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

void CreateOperationButton(HWND parent, const wchar_t* text, int id, int x, int y, int buttonWidth, int buttonHeight) {
    CreateWindowW(L"BUTTON", text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, buttonWidth, buttonHeight,
        parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), hInst, NULL);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
    {
        HWND hInputEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
            10, 10, 230, 30,
            hWnd, (HMENU)IDC_INPUT_EDIT, GetModuleHandle(NULL), NULL);

        int numRows = 4;
        int buttonsPerRow = 3;
        int buttonHeight = 40;
        int buttonWidth = 40;
        int startX = 10;
        int startY = 50;
        int padding = 10;

        // Number buttons (1-9)
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < buttonsPerRow; j++) {
                int buttonID = IDB_BUTTON1 + i * buttonsPerRow + j;
                int number = i * buttonsPerRow + j + 1;
                if (number > 9) break;
                int x = startX + j * (buttonWidth + padding);
                int y = startY + i * (buttonHeight + padding);

                std::wstring buttonText = std::to_wstring(number);
                CreateWindowW(L"BUTTON", buttonText.c_str(),
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    x, y, buttonWidth, buttonHeight,
                    hWnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(buttonID)), hInst, NULL);
            }
        }

        // 0 button
        CreateWindow(L"BUTTON", L"0",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            startX, startY + 3 * (buttonHeight + padding),
            buttonWidth, buttonHeight,
            hWnd, (HMENU)(IDB_BUTTON1 + 9), hInst, NULL);

        CreateWindow(L"BUTTON", L"=",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            startX + 1 * (buttonWidth + padding), startY + 3 * (buttonHeight + padding),
            buttonWidth, buttonHeight,
            hWnd, (HMENU)IDC_EQUALS_BUTTON, hInst, NULL);

        int decimalButtonX = startX + 2 * (buttonWidth + padding);

        CreateWindow(L"BUTTON", L".",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            decimalButtonX, startY + 3 * (buttonHeight + padding),
            buttonWidth, buttonHeight,
            hWnd, (HMENU)IDC_DECIMAL_BUTTON, hInst, NULL);

        // operations buttons
        CreateOperationButton(hWnd, L"+", IDC_ADD_BUTTON, startX + 3 * (buttonWidth + padding), startY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"-", IDC_SUBTRACT_BUTTON, startX + 3 * (buttonWidth + padding), startY + (buttonHeight + padding), buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"×", IDC_MULTIPLY_BUTTON, startX + 3 * (buttonWidth + padding), startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"÷", IDC_DIVIDE_BUTTON, startX + 3 * (buttonWidth + padding), startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight);

        int newStartY = startY + 4 * (buttonHeight + padding);


        CreateOperationButton(hWnd, L"BIN", IDC_BIN_BUTTON, startX, newStartY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"OCT", IDC_OCT_BUTTON, startX + (buttonWidth + padding), newStartY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"HEX", IDC_HEX_BUTTON, startX + 2 * (buttonWidth + padding), newStartY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"FLT", IDC_FLOAT_BUTTON, startX + 3 * (buttonWidth + padding), newStartY, buttonWidth, buttonHeight);

        int startX_AF = startX + 4 * (buttonWidth + padding);

        // Create buttons A to F
        CreateOperationButton(hWnd, L"A", IDB_BUTTON_A, startX_AF, startY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"B", IDB_BUTTON_B, startX_AF, startY + (buttonHeight + padding), buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"C", IDB_BUTTON_C, startX_AF, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"D", IDB_BUTTON_D, startX_AF, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"E", IDB_BUTTON_E, startX_AF, newStartY, buttonWidth, buttonHeight);
        CreateOperationButton(hWnd, L"F", IDB_BUTTON_F, startX_AF, newStartY + (buttonHeight + padding), buttonWidth, buttonHeight);

        int startX_Power = startX_AF - (buttonWidth + padding);
        CreateOperationButton(hWnd, L"^", IDC_POWER_BUTTON, startX_Power, newStartY + (buttonHeight + padding), buttonWidth, buttonHeight);


        int indicatorWidth = 2 * buttonWidth + padding;
        int indicatorHeight = buttonHeight; 
        int indicatorXPosition = startX;
        int indicatorYPosition = newStartY + buttonHeight + padding;

        HWND hModeIndicator = CreateWindowW(L"STATIC", L"FLT",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            indicatorXPosition, indicatorYPosition, indicatorWidth, indicatorHeight,
            hWnd, (HMENU)IDC_MODE_INDICATOR, hInst, NULL);


        break;

     }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDB_BUTTON1: 
            case IDB_BUTTON2:
            case IDB_BUTTON3:
            case IDB_BUTTON4: 
            case IDB_BUTTON5:
            case IDB_BUTTON6: 
            case IDB_BUTTON7:
            case IDB_BUTTON8: 
            case IDB_BUTTON9:
            case IDB_BUTTON1 + 9:
            {
                int number = wmId - IDB_BUTTON1 + 1;
                if (wmId == (IDB_BUTTON1 + 9)) // Adjust for 0 button
                {
                    number = 0;
                }

                if (Calculator::getMode() == CalculatorMode::BIN && number > 1) {
                    break;
                }

                if (Calculator::getMode() == CalculatorMode::OCT && number > 7) {
                    break;
                }

                WCHAR szCurrentText[256];
                GetWindowTextW(GetDlgItem(hWnd, IDC_INPUT_EDIT), szCurrentText, _countof(szCurrentText));

                std::wstring newText(szCurrentText);
                newText += std::to_wstring(number);

                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), newText.c_str());
            }
            break;
            case IDB_BUTTON_A:
            case IDB_BUTTON_B:
            case IDB_BUTTON_C: 
            case IDB_BUTTON_D: 
            case IDB_BUTTON_E:
            case IDB_BUTTON_F:
            {
                if (Calculator::getMode() == CalculatorMode::HEX) {
                    wchar_t hexChar;
                    switch (wmId) {
                    case IDB_BUTTON_A: hexChar = L'A'; break;
                    case IDB_BUTTON_B: hexChar = L'B'; break;
                    case IDB_BUTTON_C: hexChar = L'C'; break;
                    case IDB_BUTTON_D: hexChar = L'D'; break;
                    case IDB_BUTTON_E: hexChar = L'E'; break;
                    case IDB_BUTTON_F: hexChar = L'F'; break;
                    default: break;
                    }

                    WCHAR szCurrentText[256];
                    GetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), szCurrentText, _countof(szCurrentText));

                    std::wstring newText(szCurrentText);
                    newText += hexChar;

                    SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), newText.c_str());
                }
            }
            break;

            case IDC_DECIMAL_BUTTON:
            {
                WCHAR szCurrentText[256];
                GetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), szCurrentText, 256);

                // Find the last operator
                const wchar_t* operators = L"+-×÷";
                const wchar_t* lastOperator = wcsrchr(szCurrentText, L'+');
                for (const wchar_t* op = operators; *op != L'\0'; ++op) {
                    const wchar_t* pos = wcsrchr(szCurrentText, *op);
                    if (pos > lastOperator) {
                        lastOperator = pos;
                    }
                }

                // Check if a decimal point already exists after the last operator
                const wchar_t* lastDecimal = wcsrchr(szCurrentText, L'.');
                if (lastDecimal == NULL || (lastOperator != NULL && lastDecimal < lastOperator)) {
                    // Append the decimal point
                    std::wstring newText(szCurrentText);
                    newText += L".";
                    SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), newText.c_str());
                }
                break;
            }
            case IDC_ADD_BUTTON:
            case IDC_SUBTRACT_BUTTON: 
            case IDC_MULTIPLY_BUTTON: 
            case IDC_DIVIDE_BUTTON:
            case IDC_POWER_BUTTON:
            {
                wchar_t operation;
                if (wmId == IDC_ADD_BUTTON) {
                    operation = L'+';
                }
                else if (wmId == IDC_SUBTRACT_BUTTON) {
                    operation = L'-';
                }
                else if (wmId == IDC_MULTIPLY_BUTTON) {
                    operation = L'×';
                }
                else if (wmId == IDC_DIVIDE_BUTTON) {
                    operation = L'÷';
                }
                else if (wmId == IDC_POWER_BUTTON) {
                    operation = L'^';
                }
                else {
                    break;
                }

                WCHAR szCurrentText[256];
                GetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), szCurrentText, 256);

                std::wstring newText(szCurrentText);
                newText += operation;

                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), newText.c_str());
            }
            break;
            case IDC_EQUALS_BUTTON: {
                WCHAR szCurrentText[256];
                GetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), szCurrentText, 256);
                std::wstring expr(szCurrentText);

                try {
                    auto currentMode = Calculator::getMode();
                    std::unique_ptr<Expression> parsedExpression;

                    if (currentMode == CalculatorMode::BIN) {
                        parsedExpression = Calculator::parse(Calculator::convertBinaryToDecimalExpression(expr));
                    }
                    else if (currentMode == CalculatorMode::OCT) {
                        parsedExpression = Calculator::parse(Calculator::convertOctalToDecimalExpression(expr));
                    }
                    else if (currentMode == CalculatorMode::HEX) {
                        parsedExpression = Calculator::parse(Calculator::convertHexToDecimalExpression(expr));
                    }
                    else {
                        parsedExpression = Calculator::parse(expr);
                    }

                    double result = parsedExpression->evaluate();

                    std::wstring resultStr;
                    if (currentMode == CalculatorMode::BIN) {
                        resultStr = Calculator::decimalToBinary(result);
                    }
                    else if (currentMode == CalculatorMode::OCT) {
                        resultStr = Calculator::decimalToOctal(result);
                    }
                    else if (currentMode == CalculatorMode::HEX) {
                        resultStr = Calculator::decimalToHex(result);
                    }
                    else {
                        resultStr = std::to_wstring(result);
                    }

                    SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), resultStr.c_str());
                }
                catch (const std::exception& e) {
                    MessageBox(hWnd, L"Error evaluating expression", L"Error", MB_OK | MB_ICONERROR);
                }
                break;
            }
            case IDC_BIN_BUTTON:
                calculator.switchMode(CalculatorMode::BIN);
                SetWindowText(GetDlgItem(hWnd, IDC_MODE_INDICATOR), L"BIN");
                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), L"");
                break;
            case IDC_OCT_BUTTON:
                calculator.switchMode(CalculatorMode::OCT);
                SetWindowText(GetDlgItem(hWnd, IDC_MODE_INDICATOR), L"OCT");
                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), L"");
                break;
            case IDC_HEX_BUTTON:
                calculator.switchMode(CalculatorMode::HEX);
                SetWindowText(GetDlgItem(hWnd, IDC_MODE_INDICATOR), L"HEX");
                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), L"");
                break;
            case IDC_FLOAT_BUTTON:
                calculator.switchMode(CalculatorMode::FLT);
                SetWindowText(GetDlgItem(hWnd, IDC_MODE_INDICATOR), L"FLT");
                SetWindowText(GetDlgItem(hWnd, IDC_INPUT_EDIT), L"");
                break;
           

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

// Message handler for about box.
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

