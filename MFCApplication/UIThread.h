#pragma once

class UIThread : public CWinThread
{
    DECLARE_DYNCREATE (UIThread)

protected:
    UIThread ();
    virtual ~UIThread ();

public:
    virtual BOOL InitInstance ();

    DECLARE_MESSAGE_MAP ()
};
