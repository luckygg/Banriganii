#pragma once
class CDraw
{
public:
	CDraw(void);
	~CDraw(void);

public :
	static void DrawImage(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, bool bCross);
	static void DrawMeasureImage(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, float fPocX, float fPocY, bool bCross);
	static void DrawImageWithROI(CWnd* pWnd, BYTE* pBuffer, BITMAPINFO* pBmpInfo, int nWidth, int nHeight, int nOrgX, int nOrgY);

private :
	
};

