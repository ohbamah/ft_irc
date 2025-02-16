/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.inl                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:49:06 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 15:58:19 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

template <typename T>
void
Server::SendTo(const Server::Str& clientName, const T& data, unsigned int nbytes)
{
	this->FindClientByName(clientName)->GetRemote()->Send(data, nbytes);
}
template <typename T>
void
Server::SendTo(const Server::Str& clientName, const T* data, unsigned int nbytes)
{
	this->FindClientByName(clientName)->GetRemote()->Send(data, nbytes);
}

template <typename T>
void
Server::RecvFrom(const Server::Str& clientName, T& data, unsigned int nbytes)
{
	this->FindClientByName(clientName)->GetRemote()->Recv(data, nbytes);
}
template <typename T>
void
Server::RecvFrom(const Server::Str& clientName, T* data, unsigned int nbytes)
{
	this->FindClientByName(clientName)->GetRemote()->Recv(data, nbytes);
}

template <typename T>
void
Server::SendTo(int clientIndex, const T& data, unsigned int nbytes)
{
	this->clients[clientIndex]->GetRemote()->Send(data, nbytes);
}
template <typename T>
void
Server::SendTo(int clientIndex, const T* data, unsigned int nbytes)
{
	this->clients[clientIndex]->GetRemote()->Send(data, nbytes);
}

template <typename T>
void
Server::RecvFrom(int clientIndex, T& data, unsigned int nbytes)
{
	this->clients[clientIndex]->GetRemote()->Recv(data, nbytes);
}
template <typename T>
void
Server::RecvFrom(int clientIndex, T* data, unsigned int nbytes)
{
	this->clients[clientIndex]->GetRemote()->Recv(data, nbytes);
}
