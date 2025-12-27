#include "pch.h"
#include "WorkerThread.h"

unsigned long long CalculateFactorial (int n)
{
    if (n <= 1)
        return 1;
    return n * CalculateFactorial (n - 1);
}

/*
 The use of global or static variables should be avoided.
 Local variables are safe because each thread has its own stack.
 */

UINT FactorialWorkerThread (LPVOID pParam)
{
    FactorialThreadData* pData = (FactorialThreadData*)pParam;

    if (pData->nInput > 40 || pData->nInput < 0)
    {
        delete pData;
        AfxEndThread (0);
		return 0; // to avoid warning
    }

    unsigned long long result = CalculateFactorial (pData->nInput);

    if (pData->pNotifyWnd && ::IsWindow (pData->pNotifyWnd->m_hWnd))
    {
        // ::Sleep (0) Allow other threads to run
        ::Sleep (5000);
        pData->pNotifyWnd->PostMessage (WM_FACTORIAL_COMPLETE, (WPARAM)result, 0);
    }

    delete pData;

    return 0;
}