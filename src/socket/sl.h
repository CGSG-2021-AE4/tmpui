/*******************
 * PURPOSE: Socket lib
 * FILE: Main handle file
 * PROGRAMMER: AE4
 * DATE: 04.05.23
 *******************/

#ifndef __SL_H_
#define __SL_H_

#define MAX_Q_LEN 10

#define MAX_MESSAGE_LEN 1024

#include <cassert>
#include <string>
#include <iostream>
#include <optional>
#include <thread>
#include <set>
#include <vector>
#include <map>
#include <functional>
#include <span>
#include <format>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

/* Socket lib namespace */
namespace sl
{
  /* Socket class */
  class sock
  {
  private:

    static BOOL IsInit;                        // Is sockets system initialised
    std::optional<std::thread> AcceptorThread; // Thread for accept clients to server

  public:

    /* Log static inline function */
    std::function<VOID( const std::string &Str )> Log = []( const std::string &Str )
    {
      std::cout << "SOCKET LOG: " << Str << std::endl;
    };

    /* Sockets system static init function.
     * ARUGMENTS: None.
     * RETURNS: None.
     */
    static VOID Init( VOID )
    {
      if (IsInit)
        return;

      // Init

      WSADATA Info;

      if (WSAStartup(MAKEWORD(2,0), &Info))
        throw "Could not start WSA";
      else
        IsInit = 1;

    } /* End of 'Init' function */

    /* Sockets system static close function.
     * ARUGMENTS: None.
     * RETURNS: None.
     */
    static VOID Close( VOID )
    {
      if (!IsInit)
        return;

      // Close

      WSACleanup();
      IsInit = 0;
    } /* End of 'Close' function */

    /* Log error static inline function.
     * ARGUMENTS:
     *   - log string:
     *       const std::string &str;
     * RETURNS: None.
     */
    inline VOID LogError( const std::string &Str )
    {
      Log("ERROR: " + Str);
    } /* End of 'Log' function */

  public:

    /* Type of port enum*/
    enum struct type
    {
      SERVER,
      CLIENT, 
    }; /* End of 'type' enum struct */

    /* Type of communication enum */
    enum struct com_type
    {
      TCP, // After sending message we are waiting for corfirming getting
      UDP, // We aren't waiting
    }; /* End of 'com_type' enum struct */

  private:

    SOCKET hSocket {(SOCKET)-1};                 // System socket handle
    std::map<SOCKET, std::thread> ReaderThreads; // Threads with readers

    BOOL IsListenFlag {false}; // Is listening flag

    // Server
    INT ActivePort {-1};          // Active port number
    std::set<SOCKET> Connections; // Conected client socket

    // Client
    BOOL IsConnectedFlag {0};

  public:

    const type Type; // This socket type

    /* Construct function.
     * ARGUMENTS:
     *   - type:
     *       const type NewType;
     *   - communication type:
     *       const com_type ComType;
     * RETURNS: None.
     */
    sock( const type NewType, const com_type ComType = com_type::TCP ):
      Type(NewType)
    {
      Init();

      hSocket = socket(AF_INET, ComType == com_type::TCP ? SOCK_STREAM : SOCK_DGRAM, 0); // Socket creation

      ReadBuf.resize(MAX_MESSAGE_LEN);
    } /* End of 'port' function */

    /* Destruct function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~sock( VOID )
    {
      EndAcceptor();
      closesocket(hSocket);
      ReaderThreads.clear();
      //Close(); TODO
    } /* End of '~port' function */

    /* Get active port number function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) port number.
     */
    INT GetActivePort( VOID )
    {
      return ActivePort;
    } /* End of 'GetActivePort' function */

    /* Get socket handle function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (SOCKET) socket handle.
     */
    SOCKET GetSocketHandle( VOID )
    {
      return hSocket;
    } /* End of 'GetSocketHandle' function */

    /********** Server part **********/

    /* Is bind function */
    inline BOOL IsBind( VOID )
    {
      return ActivePort != -1;
    } /* End of 'IsBind' function */

    /* Is listen function */
    inline BOOL IsListen( VOID )
    {
      return IsListenFlag;
    } /* End of 'IsListen' function */

    /* Bind function.
     * ARGUMENTS:
     *   - port:
     *       const UINT NewPort;
     * RETURNS: None.
     */
    VOID Bind( const UINT NewPort )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
        return;
      }

      if (ActivePort != -1)
        Log("WARNING: rebind form " + std::to_string(ActivePort) + " to " + std::to_string(NewPort)); // TODO - unbind binded port

