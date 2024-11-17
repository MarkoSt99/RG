
// IND_18922View.h : interface of the CIND18922View class
//

#pragma once


class CIND18922View : public CView
{
protected: // create from serialization only
	CIND18922View() noexcept;
	DECLARE_DYNCREATE(CIND18922View)

	// Attributes
public:
	bool keyPressed;
	int gridSize;
	bool showGrid = false; // Da li je mreža uključena
	bool stanje = 1;
	CIND18922Doc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	void Photo(CDC* pDC);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	CPoint drawDeo(int x, int y, CString nameOfFile, CDC* pDC);
	void transDeo(CDC* pDC, int x, int y, CString fileName, double alpha);
	void Grid(CDC* pDC, int width, int height, bool showGrid);
	int width = 500;
	int height = 500;

	
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);

	void Gray(CBitmap* bmpImage, BITMAP bm);

	void Blue(CBitmap* bmpImage, BITMAP bm);

	void AntiFlicker(CDC& dc);
	
	CImage bmpImage;

	// Implementation
public:
	virtual ~CIND18922View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18922View.cpp
inline CIND18922Doc* CIND18922View::GetDocument() const
{
	return reinterpret_cast<CIND18922Doc*>(m_pDocument);
}
#endif

