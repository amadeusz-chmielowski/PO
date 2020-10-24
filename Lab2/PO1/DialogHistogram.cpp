// DialogHistogram.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "DialogHistogram.h"
#include "afxdialogex.h"


// DialogHistogram dialog

IMPLEMENT_DYNAMIC(DialogHistogram, CDialogEx)

DialogHistogram::DialogHistogram(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_HISTOGRAM, pParent)
{
	this->image = nullptr;
	this->witch = BitMapImage::DrawWitch::normal;
}

DialogHistogram::~DialogHistogram()
{
}

void DialogHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogHistogram, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DialogHistogram message handlers


void DialogHistogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect r;
	GetClientRect(r);
	HDC kontekst = dc.GetSafeHdc();
	if (this->imageSet) {
		image->DrawHistogram(kontekst, r, this->witch);
	}


}


BOOL DialogHistogram::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(NULL, 0, 0, 720, 720, SWP_NOMOVE | SWP_NOZORDER);


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void DialogHistogram::SetImage(BitMapImage* image)
{
	this->image = image;
	this->imageSet = true;
}

void DialogHistogram::SetWitch(BitMapImage::DrawWitch witch)
{
	this->witch = witch;
}
