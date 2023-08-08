#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <math.h> //we want to use some of the functions of this e.g isfinite, isnan

/* Function to calculate oil in place */
double CalculateOilInPlace(double Area, double netPay, double porosity, double oilSaturation, double Bo)
{
    //I am considering just field units in this program

    // Error handling
    // 1. handle letter and symbol inputs, handle numbers less than 0.
    // 2. For porosity and Oil Sat, handle val above 1
if (!isfinite(Area) || !isfinite(netPay) || !isfinite(porosity) || !isfinite(oilSaturation) || !isfinite(Bo)) {
        MessageBox(NULL, _T("Invalid input. Please enter valid numbers."), _T("Error"), MB_ICONERROR);
        return 0.0;
    }
else{
    double oilInPlace = (7758 * Area * netPay * porosity * oilSaturation) / Bo;
    return oilInPlace;}
}
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Making the class name into a global variable  */
TCHAR szClassName[ ] = _T("Oil in Place Calculator");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Oil in Place Calculator"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndArea, hwndNetPay, hwndPorosity, hwndOilSaturation, hwndBo, hwndCalculateButton, hwndResultLabel;
    static double Area = 0.0, netPay = 0.0, porosity = 0.0, oilSaturation = 0.0, Bo = 0.0; //default values if theres any error
    TCHAR buffer[50];

    switch (message) /* Handle the messages */
    {
    case WM_CREATE:
    {
        // Calculate the width for all controls
        int controlWidth = 150; // because I want the controls to have constant widths to make it more organized

        // Create controls with consistent sizes
        CreateWindow(_T("STATIC"), _T("Area (acres):"), WS_VISIBLE | WS_CHILD, 10, 10, controlWidth, 25, hwnd, NULL, NULL, NULL); // Text field for the Area input
        hwndArea = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 10 + controlWidth, 10, controlWidth, 25, hwnd, NULL, NULL, NULL); //Macros/ input field for the area. Same thing for the rest of the parameters
                                                                                //the nums are more like the margin specifiers
        CreateWindow(_T("STATIC"), _T("Net Pay (ft):"), WS_VISIBLE | WS_CHILD, 10, 40, controlWidth, 25, hwnd, NULL, NULL, NULL);
        hwndNetPay = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 10 + controlWidth, 40, controlWidth, 25, hwnd, NULL, NULL, NULL);

        CreateWindow(_T("STATIC"), _T("Porosity (decimal):"), WS_VISIBLE | WS_CHILD, 10, 70, controlWidth, 25, hwnd, NULL, NULL, NULL);
        hwndPorosity = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 10 + controlWidth, 70, controlWidth, 25, hwnd, NULL, NULL, NULL);

        CreateWindow(_T("STATIC"), _T("So (decimal):"), WS_VISIBLE | WS_CHILD, 10, 100, controlWidth, 25, hwnd, NULL, NULL, NULL);
        hwndOilSaturation = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 10 + controlWidth, 100, controlWidth, 25, hwnd, NULL, NULL, NULL);

        CreateWindow(_T("STATIC"), _T("Bo (bbl/STB):"), WS_VISIBLE | WS_CHILD, 10, 130, controlWidth, 25, hwnd, NULL, NULL, NULL);
        hwndBo = CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 10 + controlWidth, 130, controlWidth, 25, hwnd, NULL, NULL, NULL);

        hwndCalculateButton = CreateWindow(_T("BUTTON"), _T("Calculate"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 160, controlWidth * 2, 30, hwnd, (HMENU)1, NULL, NULL);
        hwndResultLabel = CreateWindow(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD, 10, 200, controlWidth * 2, 25, hwnd, NULL, NULL, NULL);
    }
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == 1)
        {
            /* When Calculate button is clicked */
            GetWindowText(hwndArea, buffer, sizeof(buffer));
            Area = _tstof(buffer);

            GetWindowText(hwndNetPay, buffer, sizeof(buffer));
            netPay = _tstof(buffer);

            GetWindowText(hwndPorosity, buffer, sizeof(buffer));
            porosity = _tstof(buffer);

            GetWindowText(hwndOilSaturation, buffer, sizeof(buffer));
            oilSaturation = _tstof(buffer);

            GetWindowText(hwndBo, buffer, sizeof(buffer));
            Bo = _tstof(buffer);

            double oilInPlace = CalculateOilInPlace(Area, netPay, porosity, oilSaturation, Bo);
            _stprintf_s(buffer, _T("Oil in Place: %.2f MMSTB"), oilInPlace/1000000);
            SetWindowText(hwndResultLabel, buffer);
        }
        break;


    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);

    }
    return 0;
}


