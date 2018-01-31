/// \file abort.h
/// \brief Header and define for the ABORT macro.
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

/// Terminate the program with a printf-like formatted error message. 
/// Normally we will call this function using the ABORT macro.

void reallyAbort(const char *fmt, ...);

/// \brief The ABORT macro.
///
/// This nasty looking macro uses a handy little trick to allow it to
/// appear to to take a variable number of arguments. This is what
/// you should actually call to abort.

#define ABORT (reallyAbort)
