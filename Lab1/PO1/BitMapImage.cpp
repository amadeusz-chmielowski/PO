#include "stdafx.h"
#include "BitMapImage.h"

BitMapImage::BitMapImage()
{
	this->bitMapFileHeader = (BITMAPFILEHEADER*)(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPFILEHEADER)));
	this->bitMapInfoHeader = (BITMAPINFOHEADER*)(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER)));
	this->bitMapInfo = (BITMAPINFO*)(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFO)));
	this->rest = nullptr;
	this->restSize = 0;
	this->pallete = new RGBQUAD[256];
}

BitMapImage::~BitMapImage()
{
	HeapFree(GetProcessHeap(), 0, bitMapFileHeader);
	HeapFree(GetProcessHeap(), 0, rest);

}

bool BitMapImage::LoadDIB(CString file_path)
{

	this->file.Open(file_path, CFile::modeReadWrite | CFile::typeBinary);
	file.Read(bitMapFileHeader, sizeof(BITMAPFILEHEADER));
	restSize = file.GetLength() - sizeof(BITMAPFILEHEADER);
	rest = (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, restSize));
	file.Read(rest, restSize);
	this->file.Close();
	image_out = ((char*)rest + bitMapFileHeader->bfOffBits - sizeof(BITMAPFILEHEADER));
	this->bitMapInfo = (BITMAPINFO*)rest;
	bitMapInfoHeader = (BITMAPINFOHEADER*)rest;
	z = new CRect(0, 0, bitMapInfoHeader->biHeight, bitMapInfoHeader->biWidth);
	return true;

}

bool BitMapImage::PaintDIB(HDC kontekst, CRect prost_docelowy, CRect prost_zrodlowy)
{

	SetStretchBltMode(kontekst, COLORONCOLOR);
	StretchDIBits(kontekst, 0, 0, prost_docelowy.Width(), prost_docelowy.Height(), 0, 0, bitMapInfoHeader->biWidth, bitMapInfoHeader->biHeight, this->image_out, this->bitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	return true;
}

bool BitMapImage::CreateGreyscaleDIB(CRect rozmiar_obrazu, int xPPM, int yPPM)
{
	bitMapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	bitMapInfoHeader->biWidth = rozmiar_obrazu.Height();
	bitMapInfoHeader->biHeight = rozmiar_obrazu.Width();
	bitMapInfoHeader->biPlanes = 1;
	bitMapInfoHeader->biBitCount = 8;
	bitMapInfoHeader->biCompression = 0;
	bitMapInfoHeader->biSizeImage = rozmiar_obrazu.Width() * rozmiar_obrazu.Height();
	bitMapInfoHeader->biXPelsPerMeter = xPPM;
	bitMapInfoHeader->biYPelsPerMeter = yPPM;
	bitMapInfoHeader->biClrUsed = 256;
	bitMapInfoHeader->biClrImportant = 0;

	bitMapFileHeader->bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + sizeof(RGBQUAD) * 256;
	bitMapFileHeader->bfReserved1 = 0;
	bitMapFileHeader->bfReserved2 = 0;
	bitMapFileHeader->bfSize = bitMapFileHeader->bfOffBits + bitMapInfoHeader->biSizeImage;//sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) + bitMapInfoHeader->biSizeImage;
	bitMapFileHeader->bfType = 0x4d42;

	for (int i = 0; i < 256; ++i) {
		this->pallete[i].rgbRed = (byte)i;
		this->pallete[i].rgbGreen = (byte)i;
		this->pallete[i].rgbBlue = (byte)i;
		this->pallete[i].rgbReserved = (byte)0;
	}

	restSize = sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)) + bitMapInfoHeader->biSizeImage;
	rest = (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, restSize));
	this->image_out = (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bitMapInfoHeader->biSizeImage));
	std::memcpy(rest, bitMapInfoHeader, sizeof(BITMAPINFOHEADER));
	std::memcpy((char*)rest + sizeof(BITMAPINFOHEADER), pallete, 256 * sizeof(RGBQUAD));
	std::memcpy((char*)rest + sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)), image_out, bitMapInfoHeader->biSizeImage);
	this->bitMapInfo = (BITMAPINFO*)rest;
	z = new CRect(0, 0, bitMapInfoHeader->biHeight, bitMapInfoHeader->biWidth);
	return true;
}

bool BitMapImage::SaveDIB(CString sciezka_do_pliku)
{
	HANDLE hFile = CreateFile(sciezka_do_pliku, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//file_.Open(sciezka_do_pliku, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
	if (!hFile) // return if error opening file
	{
		return false;
	}
	DWORD dwWritten = 0;
	WriteFile(hFile, bitMapFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(hFile, bitMapInfoHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	WriteFile(hFile, &pallete[0], 256 * sizeof(RGBQUAD), &dwWritten, NULL);
	if (bitMapInfoHeader->biWidth % 4 == 0)
	{
		WriteFile(hFile, image_out, bitMapInfoHeader->biSizeImage, &dwWritten, NULL);
	}
	//else
	//{
	//	char* empty = new char[4 - bitMapInfoHeader->biWidth % 4];
	//	for (int i = 0; i < bitMapInfoHeader->biHeight; ++i)
	//	{
	//		LONG lWidth = bitMapInfoHeader->biWidth;
	//		//file_.Write((void*)(image_out[i*bitMapInfoHeader->biWidth]), bitMapInfoHeader->biWidth);
	//		WriteFile(hFile, &image_out[i * lWidth], lWidth, &dwWritten, NULL);
	//		WriteFile(hFile, empty, 4 - lWidth % 4, &dwWritten, NULL);
	//	}
	//}
	CloseHandle(hFile);
	return true;
}

RGBTRIPLE BitMapImage::GetPixel24(int x, int y)
{
	RGBTRIPLE colour;
	int byte_number = ((24 * this->bitMapInfoHeader->biWidth + 31) / 32) * 4;
	int offset_x = x * 3;
	int offset_y = (y)*byte_number;
	char* pixel_offset = (char*)image_out + offset_x + offset_y;
	colour.rgbtBlue = (byte)(pixel_offset[0]);
	colour.rgbtGreen = (byte)(pixel_offset[1]);
	colour.rgbtRed = (byte)(pixel_offset[2]);

	return colour;
}

bool BitMapImage::GetPixel1(int x, int y)
{
	int byte_number = ((1 * this->bitMapInfoHeader->biWidth + 31) / 32) * 4;
	int byte_offset = int(x / 8);
	char* pixel_offset = (char*)image_out + byte_offset + (y * byte_number);
	uint8_t byte_value = pixel_offset[0];
	int bool_offset = 7-(x % 8);
	bool value = ((byte_value >> (bool_offset)) & 0x01);
	return value;
}

BYTE BitMapImage::GetPixel8(int x, int y)
{
	int byte_number = ((8 * this->bitMapInfoHeader->biWidth + 31) / 32) * 4;
	char* pixel_offset = (char*)image_out + x + (y * byte_number);
	return pixel_offset[0];
}

bool BitMapImage::SetPixel8(int x, int y, BYTE val)
{
	int byte_number = ((8 * this->bitMapInfoHeader->biWidth + 31) / 32) * 4;
	void* pixel_offset = (char*)image_out + x + (y * byte_number);
	std::memcpy(pixel_offset, &val, sizeof(byte));
	return true;
}
