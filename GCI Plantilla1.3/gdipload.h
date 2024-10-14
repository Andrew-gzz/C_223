#ifndef gdipload_HEADER
#define gdipload_HEADER

#include<Windows.h>
#include<GdiPlus.h>


using namespace Gdiplus;

struct miBitmap{

	int ancho;
	int alto;
	unsigned int* Direction;
	int *pixeles;

};

inline miBitmap gdipLoad(WCHAR nombre[])
{
	miBitmap mbmp;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR  gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap *bitmap=new Bitmap(nombre);
	BitmapData *bitmapData=new BitmapData;

	mbmp.ancho = bitmap->GetWidth();
	mbmp.alto = bitmap->GetHeight();

	Rect rect(0, 0, mbmp.ancho, mbmp.alto);

	bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppRGB, bitmapData);

	int* pixels = (int*)bitmapData->Scan0;

	mbmp.pixeles = new int[mbmp.alto*mbmp.ancho];

	mbmp.Direction = new unsigned int[mbmp.ancho * mbmp.alto * 4];

	//tenemos que invertir el blue por el red
	//el green se conserva en posicion
	for (unsigned int i = 0; i < (mbmp.alto * mbmp.ancho); i += 4)
	{
		mbmp.Direction[i + 2] = pixels[i];
		mbmp.Direction[i + 1] = pixels[i + 1];
		mbmp.Direction[i] = pixels[i + 2];
		mbmp.Direction[i + 3] = pixels[i + 3];
	}

	for(int i=0, j=mbmp.alto*mbmp.ancho;i<j;i++)
	{
		mbmp.pixeles[i]=pixels[i];
	}

	bitmap->UnlockBits(bitmapData);

	delete bitmapData;
	delete bitmap;
	  
	GdiplusShutdown(gdiplusToken);

	return mbmp;
}
#endif