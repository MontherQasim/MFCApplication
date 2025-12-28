#include "pch.h"
#include "WorkerThread.h"

unsigned long long CalculateFactorial (int n, volatile LONG* pContinue)
{
    if (*pContinue == 0)
    {
        return -1;
    }
    if (n <= 1)
        return 1;
    return n * CalculateFactorial(n - 1, pContinue);
}

/*
 The use of global or static variables should be avoided.
 Local variables are safe because each thread has its own stack.
 */

UINT FactorialWorkerThread (LPVOID pParam)
{
    auto* pData = static_cast<FactorialThreadData*>(pParam);

    if (!pData)
        return 0;

    if (!pData->pContinue || *(pData->pContinue) == 0)
    {
        delete pData;
        return 0;
    }

    if (pData->nInput > 40 || pData->nInput < 0)
    {
        delete pData;
        AfxEndThread (0);
    }

    unsigned long long result =
        CalculateFactorial (pData->nInput, pData->pContinue);
    Sleep (5000);

    if (pData->pContinue &&
        *pData->pContinue != 0 &&
        result != (unsigned long long) - 1 &&
        pData->pNotifyWnd &&
        ::IsWindow (pData->pNotifyWnd->m_hWnd))
    {
     // ::Sleep (0) Allow other threads to run
        pData->pNotifyWnd->PostMessage (WM_FACTORIAL_COMPLETE, (WPARAM)result, 0);
    }

    delete pData;

    return 0;
}