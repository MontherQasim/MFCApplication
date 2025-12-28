#pragma once

#define WM_STOP_WORKER   (WM_USER + 2)

class ThreadWindow : public CFrameWnd
{
public:
    ThreadWindow ();
    virtual ~ThreadWindow ();

protected:
    afx_msg void OnClose ();
    afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP ()
};
