
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

IMPLEMENT_DYNCREATE(CMFCApplicationView, CView)

BEGIN_MESSAGE_MAP(CMFCApplicationView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplicationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCApplicationView construction/destruction

CMFCApplicationView::CMFCApplicationView() noexcept
{
	// TODO: add construction code here

}

CMFCApplicationView::~CMFCApplicationView()
{
}

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
