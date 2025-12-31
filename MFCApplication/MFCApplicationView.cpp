
// MFCApplicationView.cpp : implementation of the CMFCApplicationView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication.h"
#endif

#include "MFCApplicationDoc.h"
#include "MFCApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplicationView
CMFCApplicationView* g_pView = nullptr;

IMPLEMENT_DYNCREATE(CMFCApplicationView, CView)

BEGIN_MESSAGE_MAP(CMFCApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplicationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY ()
 //   ON_WM_TIMER ()
	ON_WM_CREATE ()
	//ON_WM_SIZE ()
	ON_WM_TIMER ()
	ON_WM_DESTROY ()
	ON_COMMAND (ID_DEMO_PROGRESS, &CMFCApplicationView::OnDemoProgress)
	ON_UPDATE_COMMAND_UI (ID_DEMO_PROGRESS, &CMFCApplicationView::OnUpdateDemoProgress)
END_MESSAGE_MAP()

// CMFCApplicationView construction/destruction

CMFCApplicationView::CMFCApplicationView() noexcept
{
	// TODO: add construction code here
	m_counter = 0;
	m_bProgressCreated = FALSE;
	m_nProgressPos = 0;
}

CMFCApplicationView::~CMFCApplicationView()
{
	//KillTimer (1);	  // or inside OnDestroy
}

int CMFCApplicationView::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if (CView::OnCreate (lpCreateStruct) == -1)
		return -1;
	CRect rectProgress (50, 50, 700, 100); if (m_wndProgress.Create (WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_SMOOTH, rectProgress, this, IDC_PROGRESS)) {
		m_bProgressCreated = TRUE; m_wndProgress.SetRange (0, 100); m_wndProgress.SetPos (50); m_wndProgress.SetStep (1); m_nProgressPos = 0;
	} return 0;
}
void CMFCApplicationView::OnInitialUpdate ()
{
	CView::OnInitialUpdate ();

	g_pView = this;

	::SetTimer (GetSafeHwnd (), 1, 1000, CMFCApplicationView::MyTimerProc);
}
/*
int CMainWindow::OnCreate (LPCREATESTRUCT lpcs)
{
	if (CFrameWnd::OnCreate (lpcs) == -1)
		return -1;

	if (!SetTimer (ID_TIMER_ELLIPSE, 100, NULL)) {
		MessageBox (_T ("Error: SetTimer failed"));
		return -1;
	}
	return 0;
}
*/

BOOL CMFCApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCApplicationView drawing

void CMFCApplicationView::OnDraw(CDC* pDC)
{
	CMFCApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CString s;
	s.Format (_T ("Seconds passed: %d"), m_counter);

	pDC->TextOut (10, 10, s);
}

void CMFCApplicationView::DrawRectangle (CDC* pDC)
{

	/*
	Create three pens.
	CPen pen1 (PS_SOLID, 16, RGB (242, 36, 204));	 // RGB color dont't use palette
	CPen pen2 (PS_SOLID, 16, PALETTERGB (242, 36, 204)); // RGB color use palette
	CPen pen3 (PS_SOLID, 16, PALETTEINDEX (3));			 // Palette index
	*/
	COLORREF clr = RGB (255, 0, 255);  

	BYTE r = GetRValue (clr);
	BYTE g = GetGValue (clr);
	BYTE b = GetBValue (clr);

	CString text;
	text.Format (_T ("Color: R=%d  G=%d  B=%d"), r, g, b);
	pDC->TextOut (10, 10, text);

	CBrush brush (clr);              
	CBrush* pOldBrush = pDC->SelectObject (&brush);

	pDC->Rectangle (10, 40, 160, 140); 

	pDC->SelectObject (pOldBrush);  
}

void CMFCApplicationView::DrawEllipse (CDC* pDC) 
{
	CPalette pal;
	pal.CreateHalftonePalette (pDC);

	/*
	 palette.Attach(::CreateHalftonePalette(NULL));
	*/

	CPalette* pOld = pDC->SelectPalette (&pal, FALSE);
	pDC->RealizePalette ();

	CBrush br1 (RGB (255, 0, 0));  
	CBrush br2 (RGB (0, 0, 255)); 

	pDC->SelectObject (&br1);
	pDC->Ellipse (10, 10, 120, 100);

	pDC->SelectObject (&br2);
	pDC->Ellipse (140, 10, 250, 100);

	if (pOld)
		pDC->SelectPalette (pOld, FALSE);
}

