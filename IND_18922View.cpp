
// IND_18922View.cpp : implementation of the CIND18922View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18922.h"
#include "DImage.h"
#endif

#include "IND_18922Doc.h"
#include "IND_18922View.h"

#define TO_RAD(x) (x*3.14159/180)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

constexpr auto CENTAR_X = 250;
constexpr auto CENTAR_Y = 250;

#define DEBLJINA 60


// CIND18922View

IMPLEMENT_DYNCREATE(CIND18922View, CView)

BEGIN_MESSAGE_MAP(CIND18922View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18922View construction/destruction

CIND18922View::CIND18922View() noexcept
{
	bmpImage.Load(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\download.bmp");
	// TODO: add construction code here
	this->keyPressed = false;
	this->gridSize = 25; //500px/20 =25px po kvadratu

}

CIND18922View::~CIND18922View()
{
}

BOOL CIND18922View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}




void CIND18922View::OnDraw(CDC* pDC)
{
	CIND18922Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int n = this->gridSize;

	CRect rect;
	GetClientRect(&rect);

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	MemDC->SelectObject(&bmp);
	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = MemDC->SelectObject(&brush);
	MemDC->Rectangle(0, 0, rect.Width(), rect.Height());

	//Grid(MemDC, width, height, showGrid);

	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);
	MemDC->SetGraphicsMode(GM_ADVANCED);

	XFORM old;
	MemDC->GetWorldTransform(&old);
	this->AntiFlicker(*MemDC);
	this->Photo(MemDC);
	MemDC->SetWorldTransform(&old);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
	MemDC->DeleteDC();
	delete MemDC;




	CPen* gridPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* stariPen = (CPen*)pDC->SelectObject(gridPen);


	


	if (this->keyPressed)
	{
		for (long x = 0; x <= this->gridSize * 20; x += this->gridSize) //vertikalne linije
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, this->gridSize * 20);
		}
		for (long y = 0; y <= this->gridSize * 20; y += this->gridSize) //horizontalne linije
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(this->gridSize * 20, y);
		}
	}



	gridPen->DeleteObject();
}

void CIND18922View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = 1.0;
	xForm.eM12 = 0.0;
	xForm.eM21 = 0.0;
	xForm.eM22 = 1.0;
	xForm.eDx = dX;
	xForm.eDy = dY;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18922View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xForm{};
	xForm.eM11 = sX;
	xForm.eM12 = 0.0;
	xForm.eM21 = 0.0;
	xForm.eM22 = sY;
	xForm.eDx = 0.0;
	xForm.eDy = 0.0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CIND18922View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xForm{};
	float radians = angle * 3.14159f / 180.0f; // Konverzija stepeni u radijane
	xForm.eM11 = cos(radians);
	xForm.eM12 = sin(radians);
	xForm.eM21 = -sin(radians);
	xForm.eM22 = cos(radians);
	xForm.eDx = 0.0;
	xForm.eDy = 0.0;

	pDC->ModifyWorldTransform(&xForm, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);//za izmenu trenutne transformacije DC-a
}


