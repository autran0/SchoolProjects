/// \file abort.cpp
/// \brief Code for aborting when something catastrophic goes wrong.
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

#include "abort.h"

#include <stdio.h>
#include <windows.h>

/// \brief The real abort function.
///
/// This is the function that actually really does all of the
/// work of aborting, despite all of the redirections in
/// the code. Normally we will call this function using the ABORT macro.
/// This allows an error message to be displayed in a dialog box (provided things 
/// don't go TOO wrong) last thing on exit by calling the 
/// ABORT macro anyplace in the code. The parameters to this function
/// are identical to those of the stdio function printf.
/// \param fmt Printf style format string.

void reallyAbort(const char *fmt, ...){ 
  char buffer[1024];

  //format the error message into our buffer
  va_list ap;
  va_start(ap, fmt);
  _vsnprintf_s(buffer, sizeof(buffer)-1, fmt, ap);
  va_end(ap);
  //flag the error so the app exits cleanly
  FatalAppExit(0, buffer); //this is the magic Windows API incantation to abort. Thanks, Bill Gates.
} //reallyAbort