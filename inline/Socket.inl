/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.inl                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 02:04:02 by bama              #+#    #+#             */
/*   Updated: 2025/03/24 15:13:02 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_INL
#define SOCKET_INL

# include "Socket.hpp"

template <typename T>
int
Socket::Recv(T& data, int nbytes)
{
	int	bytesSend;

	if (this->com == -1)
		throw (Socket::CantRecv());
	if ((bytesSend = recv(this->com, &data, nbytes, 0)) < 0)
		throw (Socket::FailedRecv());
	return (bytesSend);
}

// template <typename T>
// int
// Socket::Recv(T* data, int nbytes)
// {
// 	int	bytesSend;

// 	if (this->com == -1)
// 		throw (Socket::CantRecv());
// 	if ((bytesSend = recv(this->com, data, nbytes, 0)) < 0)
// 		throw (Socket::FailedRecv());
// 	return (bytesSend);
// }

#include <errno.h>
#include <iostream>
template <typename T>
int
Socket::Recv(T* data, int nbytes)
{
	int bytesRead;

	if (this->com == -1)
		throw (Socket::CantRecv());

	bytesRead = recv(this->com, data, nbytes, 0);
	if (bytesRead < 0)
	{
		std::cerr << "Erreur recv() : " << strerror(errno) << std::endl;
		throw (Socket::FailedRecv());
	}
	return bytesRead;
}


template <typename T>
int
Socket::Send(const T& data, int nbytes)
{
	int	bytesSend;

	if (this->com == -1)
		throw (Socket::CantRecv());
	if ((bytesSend = send(this->com, &data, nbytes, 0)) < 0)
		throw (Socket::FailedRecv());
	return (bytesSend);
}

template <typename T>
int
SocketRemote::Recv(T& data, int nbytes)
{
	return (Socket::Recv(data, nbytes));
}

template <typename T>
int
SocketRemote::Recv(T* data, int nbytes)
{
	return (Socket::Recv(data, nbytes));
}

template <typename T>
int
SocketRemote::Send(const T& data, int nbytes)
{
	return (Socket::Send(data, nbytes));
}

#endif