void CIND18922View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply) {
	XFORM transform;
	transform.eM11 = mx ? -1.0f : 1.0f;
	transform.eM12 = 0.0f;
	transform.eM21 = 0.0f;
	transform.eM22 = my ? -1.0f : 1.0f;
	transform.eDx = 0.0f;
	transform.eDy = 0.0f;
	pDC->ModifyWorldTransform(&transform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


void CIND18922View::Photo(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	//if (this->stanje == 1)
	//	Grid(pDC, rect);

	transDeo(pDC, -CENTAR_X + 145, CENTAR_Y - 145, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d1.bmp"), -155);
	transDeo(pDC, -CENTAR_X + 9, CENTAR_Y - 2, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d2.bmp"), 58);
	transDeo(pDC, -CENTAR_X - 140, CENTAR_Y, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d3.bmp"), 72);
	transDeo(pDC, -CENTAR_X + 155, CENTAR_Y + 142, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d4.bmp"), 19);
	transDeo(pDC, -CENTAR_X - 156, CENTAR_Y - 155, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d5.bmp"), -76);
	transDeo(pDC, -CENTAR_X + 5, CENTAR_Y + 155, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d6.bmp"), 113); // PLAVA
	transDeo(pDC, -CENTAR_X - 7, CENTAR_Y - 149, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d7.bmp"), -125);
	transDeo(pDC, -CENTAR_X - 149, CENTAR_Y + 158, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d8.bmp"), 161);
	transDeo(pDC, -CENTAR_X + 157, CENTAR_Y + 5, CString(L"C:\\Users\\mstoj\\source\\repos\\IND_18922\\bitmap\\d9.bmp"), 107);
}

CPoint CIND18922View::drawDeo(int x, int y, CString nameOfFile, CDC* pDC) {
	DImage* slika = new DImage();
	slika->Load(nameOfFile);

	CBitmap* bmpSlika = slika->m_pBmp;
	CBitmap bmpMask;
	BITMAP bm;

	bmpSlika->GetBitmap(&bm);
	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL); //Inicijalizacija objekta sa bitmapom iz memorije zavisnom od uređaja koja ima navedenu 
																//širinu, visinu i niz bitova

	CDC* SrcDC = new CDC();
	SrcDC->CreateCompatibleDC(pDC);
	CDC* DstDC = new CDC();
	DstDC->CreateCompatibleDC(pDC);

	CBitmap* pOldSrcBmp = SrcDC->SelectObject(bmpSlika); //ucitavanje bitmapa u dc
	CBitmap* pOldDstBmp = DstDC->SelectObject(&bmpMask);

	COLORREF clrTopLeft = SrcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = SrcDC->SetBkColor(clrTopLeft);
	DstDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCCOPY); // copy sve iz scrDC u dstDC, dobija se maska


	// Ako je ime fajla d6.bmp, primeni plavi filter
	if (nameOfFile.Find(L"d6.bmp") != -1) {
		BITMAP bmBlue;
		bmpSlika->GetBitmap(&bmBlue);
		Blue(bmpSlika, bmBlue);
	}
	Gray(bmpSlika, bm);

	COLORREF clrSaveDstText = SrcDC->SetTextColor(RGB(255, 255, 255));
	SrcDC->SetBkColor(RGB(0, 0, 0));
	SrcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, DstDC, 0, 0, SRCAND); // vraca iz dstDC u srcDC ali sa AND i pravi transparent


	pDC->BitBlt(-bm.bmWidth / 2, -bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, DstDC, 0, 0, SRCAND);//maska na pozadinu
	pDC->BitBlt(-bm.bmWidth / 2, -bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCPAINT); //slika na tu pozadinu

	SrcDC->DeleteDC();
	delete SrcDC;
	DstDC->DeleteDC();
	delete DstDC;

	return{ bm.bmWidth, bm.bmHeight };
}

void CIND18922View::transDeo(CDC* pDC, int x, int y, CString fileName, double alpha) {
	this->Mirror(pDC, true, false, true);
	this->Scale(pDC, 0, 0, true);
	this->Translate(pDC, x, y, false);
	this->Rotate(pDC, alpha, false);
	CString nameOfFile1(fileName);
	CPoint imageWidthHeight1 = drawDeo(0, 0, nameOfFile1, pDC);

	XFORM Xform;
	ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_IDENTITY);
}

void CIND18922View::Gray(CBitmap* bmpImage, BITMAP bm)
{

	long dwCount = bm.bmWidthBytes * bm.bmHeight;
	BYTE* lpBits = new BYTE[dwCount];
	ZeroMemory(lpBits, dwCount);
	bmpImage->GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, lpBits);

	for (long i = 0; i < dwCount - 3; i += 3)
	{
		BYTE b = BYTE(lpBits[i]);
		BYTE g = BYTE(lpBits[i + 1]);
		BYTE r = BYTE(lpBits[i + 2]);
		BYTE gray = 64 + (b + g + r) / 3; if (gray > 255) gray = 255; // 0.299 * r + 0.587 * g + 0.114 * b;
		lpBits[i] = gray;
		lpBits[i + 1] = gray;
		lpBits[i + 2] = gray;
	}

	bmpImage->SetBitmapBits(dwCount, lpBits);
	delete[] lpBits;
}
void CIND18922View::Blue(CBitmap* bmpImage, BITMAP bm)
{

	long dwCount = bm.bmWidthBytes * bm.bmHeight;
	BYTE* lpBits = new BYTE[dwCount];
	ZeroMemory(lpBits, dwCount);
	bmpImage->GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, lpBits); // kopira sve piksele iz bmpImage u lpBits

	for (long i = 0; i < dwCount - 3; i += 3)
	{
		lpBits[i] = 0;      // B na 0
		lpBits[i + 1] = 0;  // G na 0
		lpBits[i + 2] = lpBits[i + 2];  // Ostavlja plavi kanal
	}

	bmpImage->SetBitmapBits(dwCount, lpBits);
	delete[] lpBits;
}


void CIND18922View::AntiFlicker(CDC& dc)
{
	// Kreiranje memorijskog DC-ja
	CDC memDC;
	memDC.CreateCompatibleDC(&dc); // Kompatibilan sa glavnim DC-jem

	// Kreiranje kompatibilne bitmape
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc, 500, 500); // Dimenzije bitmape
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap); // Selektovanje bitmape u memorijski DC

	// Iscrtavanje na memorijskom DC-ju
	memDC.FillSolidRect(0, 0, 500, 500, RGB(255, 255, 255)); // Pozadina bele boje
	memDC.TextOut(100, 100, _T("Test Anti-Flicker")); // Tekst

	// Kopiranje sadržaja memorijskog DC-ja u glavni DC prozora
	dc.BitBlt(0, 0, 500, 500, &memDC, 0, 0, SRCCOPY);

	// Vraćanje stare bitmape u memorijski DC
	memDC.SelectObject(pOldBitmap);
}

// CIND18922View printing

BOOL CIND18922View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18922View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18922View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18922View diagnostics

#ifdef _DEBUG
void CIND18922View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18922View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18922Doc* CIND18922View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18922Doc)));
	return (CIND18922Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18922View message handlers


void CIND18922View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{


	if (nChar == VK_SPACE)
	{
		this->keyPressed = !this->keyPressed;
		Invalidate();
	}
	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
