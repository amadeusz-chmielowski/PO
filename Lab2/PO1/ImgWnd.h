#pragma once
#include "BitMapImage.h"

// CImgWnd

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	BitMapImage image;
	virtual ~CImgWnd();

	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);

protected:
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


