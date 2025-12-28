#include "pch.h"
#include "ThreadWindow.h"

BEGIN_MESSAGE_MAP (ThreadWindow, CFrameWnd)
	ON_WM_CLOSE ()
	ON_WM_KEYDOWN ()
END_MESSAGE_MAP ()

ThreadWindow::ThreadWindow ()
{
	Create (NULL, _T ("UI Thread Window Demo"),
		WS_OVERLAPPEDWINDOW, CRect (100, 100, 600, 400));
}

ThreadWindow::~ThreadWindow ()
{
}

void ThreadWindow::OnClose ()
{
	// TODO: Add a handler for the ESC key; when pressed, terminate the UI thread cleanly by posting WM_QUIT using AfxPostQuitMessage.
	CFrameWnd::OnClose ();
}

void ThreadWindow::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_ESCAPE)
    {
        CWnd* pMain = AfxGetApp ()->m_pMainWnd;
        if (pMain && ::IsWindow (pMain->m_hWnd))
        {
            pMain->PostMessage (WM_STOP_WORKER, 0, 0);
		}
        // Alternative: AfxPostQuitMessage (0);
        ::PostMessage (m_hWnd, WM_CLOSE, 0, 0);
        return; 
    }

    CFrameWnd::OnKeyDown (nChar, nRepCnt, nFlags);
}