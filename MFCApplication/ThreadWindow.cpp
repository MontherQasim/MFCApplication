#include "pch.h"
#include "ThreadWindow.h"

BEGIN_MESSAGE_MAP (ThreadWindow, CFrameWnd)
	ON_WM_CLOSE ()
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
