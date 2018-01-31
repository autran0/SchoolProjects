/// \file IPMgr.h 
/// \brief Interface for the IPManager class.
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

#include <winsock.h>

#define WINSOCK_VERSION MAKEWORD(1,1) ///< Winsock version. Use MAKEWORD(x,y) for version x.y

/// Winsock error description structure.

struct WSAEDESCRIPTION{
  int nErrorCode; ///< Error code.
  char *szDescription; ///< Error string.
}; //WSAEDESCRIPTION

/// \brief The IP manager class.
///
/// This class manages UDP/IP error reporting.
/// The IP manager is intended only to be used by the debug manager for sending
/// error strings to a debug client.

class IPManager{
  private:
    BOOL m_bInitialized; ///< TRUE if initialized OK.
    SOCKADDR_IN m_saClient; ///< Client socket info.
    SOCKET m_Socket; ///< The socket.

  public:
    IPManager(char* addr, int port); ///< Constructor.
    virtual ~IPManager(); ///< Destructor.
    BOOL SendPacket(char* message, int length); ///< Send packet.
    LPCTSTR WinsockErrorDescription(int nErrorCode); ///< Describe WS error message.
}; //IPManager