void CMFCApplicationView::DrawPie (CDC* pDC)
{
	const int NUM_COLORS = 32;

	struct
	{
		WORD         palVersion;
		WORD         palNumEntries;
		PALETTEENTRY palPalEntry[NUM_COLORS];
	} pal;

	pal.palVersion = 0x300;
	pal.palNumEntries = NUM_COLORS;

	for (int i = 0; i < NUM_COLORS; ++i)
	{
		BYTE v = (BYTE)(i * 8);
		pal.palPalEntry[i].peRed = v;
		pal.palPalEntry[i].peGreen = 0;  
		pal.palPalEntry[i].peBlue = 0; 
		pal.palPalEntry[i].peFlags = 0;	 //   PC_EXPLICIT ,PC_NOCOLLAPSE, PC_RESERVED
	}
	CPalette palette;
	palette.CreatePalette ((LOGPALETTE*)&pal);
	CPalette* pOld = pDC->SelectPalette (&palette, FALSE);
	// The palette manager maps (dithers) the logical palette to the system palette
	pDC->RealizePalette ();
	CBrush brush (RGB (255, 0, 0));
	CBrush* pOldBrush = pDC->SelectObject (&brush);
	pDC->Pie (10, 10, 200, 200, 10, 100, 100, 10);
	pDC->SelectObject (pOldBrush);
	if (pOld)
		pDC->SelectPalette (pOld, FALSE);
}

void CMFCApplicationView::CheckPaletteSupport ()
{
	CClientDC dc (this);

	BOOL bUsePalette = FALSE;

	if (dc.GetDeviceCaps (RASTERCAPS) & RC_PALETTE)
		bUsePalette = TRUE;

	if (bUsePalette)
		AfxMessageBox (_T ("This device supports a hardware palette (8-bit mode)."));
	else
		AfxMessageBox (_T ("No hardware palette support. Logical palettes are not needed."));
}

//void CMFCApplicationView::OnTimer (UINT_PTR nIDEvent)
//{
//	if (nIDEvent == 1)
//	{
//		m_counter++;     
//		Invalidate ();
//	}
//
//	CView::OnTimer (nIDEvent);
//}
VOID CALLBACK CMFCApplicationView::MyTimerProc (
	HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	UNREFERENCED_PARAMETER (hwnd);
	UNREFERENCED_PARAMETER (uMsg);
	UNREFERENCED_PARAMETER (dwTime);

	if (idEvent != 1)
		return;

	if (g_pView == nullptr)
		return;

	g_pView->m_counter++;       
	g_pView->Invalidate (FALSE);    
}

// CMFCApplicationView printing


void CMFCApplicationView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCApplicationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplicationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMFCApplicationView::OnUpdateDemoProgress (CCmdUI* pCmdUI)
{
	pCmdUI->Enable (m_bProgressCreated);
}
void CMFCApplicationView::OnDemoProgress ()
{
	if (!m_bProgressCreated || !m_wndProgress.GetSafeHwnd ())
		return;

	SetTimer (IDT_PROGRESS_TIMER, 100, NULL);

	AfxMessageBox (_T ("Progress demo started!\nWatch the progress bar fill automatically."),
		MB_OK | MB_ICONINFORMATION);
}

void CMFCApplicationView::OnTimer (UINT_PTR nIDEvent)
{
	if (nIDEvent == IDT_PROGRESS_TIMER)
	{
		m_nProgressPos++;

		if (m_nProgressPos <= 100)
		{
			m_wndProgress.SetPos (m_nProgressPos);
		}
		else
		{
			KillTimer (IDT_PROGRESS_TIMER);

			m_nProgressPos = 0;

			AfxMessageBox (_T ("Progress completed!\nYou can run the demo again from the context menu."),
				MB_OK | MB_ICONINFORMATION);
		}
	}

	CView::OnTimer (nIDEvent);
}
void CMFCApplicationView::OnDestroy ()
{
	KillTimer (1);

	KillTimer (IDT_PROGRESS_TIMER);

	if (m_wndProgress.GetSafeHwnd ())
	{
		m_wndProgress.DestroyWindow ();
	}

	m_bProgressCreated = FALSE;

	CView::OnDestroy ();
}



// CMFCApplicationView diagnostics

#ifdef _DEBUG
void CMFCApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplicationDoc* CMFCApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplicationDoc)));
	return (CMFCApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplicationView message handlers
