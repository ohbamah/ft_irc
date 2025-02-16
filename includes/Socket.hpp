/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 19:26:12 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <exception>
# include <unistd.h>
# include <cstring>
# include <string>
# include "Ptr.hpp"

struct SocketInfo
{
	const char*	address;
	int			port;
};

class SocketRemote;

class Socket
{
public:
	Socket(int af, int type, int prot);
	virtual ~Socket();

	template <typename T>
	int
	Recv(T& data, int nbytes = sizeof(T));
	template <typename T>
	int
	Recv(T* data, int nbytes);
	template <typename T>
	int
	Send(const T& data, int nbytes = sizeof(T));

	void
	SetOptions(int flags);
	void
	Listen(void);
	SocketRemote*
	Accept(void);
	void
	Connect(const Socket& socket, const char *address, int port);
	void
	Bind(int port);
	void
	Close(void);
	const int&
	Get(void) const;

protected:
	Socket(void);

protected:
	SocketInfo	info;
	bool		bound;
	bool		connected;
	bool		client;
	bool		listener;
	int			com;	// the communication fd
	int			fd;

public:
	struct FailedToCreate : std::exception { inline virtual const char* what(void) const throw() {return ("Failed to create a socket\n"); } };
	struct ImpossibleToBind : std::exception { inline virtual const char* what(void) const throw() {return ("Impossible to bind\n"); } };
	struct AlreadyBound : std::exception { inline virtual const char* what(void) const throw() {return ("Already bound\n"); } };
	struct ConnectionFailed : std::exception { inline virtual const char* what(void) const throw() {return ("Connection failed\n"); } };
	struct AlreadyConnected : std::exception { inline virtual const char* what(void) const throw() {return ("Client socket already connected\n"); } };
	struct CantBindAClient : std::exception { inline virtual const char* what(void) const throw() {return ("Can not bind a client socket\n"); } };
	struct CantConnectAServer : std::exception { inline virtual const char* what(void) const throw() {return ("Can not connect a server socket\n"); } };
	struct IsNotAServer : std::exception { inline virtual const char* what(void) const throw() {return ("Can not accept anything, because socket is not a server\n"); } };
	struct CantAccept : std::exception { inline virtual const char* what(void) const throw() {return ("Server can not accept client\n"); } };
	struct CantListen : std::exception { inline virtual const char* what(void) const throw() {return ("Server can not listen\n"); } };
	struct IsNotListening : std::exception { inline virtual const char* what(void) const throw() {return ("Server is not listening\n"); } };
	struct FailedRecv : std::exception { inline virtual const char* what(void) const throw() {return ("Failed to receive data\n"); } };
	struct FailedSend : std::exception { inline virtual const char* what(void) const throw() {return ("Failed to send data\n"); } };
	struct CantRecv : std::exception { inline virtual const char* what(void) const throw() {return ("Can not receive data\n"); } };
	struct CantSend : std::exception { inline virtual const char* what(void) const throw() {return ("Can not send data\n"); } };

public:
		// Socket protocol
	struct Protocol
	{
		enum
		{
			Unknow	= -1,			// The SocketProtocol is unknow
			Auto	= 0,
			TCP		= IPPROTO_TCP,
			UDP		= IPPROTO_UDP,
			ICMP	= IPPROTO_ICMP, // Used for control protocols, such as ping
			IPv4	= IPPROTO_IP,
			IPv6	= IPPROTO_IPV6
		};
	};

		// Socket domain
	struct AddrFamily
	{
		enum
		{
			Unknow		= -1,				// The SocketAddressFamily is unknow
			Default		= AF_INET,			// Equivalent to IPv4
			IPv4 		= AF_INET,
			IPv6 		= AF_INET6,
			Linked		= AF_PACKET,		// Low-level/physicaly linked communication (like Ethernet)
			Netlink		= AF_NETLINK,		// Communication between the UNIX kernel and the user's space
# if defined(AF_LOCAL)
			LocalHost	= AF_LOCAL,			// Communication only with the localhost
# elif defined(AF_UNIX)
			LocalHost	= AF_UNIX,			// Communication only with the localhost
# endif
# if defined(AF_BLUETOOTH)
			Bluetooth	= AF_BLUETOOTH,
# endif
# if defined(AF_IRDA)
			Infrared	= AF_IRDA,			// Infrared Data Association
# endif
# if defined(AF_ROSE)
			ROSE		= AF_ROSE,			// For ham radio communication
# endif
# if defined(AF_AX25)
			AX25		= AF_AX25,			// For ham radio communication
# endif
# if defined(AF_X25)
			X25			= AF_X25,			// ! Now deprecated !
# endif
# if defined(AF_IPX)
			IPX			= AF_IPX,			// ! Now deprecated !
# endif
# if defined(AF_DECnet)
			DECnet		= AF_DECnet,		// ! Now deprecated !
# endif
# if defined(AF_APPLETALK)
			AppleTalk	= AF_APPLETALK		// ! Now deprecated !
# endif
		};
	};

		// Socket type
	struct Type
	{
		enum
		{
			Unknow		= -1,				// The SocketType is unknow
			TCP			= SOCK_STREAM,		// Provides a reliable, connection-oriented data flow, guarantees data integrity (delivery in order, no duplicates)
			UDP			= SOCK_DGRAM,		// Provides an unreliable, connectionless datagram service, does not guarantee packet order or delivery, but is faster than SocketType::TCP
			RAW			= SOCK_RAW,			// Provides direct access to IP packets, without kernel processing, used for custom network protocol development and packet manipulation. Requires administrative privileges
			RDM			= SOCK_RDM,			// Ensures reliable delivery of datagrams, but does not ensure the connection
# if defined(SOCK_PACKET)
			Linked		= SOCK_PACKET,		// Used to receive packets at data link level, like AF_PACKET in Linux, !deprecated on many modern system!
# endif
			Sequential	= SOCK_SEQPACKET	// Similar to SocketType::UDP but guarantees the integrity and sequence of packets.
		};
	};
};

class SocketRemote : private Socket
{
public:
	SocketRemote(int fd);
	virtual ~SocketRemote();

	void
	SetInfo(const char* addr, int port);
	int
	Get(void);

	template <typename T>
	int
	Recv(T& data, int nbytes = sizeof(T));
	template <typename T>
	int
	Recv(T* data, int nbytes);
	template <typename T>
	int
	Send(const T& data, int nbytes = sizeof(T));
};

# include "Socket.inl"

#endif
