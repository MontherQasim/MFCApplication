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
	/*
     IMPORTANT – THREAD-SAFETY NOTE

     This worker thread must NOT call MFC UI functions or access UI objects
     (CWnd, CDialog, CView, CDocument, etc.) directly.

     MFC UI objects are created in the main GUI thread, and many of their member 
	 functions are NOT thread-safe. Calling UI functions from a worker thread may:

     • cause ASSERT failures in Debug builds
     • crash randomly in Release builds
     • corrupt internal MFC handle maps
     • produce undefined behaviour

    Therefore the worker thread performs only the background calculation here,
    and when the result is ready it notifies the main thread using PostMessage().
    The message is then handled safely inside the GUI thread.

	This is NOT safe:

	 CMainFrame* pFrame = static_cast<CMainFrame*>(pData->pNotifyWnd);

	 pFrame->OnFactorialComplete((WPARAM)result, 0);
	 pFrame->SetWindowText(msg);             
	 pFrame->m_wndStatusBar.SetWindowText(msg);

	 We have to use PostMessage to notify the main thread. Passing HWND is better than using pointers to MFC objects.

	*/
	{
		// ::Sleep (0) Allow other threads to run
		pData->pNotifyWnd->PostMessage (WM_FACTORIAL_COMPLETE, (WPARAM)result, 0);
	}

	delete pData;

	return 0;
}