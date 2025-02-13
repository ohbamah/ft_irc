/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 02:51:47 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(int port, const char* pass) : server(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
	this->server.Bind(port);
	this->server.Listen();
	this->server.SetOptions(SO_REUSEADDR);

	this->ircPoll.AddFd(this->server.Get());
	FControl::SetFlags(this->server.Get(), FControl::NonBlock);
	while (1)
	{
		this->ircPoll.Check(0);
		this->AcceptConnexion();
		this->RecvMessage();
	}
}

Irc::~Irc()
{
}

void
Irc::AcceptConnexion(void)
{
	try
	{
		//std::cout << "feur\n";
		if (this->ircPoll.ReadRequest(this->server.Get()))
		{
			std::cout << "Connexion en cours..." << std::endl;
			this->clients.push_back(this->server.Accept());
			std::cout << "Connexion detected and accepted!" << std::endl;
		}
	}
	catch (...)
	{
	}
}

void
Irc::RecvMessage(void)
{
	char	message;

	for (unsigned long i = 0 ; i < this->clients.size() ; ++i)
	{
		try
		{
			if (this->ircPoll.WriteRequest(this->clients[i].Get()))
			{
				this->clients[i].Recv(message);
				std::cout << message << std::endl;
			}
		}
		catch (Socket::FailedRecv& e)
		{
		}
	}
}

void
Irc::SendMessage(void)
{
	
}
