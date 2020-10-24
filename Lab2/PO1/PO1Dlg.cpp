
// PO1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "PO1Dlg.h"
#include "ParamsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IMG_WND_ID_IN	100
#define IMG_WND_ID_OUT	101


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPODlg dialog



CPODlg::CPODlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo1);
	DDX_Control(pDX, IDC_SLIDER1, SliderControl);
}

BEGIN_MESSAGE_MAP(CPODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CPODlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CPODlg::OnBnClickedButtonProcess)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPODlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PARAMS, &CPODlg::OnBnClickedButtonParams)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CPODlg::OnNMReleasedcaptureSlider1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPODlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CPODlg message handlers

BOOL CPODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rDlg(7, 7, 407, 407);
	MapDialogRect(rDlg);

	m_imgIN.Create(rDlg, this, IMG_WND_ID_IN);

	rDlg = CRect(530, 7, 930, 407);
	MapDialogRect(rDlg);

	m_imgOUT.Create(rDlg, this, IMG_WND_ID_OUT);

	m_combo1.AddString(L"convert to greyscale");
	m_combo1.AddString(L"zmiana jasnosci");
	m_combo1.AddString(L"zmiana kontrastu");
	m_combo1.AddString(L"zmiana potegi");
	m_combo1.AddString(L"negatyw");
	m_combo1.AddString(L"histogram 8b");
	m_combo1.AddString(L"normalized histogram 8b");
	m_combo1.SelectString(0, L"convert to greyscale");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPODlg::OnBnClickedButtonLoad()
{
	WCHAR strFilter[] = { L"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||" };

	CFileDialog FileDlg(TRUE, NULL, NULL, 0, strFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		CString path = FileDlg.GetPathName();
		m_imgIN.image.LoadDIB(path);
		InvalidateRect(NULL);
		//MessageBox(L"Now what?", L";-)", MB_OK);
	}


}


