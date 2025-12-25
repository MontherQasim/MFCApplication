#pragma once

#define WM_FACTORIAL_COMPLETE (WM_USER + 1)

struct FactorialThreadData
{
    int nInput;
    CWnd* pNotifyWnd;
};

// It must be a standalone function or a static function.
UINT FactorialWorkerThread (LPVOID pParam);

unsigned long long CalculateFactorial (int n
);