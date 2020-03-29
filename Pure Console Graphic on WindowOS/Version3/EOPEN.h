#pragma once
#include "EMATRIX.h"
#include "ECANVAS.h"
#include "EBMP.h"
#include "Window_rendering.h"
#include <thread>

#define WINDOW 0x00
#define MAC  0x01
#define ANDROID  0x02
#define APPLE 0x03

#define BORDERLESS 'B'
#define BASIC 'b'

#define CRED COLORCODE r(255, 0, 0);
#define CGREEN COLORCODE g(0, 255, 0);
#define CBLUE COLORCODE b(0, 0, 255);
#define CWHITE COLORCODE w(255, 255, 255);
#define PIXEL 'P'
#define LINE 'L'

//w - set the window width 
//h - set the window height
//cw - set the canvas size(defualt (-1), as smae as w)
//ch - set the canvas size(defualt (-1), as same as h)  
//OS - WINDOW, MAC, ANDROID, APPLE
//TYPE : BORDERLESS, BASIC
CANVAS* OPENCLSET(string title, int w, int h, int cw, int ch, char OS, const char TYPE);
//The canvas is based on lower left corner (0,0)
//Pixel is the size of pixel of bmp (it is not dpi)
//default pixel size is 1
void OPENCL_BMP(CANVAS* canvas, string file_name, int x, int y, int pixel);
//BASE ON BMP FILE DRAWING
void GRAPHICDISPLAYER(CANVAS& canvas);
//CLEAR canvas then display
void GRAPHICDISCLEAR(CANVAS& canvas);
//SET THE STATIC MAIN CANVAS
CANVAS* SETCANVASSIZE(int w, int h);
//OS : WINDOW, MAC, ANDROID, APPLE
//TYPE : BORDERLESS, BASIC
void SETCONSOLESIZE(string title, int w, int h, char OS, const char TYPE);
//THE COLUMN OF A MUST BE 2
//************ TYPE  *************************
//PIXEL: pixel type m x 2 matrix is required.
//LINE: drawing line m x 4 matrix is required.
void OPENCL_MATRIX(CANVAS* canvas, EMATRIX A, COLORCODE b, const char TYPE);
//************ MODE  **************************
//Fx, Fy : SCALING
//CX,CY : Optical Centre
//Tx,Ty,Tz : Camera Translation
EMATRIX* SETCAMERA(float fx, float fy, float cx, float cy, float rx, float ry, float rz, float tx, float ty, float tz);
EMATRIX* CAMERA_MATRIX(EMATRIX CAMERA, EMATRIX A);