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

    SOCKET hSocket {(SOCKET)-1};      // System socket handle

    // Server
    INT ActivePort {-1}; // Active port number

  public: // TMP KAL

    std::set<SOCKET> Connections; // Conected client? socket

  private:

    std::map<SOCKET, std::thread> ReaderThreads; // Threads with readers

    // Client
    BOOL IsConnectedFlag {0};

  public:

    const type Type; // This port type

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

    /* Bind function.
     * ARGUMENTS:
     *   - port:
     *       const UINT NewPort;
     * RETURNS:
     *   (sock &) self refernce.
     */
    sock & Bind( const UINT NewPort )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
        return *this;
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

      return *this;
    } /* End of 'Bind' function */

    /* Listen function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (sock &) self reference.
     */
    sock & Listen( VOID )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
        return *this;
      }

      Log("Listen...");
      if (listen(hSocket, MAX_Q_LEN) < 0)
        LogError("Listen failed");

      return *this;
    } /* End of 'Listen' function */

    /* Typedef call back types */
    template<typename ...ArgTypes>
      using AcceptCBFType = VOID (*)( const SOCKET ClientSocket, ArgTypes ...Args ) ;
    template<typename ...ArgTypes>
      using ReadCBFType = VOID (*)( const SOCKET hSender, const std::vector<BYTE> &Data, ArgTypes ...Args ) ;

    /* Accept function.
     * ARGUMENTS:
     *   - accept call back function:
     *       VOID (*AcceptCallBack)( const SOCKET ClientSocket );
     * RETURNS:
     *   (SOCKET) connected socket handle.
     */
    SOCKET Accept( const std::function<VOID( const SOCKET ClientSocket )> &AcceptCallBack )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for server type socket
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

        AcceptCallBack(NewSocket);

        Connections.emplace(NewSocket);
      }
    
      return NewSocket;
    } /* End of 'Accept' function */

    /* Start acceptor thread function.
     * ARGUMENTS:
     *   - max count of clients:
     *       const UINT MaxClientsCount;
     * RETURNS:
     *   (sock &) self reference.
     */
    sock & StartAcceptor( const UINT MaxClientsCount, const std::function<VOID( const SOCKET ClientSocket )> &AcceptCallBack, const std::function<VOID( const SOCKET hReadSocket, const std::vector<BYTE> &Message )> &ReadCallBack )
    {
      if (Type != type::SERVER)
      {
        assert(0); // Bind function is only for client type socket
        return *this;
      }
    
      if (AcceptorThread.has_value())
        return *this;
    
      Log("Start acceptor...");
      AcceptorThread = std::thread(
        [=]( VOID )
        {
          Listen();
          while (Connections.size() <= MaxClientsCount)
            StartReader(Accept(AcceptCallBack), ReadCallBack);
        });
      AcceptorThread.value().detach();
    
      return *this;
    } /* End of 'StartAcceptor' function */

    /* End acceptor function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (sock &) self reference.
     */
    sock & EndAcceptor( VOID )
    {
      if (AcceptorThread.has_value())
      {
        Log("Acceptor ended...");
        AcceptorThread.reset();
      }
      
      return *this;
    } /* End of 'EndAcceptor' function */

    /********** Client part **********/

    /* Connect function.
     * ARGUMENTS:
     *   - addr:
     *       const std::string &IpStr;
     *   - port:
     *       UINT OutPort;
     * RETURNS:
     *   (sock &) self reference.
     */
    sock & Connect( const std::string &IpStr, UINT OutPort )
    {
      if (Type != type::CLIENT)
      {
        assert(0); // Bind function is only for client type socket
        return *this;
      }

      sockaddr_in Addr;

      Addr.sin_family = AF_INET;
      Addr.sin_port = htons(OutPort);

      // Convert ip form str to bin

      if (inet_pton(AF_INET, IpStr.c_str(), &Addr.sin_addr) <= 0)
      {
        LogError("Invalid ip");
        return *this;
      }

      Log("Connect...");
      if (connect(hSocket, (sockaddr *)&Addr, sizeof(Addr)) < 0)
        LogError("Connect");
      else
        IsConnectedFlag = 1;

      return *this;
    } /* End of 'Connect' function */

    /* Is connected function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) is connected.
     */
    BOOL IsConnected( VOID )
    {
      return IsConnectedFlag;
    } /* End of 'IsConnected' function */

    /******* Common *******/

    private:
      std::vector<BYTE> ReadBuf;
    public:

    /* Read inline function.
     * ARGUMENTS:
     *   - socket heandle:
     *       const SOCKET hReadingSocket;
     * RETURNS:
     *   (std::vector<BYTE>) message.
     */
    inline std::vector<BYTE> Read( const SOCKET hReadingSocket )
    {
      switch (INT Result = recv(hReadingSocket, (CHAR *)ReadBuf.data(), (INT)ReadBuf.size(), 0))
      {
      case -1:
        LogError("OOOOOOOOH SHIT - reading socket failed.");
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
     *   (sock &) self reference.
     */
    inline sock & Send( const SOCKET hOutSocket, const std::vector<BYTE> &Message )
    {
      switch (send(hOutSocket, (CHAR *)Message.data(), (UINT)Message.size(), 0))
      {
      case SOCKET_ERROR:
        LogError("OOOOOOOOOH SHIT - Sending failed.");
        break;
      }
      return *this;
    } /* End of 'Send' function */

    /* Send to every connection function.
     * ARGUMENTS:
     *   - message:
     *       const std::vector<BYTE> &Message;
     * RETURNS: None.
     */
    VOID SendToAll( const std::vector<BYTE> &Message )
    {
      for ( auto &hS : Connections)
        Send(hS, Message);
    } /* End of 'SendToAll' function */

    /* Send to every connection except one function.
     * ARGUMENTS:
     *   - not usable socket:
     *       const SOCKET hOutSocket;
     *   - message:
     *       const std::vector<BYTE> &Message;
     * RETURNS: None.
     */
    VOID SendToAllExcept( const SOCKET hExceptSocket, const std::vector<BYTE> &Message )
    {
      for ( auto &hS : Connections)
        if (hS != hExceptSocket)
          Send(hS, Message);
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
    VOID StartReader( const SOCKET hReadSocket, const std::function<VOID( const SOCKET hReadSocket, const std::vector<BYTE> &Message )> &ReadCallBack )
    {
      ReaderThreads.emplace(hReadSocket, std::thread(
        [=]( VOID )
        {
          while (1)
            ReadCallBack(hReadSocket, Read(hReadSocket));
        }));
    } /* End of 'StartReader' function */

  }; /* End of 'sock' class */

} /* end of 'sl' namespace */


#endif // __SL_H_

/* END OF 'sl.h' FILE */