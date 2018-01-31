/// \file WindowClass.h
/// \brief Interface for the window class CWindow.
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

#pragma once

#include <Windows.h>

#include "Defines.h"

/// \brief The window class. 
///
/// The game class encapsulates a collection of Windows-related
/// global variables.

class CWindow{
  private:
    HWND m_Hwnd; ///< Application window handle.
    char m_szName[256]; ///< Name of this program.
    int m_nWidth; ///< Window width in pixels.
    int m_nHeight; ///< Window height in pixels.
    BOOL m_bActive;  ///< TRUE if this is the active application.

  public:
    HWND CreateDefaultWindow(HINSTANCE hInstance, int nCmdShow); ///< Create default window.
    HWND GetHwnd(); ///< Get window handle.

    void SetName(const char* s); ///< Set the game name.
    char* GetName(); ///< Get the game name.

    int GetWidth(); ///< Get window width.
    int GetHeight(); ///< Get window height.
    void GetSize(int& w, int& h); ///< Get window width and height.
    Vector2 GetSize(); ///< Get window width and height.
    void SetSize(const int w, const int h); ///< Set window width and height.

    void SetActive(const BOOL b); ///< Set window active flag.
    BOOL GetActive(); ///< Get window active flag.
}; //CWindow
