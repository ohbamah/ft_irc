/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:36:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 17:42:48 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FControl.hpp"

Server::Server(int port) : Socket(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
	this->Socket::SetOptions(SO_REUSEADDR | SO_REUSEPORT);
	this->Socket::Bind(port);
	FControl::SetFlags(this->Socket::Get(), FControl::NonBlock);
}

Server::~Server()
{
}

const int&
Server::Get(void)
{
	return (this->fd);
}

std::vector<Client*>&
Server::RefClients(void)
{
	return (this->clients);
}

void
Server::Accept(const Server::Str& clientName)
{
	this->clients.push_back(new Client(clientName, this->Socket::Accept()));
}

void
Server::Listen(void)
{
	this->Socket::Listen();
}

Client*
Server::FindClientByName(const Str& name)
{
	for (unsigned long i = 0 ; i < this->clients.size() ; ++i)
		if (!this->clients[i]->GetName().compare(name))
			return (this->clients[i]);
	throw (Server::CantFindClient());
}
