#pragma once
#include<fstream>
#include<iostream>
class BitMapImage
{
public:

	BitMapImage();
	~BitMapImage();

	bool LoadDIB(CString file_path);
	bool PaintDIB(HDC kontekst, CRect prost_docelowy, CRect prost_zrodlowy);
	bool CreateGreyscaleDIB(CRect rozmiar_obrazu, int xPPM, int yPPM);
	bool SaveDIB(CString sciezka_do_pliku);
	RGBTRIPLE GetPixel24(int x, int y);
	bool GetPixel1(int x, int y);
	BYTE GetPixel8(int x, int y);
	bool SetPixel8(int x, int y, BYTE val);
	CRect z;
	BITMAPFILEHEADER* bitMapFileHeader;
	BITMAPINFOHEADER* bitMapInfoHeader;
	BITMAPINFO* bitMapInfo;
private:
	void* rest;
	UINT32 restSize;
	CFile file;
	RGBQUAD *pallete;
	void* image_out;
	bool loaded;

};

