#pragma once
#include"BitMapImage.h";

// DialogHistogram dialog

class DialogHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(DialogHistogram)

public:
	DialogHistogram(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DialogHistogram();
	BitMapImage* image;
	bool imageSet = false;
	BitMapImage::DrawWitch witch;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void SetImage(BitMapImage* image);
	void SetWitch(BitMapImage::DrawWitch witch);
};
