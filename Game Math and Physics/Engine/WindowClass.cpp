/// \file WindowClass.cpp
/// \brief Code for the window class CWindow.
///
/// Created by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D", published by CRC Press in 2013.
/// Copyright Ian Parberry, Laboratory for Recreational Computing,
/// Department of Computer Science & Engineering, University of North
/// Texas, Denton, TX, USA. URL: http://larc.unt.edu/ian.
///
/// This file is made available under the GNU All-Permissive License.
/// Copying and distribution of this file, with or without
/// modification, are permitted in any medium without royalty
/// provided the copyright notice and this notice are preserved.
/// This file is offered as-is, without any warranty.

#include "WindowClass.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

/// Register and create a window. Care is taken to ensure that the
/// client area of the window is the right size, because the default
/// way of creating a window has you specify the width and height
/// including the frame.
///  \param hInstance handle to the current instance of this application
///  \param nCmdShow specifies how the window is to be shown
///  \return handle to the application window

HWND CWindow::CreateDefaultWindow(HINSTANCE hInstance, int nCmdShow){
  WNDCLASS wc; //window registration info

  //fill in registration information wc
  wc.style = CS_HREDRAW | CS_VREDRAW; //style
  wc.lpfnWndProc = WindowProc; //window message handler
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.hInstance = hInstance; //instance handle
  wc.hIcon = nullptr; //default icon
  wc.hCursor = nullptr; //no cursor
  wc.hbrBackground = nullptr; //we will draw background
  wc.lpszMenuName = nullptr; //no menu
  wc.lpszClassName = m_szName; //game name
            
  RegisterClass(&wc); //register window

  //create and set up window 
  RECT r;  
  r.left = 0; r.right = m_nWidth; 
  r.top = 0; r.bottom = m_nHeight;

  DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_SYSMENU; 
  DWORD dwStyleEx = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME;

  AdjustWindowRectEx(&r, dwStyle, FALSE, dwStyleEx); 

  m_Hwnd = CreateWindowEx(dwStyleEx, m_szName, m_szName, dwStyle, 0, 0,
    r.right-r.left, r.bottom-r.top, nullptr, nullptr, hInstance, nullptr);

  //center window on screen
  int x = (GetSystemMetrics(SM_CXSCREEN) - m_nWidth)/2;
  int y = (GetSystemMetrics(SM_CYSCREEN) - m_nHeight)/2;
  ::SetWindowPos(m_Hwnd, nullptr, x, y, r.right - r.left, r.bottom - r.top,
    SWP_NOZORDER | SWP_SHOWWINDOW);

  //This is what we'd have to do to make it fullscreen instead of
  //windowed, which of course we are not. Just FYI.
  //hwnd = CreateWindowEx(WS_EX_TOPMOST, m_szName, m_szName, WS_POPUP, 0, 0,
  //  GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
  //  nullptr, nullptr, hInstance, nullptr);

  if(m_Hwnd){ //if successfully created window
    ShowWindow(m_Hwnd, nCmdShow); 
    UpdateWindow(m_Hwnd); 
    SetFocus(m_Hwnd); //bind keyboard
  } //if

  return m_Hwnd; //return window handle
} //CreateDefaultWindow

/// \return Current window handle.

HWND CWindow::GetHwnd(){
  return m_Hwnd;
} //GetHwnd

/// \param s String containing the name of this game.

void CWindow::SetName(const char* s){
  strncpy_s(m_szName, strlen(s)+1, s, 256); 
} //SetName

/// Reader function for game name.
/// \return Pointer to name.

char* CWindow::GetName(){
  return m_szName;
} //GetName

 /// Reader function for m_nHWidth
 /// \return Window height.

int CWindow::GetWidth(){
  return m_nWidth;
} //GetWidth

/// Reader function for m_nHeight.
/// \return Window height.

int CWindow::GetHeight(){
  return m_nHeight;
} //GetHeight

void CWindow::SetSize(const int w, const int h){
  m_nWidth = w;
  m_nHeight = h;
} //SetSize

/// Reader function for window width and height.

void CWindow::GetSize(int& w, int& h){
  w = m_nWidth;
  h = m_nHeight;
} //SetSize

Vector2 CWindow::GetSize(){
  return Vector2((float)m_nWidth, (float)m_nHeight);
} //GetSize

void CWindow::SetActive(const BOOL b){
  m_bActive = b;
} //SetActive

/// Reader function for window active flag.

BOOL CWindow::GetActive(){
  return m_bActive;
} //GetActive