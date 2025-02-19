/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 15:13:25 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(void) : bound(false), connected(false), client(true), listener(false), closed(true), com(-1), fd(-1)
{
}

Socket::Socket(int af, int type, int prot) : bound(false), connected(false), client(false), listener(false), closed(false), com(-1)
{
	this->fd = socket(af, type, prot);
	if (this->fd == -1)
		throw (Socket::FailedToCreate());
	this->info.address = NULL;
	this->info.port = 0;
}

Socket::~Socket()
{
	this->Close();
}

void
Socket::SetOptions(int flags)
{
	bool	active = true;
	setsockopt(this->fd, SOL_SOCKET, flags, &active, sizeof(active));
}

void
Socket::Listen(void)
{
	if (!this->listener)
	{
		if (!this->bound)
			throw (Socket::IsNotAServer());
		if (listen(this->fd, SOMAXCONN) == -1)
			throw (Socket::CantListen());
		this->listener = true;
	}
}

SocketRemote*
Socket::Accept(void)
{
	if (!this->bound)
		throw (Socket::IsNotAServer());
	if (!this->listener)
		throw (Socket::IsNotListening());
	sockaddr_in	saddr;
	socklen_t	saddr_len = 0;
	int			client_fd;

	std::memset(&saddr, 0, sizeof(saddr));
	if ((client_fd = accept(this->fd, (sockaddr*)&saddr, &saddr_len)) == -1)
		throw (Socket::CantAccept());

	char			clientIp[INET_ADDRSTRLEN];
	SocketRemote*		sr = new SocketRemote(client_fd);
	inet_ntop(Socket::AddrFamily::IPv4, &saddr.sin_addr, clientIp, INET_ADDRSTRLEN);
	sr->SetInfo(std::string(clientIp).c_str(), ntohs(saddr.sin_port));
	return (sr);
}

void
Socket::Connect(const Socket& socket, const char *address, int port)
{
	if (this->connected)
		throw (Socket::AlreadyConnected());
	else if (this->bound)
		throw (Socket::CantConnectAServer());
	else
	{
		sockaddr_in	saddr;

		std::memset(&saddr, 0, sizeof(saddr));
		saddr.sin_family = Socket::AddrFamily::IPv4;
		saddr.sin_port = htons(port);
		inet_pton(saddr.sin_family, address, &saddr.sin_addr.s_addr);
		if (connect(socket.Get(), (const sockaddr*)&saddr, sizeof(saddr)) == -1)
			throw (Socket::ConnectionFailed());
		this->connected = true;
		this->com = dup(this->fd);
	}
}

#include <errno.h>
#include <stdlib.h>
void
Socket::Bind(int port)
{
	if (this->bound)
		throw (Socket::AlreadyBound());
	else if (this->connected)
		throw (Socket::CantBindAClient());
	else
	{
		sockaddr_in	saddr;

		std::memset(&saddr, 0, sizeof(saddr));
		saddr.sin_family = Socket::AddrFamily::IPv4;
		saddr.sin_addr.s_addr = INADDR_ANY;
		saddr.sin_port = htons(port);
		if (bind(this->fd, reinterpret_cast<const sockaddr*>(&saddr), sizeof(saddr)) == -1)
			throw (Socket::ImpossibleToBind());
		this->info.port = port;
		this->bound = true;
	}
}

void
Socket::Close(void)
{
	if (this->fd != -1)
		close(this->fd);
	if (this->com != -1)
		close(this->com);
	this->fd = -1;
	this->com = -1;
	this->closed = true;
}

const int&
Socket::Get(void) const
{
	return (this->fd);
}

int
SocketRemote::Get(void)
{
	return (this->fd);
}

SocketRemote::SocketRemote(int fd) : Socket()
{
	this->com = fd;
}

SocketRemote::~SocketRemote()
{
}

void
SocketRemote::Close(void)
{
	this->Socket::Close();
}

void
SocketRemote::SetInfo(const char* addr, int port)
{
	this->info.address = addr;
	this->info.port = port;
}
