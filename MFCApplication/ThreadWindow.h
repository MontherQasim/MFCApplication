#pragma once

class ThreadWindow : public CFrameWnd
{
public:
    ThreadWindow ();
    virtual ~ThreadWindow ();

protected:
    afx_msg void OnClose ();

    DECLARE_MESSAGE_MAP ()
};
