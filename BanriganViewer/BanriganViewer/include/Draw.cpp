#include "StdAfx.h"
#include "Draw.h"


CDraw::CDraw(void)
{

}


CDraw::~CDraw(void)
{
}

void CDraw::DrawImage(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, bool bCross)
{
	if (pWnd == NULL) return;
	if (pBuffer == NULL) return;
	if (pBmpInfo == NULL) return;

	CClientDC dc(pWnd);

	CRect rect;
	pWnd->GetClientRect(&rect);

	// ----- 더블 버퍼링 준비 -----//
	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(&dc);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // 흰색으로 초기화

	// ----- 지정된 영역에 본격적으로 그리기 시작 ----- //
	
	SetStretchBltMode(memDC.GetSafeHdc(), HALFTONE); 
	StretchDIBits(memDC.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, nWidth, nHeight, pBuffer, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	if (bCross == true)
	{
		CPen pen, *pOldPen;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		pOldPen = (CPen*)memDC.SelectObject(&pen);
		memDC.MoveTo(0, rect.Height()/2);
		memDC.LineTo(rect.Width(), rect.Height()/2);
		memDC.MoveTo(rect.Width()/2, 0);
		memDC.LineTo(rect.Width()/2, rect.Height());

		memDC.SelectObject(pOldPen);
		pen.DeleteObject();
	}

	// ----- 그려진 메모리를 최종 디스플레이 -----//

	// 메모리 DC를 화면 DC에 고속 복사
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CDraw::DrawMeasureImage(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, float fPocX, float fPocY, bool bCross)
{
	if (pWnd == NULL) return;
	if (pBuffer == NULL) return;
	if (pBmpInfo == NULL) return;

	CClientDC dc(pWnd);

	CRect rect;
	pWnd->GetClientRect(&rect);

	// ----- 더블 버퍼링 준비 -----//
	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(&dc);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // 흰색으로 초기화

	// ----- 지정된 영역에 본격적으로 그리기 시작 ----- //

	SetStretchBltMode(memDC.GetSafeHdc(), HALFTONE); 
	StretchDIBits(memDC.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, nWidth, nHeight, pBuffer, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	CPen *pOldPen;
	if (bCross == true)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		pOldPen = (CPen*)memDC.SelectObject(&pen);
		memDC.MoveTo(0, rect.Height()/2);
		memDC.LineTo(rect.Width(), rect.Height()/2);
		memDC.MoveTo(rect.Width()/2, 0);
		memDC.LineTo(rect.Width()/2, rect.Height());

		memDC.SelectObject(pOldPen);
		pen.DeleteObject();
	}

	float fH = (float)rect.Width() / nWidth;
	float fV = (float)rect.Height() / nHeight;

	float centerX = ((float)nWidth/2-1)+(fPocX);
	float centerY = ((float)nHeight/2-1)-(fPocY);

	float x = rect.Width() * centerX / 640;
	float y = rect.Height() * centerY / 480;

	CPen penPoc;
	penPoc.CreatePen(PS_SOLID,1,RGB(0,255,255));
	pOldPen = (CPen*)memDC.SelectObject(&penPoc);
	memDC.MoveTo(x-5, y);
	memDC.LineTo(x+5, y);
	memDC.MoveTo(x, y-5);
	memDC.LineTo(x, y+5);

	memDC.SelectObject(pOldPen);
	penPoc.DeleteObject();

	// ----- 그려진 메모리를 최종 디스플레이 -----//

	// 메모리 DC를 화면 DC에 고속 복사
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CDraw::DrawImageWithROI(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, int nOrgX, int nOrgY)
{
	if (pWnd == NULL) return;
	if (pBuffer == NULL) return;
	if (pBmpInfo == NULL) return;

	CClientDC dc(pWnd);

	CRect rect;
	pWnd->GetClientRect(&rect);

	// ----- 더블 버퍼링 준비 -----//
	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(&dc);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // 흰색으로 초기화

	// ----- 지정된 영역에 본격적으로 그리기 시작 ----- //

	SetStretchBltMode(memDC.GetSafeHdc(), HALFTONE); 
	StretchDIBits(memDC.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, nWidth, nHeight, pBuffer, pBmpInfo, DIB_RGB_COLORS, SRCCOPY);

	float fH = (float)rect.Width() / nWidth;
	float fV = (float)rect.Height() / nHeight;

	CBrush brsPoc(RGB(255,0,0));
	memDC.FrameRect(CRect(nOrgX*fH,nOrgY*fV,(nOrgX+128)*fH,(nOrgY+128)*fV),&brsPoc);

	// ----- 그려진 메모리를 최종 디스플레이 -----//

	// 메모리 DC를 화면 DC에 고속 복사
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}