      sockaddr_in Addr;

      Addr.sin_family = AF_INET;
      Addr.sin_addr.s_addr = INADDR_ANY;
      Addr.sin_port = htons(NewPort);

      Log("Bind port '" + std::to_string(NewPort) + "'...");
      if (bind(hSocket, (sockaddr *)&Addr, sizeof(Addr)) < 0)
        LogError("Bind failed");
      else
        ActivePort = NewPort;
    } /* End of 'Bind' function */

    /* Listen function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) success of listen.
     */
    BOOL Listen( VOID )
    {
      IsListenFlag = false;

      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
        return false;
      }

      if (!IsBind())
      {
        LogError("Failed to listen - no port is binded");
        return false;
      }

      Log("Listen...");
      if (listen(hSocket, MAX_Q_LEN) < 0)
      {
        LogError("Listen failed");
        return false;
      }

      IsListenFlag = true;

      return true;
    } /* End of 'Listen' function */

    /* Accept function.
     * ARGUMENTS:
     *   - accept call back function:
     *       const std::function<VOID( const SOCKET ClientSocket )> &OnAcceptCallBack;
     * RETURNS:
     *   (SOCKET) connected socket handle.
     */
    SOCKET Accept( const std::function<VOID( const SOCKET ClientSocket )> &OnAcceptCallBack )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
        return INVALID_SOCKET;
      }

      if (!IsBind())
      {
        LogError("Failed to accept - no port is binded");
        return INVALID_SOCKET;
      }
    
      sockaddr_in Addr;
    
      Addr.sin_family = AF_INET;
      Addr.sin_addr.s_addr = INADDR_ANY;
      Addr.sin_port = htons(ActivePort);
    
      SOCKET NewSocket;
      UINT AddrLen = sizeof(Addr);
    
      Log("Accept...");
      if ((NewSocket = accept(hSocket, (sockaddr *)&Addr, (socklen_t *)&AddrLen)) < 0)
        LogError("Accept failed");
      else
      {
        std::string ClientIpStr;
        ClientIpStr.resize(INET_ADDRSTRLEN);
    
        // Get ip addres str
        inet_ntop(AF_INET, (sockaddr*)&Addr, &ClientIpStr[0], ClientIpStr.size());
        Log("LIENT ACCEPTED --> IP: " + ClientIpStr + "  PORT: " + std::to_string(ActivePort));

        OnAcceptCallBack(NewSocket);

        Connections.emplace(NewSocket);
      }
    
      return NewSocket;
    } /* End of 'Accept' function */

    /* Start acceptor thread function.
     * ARGUMENTS:
     *   - max count of clients:
     *       const UINT MaxClientsCount;
     * RETURNS:
     *   (BOOL) success of operation.
     */
    BOOL StartAcceptor( const UINT MaxClientsCount, const std::function<VOID( const SOCKET ClientSocket )> &AcceptCallBack, const std::function<VOID( const SOCKET hReadSocket, const std::span<BYTE> &Message )> &ReadCallBack )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for client type socket
        return false;
      }
    
      if (AcceptorThread.has_value())
      {
        LogError("Failed to start acceptor - acceptor is already started.");
        return false;
      }

      if (!IsBind())
      {
        LogError("Failed to start acceptor - no port is binded");
        return false;
      }
    
      Log("Start acceptor...");
      AcceptorThread = std::thread(
        [=]( VOID )
        {
          if (Listen())
            while (Connections.size() <= MaxClientsCount)
              StartReader(Accept(AcceptCallBack), ReadCallBack);
        });
      AcceptorThread.value().detach();

      return true;
    } /* End of 'StartAcceptor' function */

    /* End acceptor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID EndAcceptor( VOID )
    {
      if (AcceptorThread.has_value())
      {
        Log("Acceptor ended...");
        AcceptorThread.reset();
      }
    } /* End of 'EndAcceptor' function */

    /********** Client part **********/

    /* Connect function.
     * ARGUMENTS:
     *   - addr:
     *       const std::string &IpStr;
     *   - port:
     *       UINT OutPort;
     * RETURNS:
     *   (BOOL) success of operation.
     */
    BOOL Connect( const std::string &IpStr, UINT OutPort )
    {
      IsConnectedFlag = false;

      if (Type != type::CLIENT)
      {
        assert(0); // Bind function is only for client type socket
        return false;
      }

      sockaddr_in Addr;

      Addr.sin_family = AF_INET;
      Addr.sin_port = htons(OutPort);

      // Convert ip form str to bin

      if (inet_pton(AF_INET, IpStr.c_str(), &Addr.sin_addr) <= 0)
      {
        LogError("Invalid ip");
        return false;
      }

      Log("Connect...");
      if (connect(hSocket, (sockaddr *)&Addr, sizeof(Addr)) < 0)
      {
        LogError("Connect");
        return false;
      }

      IsConnectedFlag = true;

      return true;
    } /* End of 'Connect' function */

    /* Is connected function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) is connected.
     */
    BOOL IsConnected( VOID )
    {
      return Type == type::SERVER || IsConnectedFlag;
    } /* End of 'IsConnected' function */

    /******* Common *******/

    private:
      std::vector<BYTE> ReadBuf; // Buffer is local to not allocate mem every time
    public:

    /* Read inline function.
     * ARGUMENTS:
     *   - socket heandle:
     *       const SOCKET hReadingSocket;
     * RETURNS:
     *   (std::vector<BYTE>) message.
     */
    inline std::span<BYTE> Read( const SOCKET hReadingSocket )
    {
      switch (INT Result = recv(hReadingSocket, (CHAR *)ReadBuf.data(), (INT)ReadBuf.size(), 0))
      {
      case -1:
        LogError(std::format("OOOOOOOOH SHIT - reading socket failed - result: {}", Result));
        switch (WSAGetLastError())
        {
        case WSAENOTCONN:
        case WSAENETRESET:
        case WSAENOTSOCK:
        case WSAESHUTDOWN:
        case WSAEINVAL:
        case WSAECONNRESET:
        case WSAECONNABORTED:
          IsConnectedFlag = false;
          break;
        }
      case 0:
        break; // Nothing has been readed
      default:
        return {ReadBuf.begin(), ReadBuf.begin() + Result};
      }
      return {};
    } /* End of 'Read' function */

    /* Send inline function.
     * ARGUMENTS:
     *   - socket heandle:
     *       const SOCKET hOutSocket;
     *   - message:
     *       const std::vector<BYTE> &Message;
     * RETURNS:
     *   (BOOL) success of operation.
     */
    inline BOOL Send( const SOCKET hOutSocket, std::span<const BYTE> Message )
    {
      if (send(hOutSocket, (CHAR *)Message.data(), (UINT)Message.size(), 0) == SOCKET_ERROR)
      {
        LogError("OOOOOOOOOH SHIT - Sending failed.");
        return false;
      }
      return true;
    } /* End of 'Send' function */

    /* Send to every connection function.
     * ARGUMENTS:
     *   - message:
     *       const std::span<BYTE> &Message;
     * RETURNS: None.
     */
    VOID SendToAll( std::span<const BYTE> Message )
    {
      BOOL Result = false;

      for (auto &hS : Connections)
        Result |= Send(hS, Message);
    } /* End of 'SendToAll' function */

    /* Send to every connection except one function.
     * ARGUMENTS:
     *   - not usable socket:
     *       const SOCKET hOutSocket;
     *   - message:
     *       const std::span<BYTE> &Message;
     * RETURNS: None.
     */
    VOID SendToAllExcept( const SOCKET hExceptSocket, std::span<const BYTE> Message )
    {
      BOOL Result = false;

      for ( auto &hS : Connections)
        if (hS != hExceptSocket)
          Result |= Send(hS, Message);
    } /* End of 'SendToAll' function */

    /* Start reader function.
     * ARGUMENTS:
     *   - socket for read:
     *       const SOCKET hReadSocket;
     *   - call back func:
     *       const std::function<VOID( const SOCKET hReadSocket, const std::vector<BYTE> &Message )> &ReadCallBack;
     *   - arguments to read call back function:
     *       ReadArgTypes ...ReadCBFArgs;
     * RETRUNS: None.
     */
    VOID StartReader( const SOCKET hReadSocket, const std::function<VOID( const SOCKET hReadSocket, std::span<BYTE> Message )> &ReadCallBack )
    {
      ReaderThreads.emplace(hReadSocket, std::thread(
        [=]( VOID )
        {
          while (IsConnected())
            ReadCallBack(hReadSocket, Read(hReadSocket));
        }));
    } /* End of 'StartReader' function */

  }; /* End of 'sock' class */

} /* end of 'sl' namespace */


#endif // __SL_H_

/* END OF 'sl.h' FILE */