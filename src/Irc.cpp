/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/14 20:10:42 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(int port, const char* pass) : server(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
	this->server.SetOptions(SO_REUSEADDR | SO_REUSEPORT);
	this->server.Bind(port);
	this->server.Listen();

	FControl::SetFlags(this->server.Get(), FControl::NonBlock);
	this->ircPoll.AddFd("server", this->server.Get(), Poll::ReadReq);
	while (1)
	{
		this->ircPoll.Events(0);
		this->RecvMessage();
		if (this->ircPoll.ReadRequest("server"))
			this->AcceptConnexion();
	}
}

Irc::~Irc()
{
	for (unsigned long i = 0 ; i < this->clients.size() ; ++i)
		delete(this->clients[i]);
}

void
Irc::AcceptConnexion(void)
{
	try
	{
		std::cout << "Connexion..." << std::endl;
		std::stringstream	itos;
		itos << this->clients.size();
		this->clients.push_back(this->server.Accept());
		this->ircPoll.AddFd("client" + itos.str(), (*(this->clients.end() - 1))->Get(), POLLIN);
		std::cout << "Connexion detected and accepted!" << std::endl;
	}
	catch (...)
	{
	}
}

void
Irc::RecvMessage(void)
{
	char	message[512] = {0};

	for (unsigned long i = 0 ; i < this->clients.size() ; ++i)
	{
		try
		{
			//std::cout << "client" << i << std::endl;
			std::stringstream	itos;
			itos << i;
			//if (this->ircPoll.ReadRequest("client" + itos.str()))
			//{
				std::cout << this->clients[i]->Recv(message);
				std::cout << message << std::endl;
			//}
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