void CPODlg::OnBnClickedButtonProcess()
{
	CString sOption;
	m_combo1.GetLBText(m_combo1.GetCurSel(), sOption);

	if (sOption == L"convert to greyscale")
	{
		/*        */
		m_imgOUT.image.CreateGreyscaleDIB(m_imgIN.image.z, m_imgIN.image.bitMapInfoHeader->biXPelsPerMeter, m_imgIN.image.bitMapInfoHeader->biYPelsPerMeter);
		//InvalidateRect(NULL);
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 24) {
					RGBTRIPLE pixel = m_imgIN.image.GetPixel24(j, i);
					new_pixel = 0.299 * pixel.rgbtRed + 0.587 * pixel.rgbtGreen + 0.114 * pixel.rgbtBlue;
				}
				else if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
				}
				else if (m_imgIN.image.bitMapInfoHeader->biBitCount == 1) {
					new_pixel = m_imgIN.image.GetPixel1(j, i) ? (byte)255 : (byte)0;
				}

				m_imgOUT.image.SetPixel8(j, i, new_pixel);

			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"zmiana jasnosci")
	{
		/*        */
		m_imgOUT.image.CreateGreyscaleDIB(m_imgIN.image.z, m_imgIN.image.bitMapInfoHeader->biXPelsPerMeter, m_imgIN.image.bitMapInfoHeader->biYPelsPerMeter);
		//InvalidateRect(NULL);
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				int newValue = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
					newValue = int(new_pixel) + this->sliderValue;
					if (newValue > 255) {
						newValue = 255;
					}
					else if (newValue < 0) {
						newValue = 0;
					}
					new_pixel = byte(newValue);
					
				}

				m_imgOUT.image.SetPixel8(j, i, new_pixel);

			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"zmiana kontrastu")
	{
		/*        */
		m_imgOUT.image.CreateGreyscaleDIB(m_imgIN.image.z, m_imgIN.image.bitMapInfoHeader->biXPelsPerMeter, m_imgIN.image.bitMapInfoHeader->biYPelsPerMeter);
		//InvalidateRect(NULL);
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				double newValue = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
					newValue = double(new_pixel) - 127;
					newValue = double(newValue) * this->sliderValue/(double)(10.0);
					newValue = newValue + 127;
					if (newValue > 255) {
						newValue = 255;
					}
					else if (newValue < 0) {
						newValue = 0;
					}
					new_pixel = byte(newValue);
				}

				m_imgOUT.image.SetPixel8(j, i, new_pixel);

			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"zmiana potegi")
	{
		/*        */
		byte max = 0;
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
					if (new_pixel > max) {
						max = new_pixel;
					}
				}
			}
		}
		m_imgOUT.image.CreateGreyscaleDIB(m_imgIN.image.z, m_imgIN.image.bitMapInfoHeader->biXPelsPerMeter, m_imgIN.image.bitMapInfoHeader->biYPelsPerMeter);
		//InvalidateRect(NULL);
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				double newValue = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
					newValue = 255 * pow(double(new_pixel)/double(max), this->sliderValue/double(10.0));
					if (newValue > 255) {
						newValue = 255;
					}
					else if (newValue < 0) {
						newValue = 0;
					}
					new_pixel = byte(newValue);
				}

				m_imgOUT.image.SetPixel8(j, i, new_pixel);

			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"negatyw")
	{
		/*        */
		m_imgOUT.image.CreateGreyscaleDIB(m_imgIN.image.z, m_imgIN.image.bitMapInfoHeader->biXPelsPerMeter, m_imgIN.image.bitMapInfoHeader->biYPelsPerMeter);
		//InvalidateRect(NULL);
		for (int i = 0; i < m_imgIN.image.bitMapInfoHeader->biHeight; i++) {
			for (int j = 0; j < m_imgIN.image.bitMapInfoHeader->biWidth; j++) {
				byte new_pixel = 0;
				double newValue = 0;
				if (m_imgIN.image.bitMapInfoHeader->biBitCount == 8) {
					new_pixel = m_imgIN.image.GetPixel8(j, i);
					newValue = 255 - (double)new_pixel;
					if (newValue > 255) {
						newValue = 255;
					}
					else if (newValue < 0) {
						newValue = 0;
					}
					new_pixel = byte(newValue);
				}

				m_imgOUT.image.SetPixel8(j, i, new_pixel);

			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"histogram 8b")
	{
		if (m_imgIN.image.drawHistogram) {
			DialogHistogram hist;
			hist.SetImage(&m_imgIN.image);
			hist.SetWitch(BitMapImage::DrawWitch::normal);
			hist.DoModal();
		}
		else {
			if (m_imgIN.image.Histogram()) {
				DialogHistogram hist;
				hist.SetImage(&m_imgIN.image);
				hist.SetWitch(BitMapImage::DrawWitch::normal);
				hist.DoModal();
			}
		}

		InvalidateRect(NULL);
	}
	if (sOption == L"normalized histogram 8b")
	{
		if (m_imgIN.image.drawHistogram) {
			if (m_imgIN.image.normalizedHistogramDraw) {
				DialogHistogram hist;
				hist.SetImage(&m_imgIN.image);
				hist.SetWitch(BitMapImage::DrawWitch::normalized);
				hist.DoModal();
			}
			else {
				m_imgIN.image.NormalizeHistogram();
				DialogHistogram hist;
				hist.SetImage(&m_imgIN.image);
				hist.SetWitch(BitMapImage::DrawWitch::normalized);
				hist.DoModal();
			}
		}
		else {
			if (m_imgIN.image.Histogram()) {
				m_imgIN.image.NormalizeHistogram();
				DialogHistogram hist;
				hist.SetImage(&m_imgIN.image);
				hist.SetWitch(BitMapImage::DrawWitch::normalized);
				hist.DoModal();
			}
		}



		InvalidateRect(NULL);
	}

}


void CPODlg::OnBnClickedButtonSave()
{
	WCHAR strFilter[] = { L"Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||" };

	CFileDialog FileDlg(FALSE, NULL, NULL, 0, strFilter);

	if (FileDlg.DoModal() == IDOK)
	{
		//MessageBox(L"Now what?", L";-)");
		CString path = FileDlg.GetPathName();
		m_imgOUT.image.SaveDIB(path);
	}
}


void CPODlg::OnBnClickedButtonParams()
{
	CParamsDlg paramsDlg;
	CString s;

	if (paramsDlg.DoModal() == IDOK)
	{
		s = paramsDlg.m_sParams;
	}
}


void CPODlg::OnNMReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	this->sliderValue = this->SliderControl.GetPos();
}


void CPODlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	CString sOption;
	m_combo1.GetLBText(m_combo1.GetCurSel(), sOption);

	if (sOption == L"zmiana jasnosci") {
		this->SliderControl.SetRange(-100, 100, 1);
		this->SliderControl.SetPos(0);
	}
	if (sOption == L"zmiana potegi") {
		this->SliderControl.SetRange(1, 30, 1);
		this->SliderControl.SetPos(10);
	}
	if (sOption == L"zmiana kontrastu") {
		this->SliderControl.SetRange(1, 50, 1);
		this->SliderControl.SetPos(10);
	}

}
