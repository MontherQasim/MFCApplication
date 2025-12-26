#include "pch.h"
#include "UIThread.h"
#include "ThreadWindow.h"

IMPLEMENT_DYNCREATE (UIThread, CWinThread)

BEGIN_MESSAGE_MAP (UIThread, CWinThread)
END_MESSAGE_MAP ()

UIThread::UIThread ()
{
}

UIThread::~UIThread ()
{
}

BOOL UIThread::InitInstance ()
{
    m_pMainWnd = new ThreadWindow;
    m_pMainWnd->ShowWindow (SW_SHOW);
    m_pMainWnd->UpdateWindow ();
    return TRUE;
}
