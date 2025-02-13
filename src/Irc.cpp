/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 18:08:01 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(int port, const char* pass) : server(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
	this->server.Bind(port);
	this->server.Listen();
	this->server.SetOptions(SO_REUSEADDR);

	this->ircPoll.AddFd("server", this->server.Get(), POLLIN);
	FControl::SetFlags(this->server.Get(), FControl::NonBlock);
	//pollfd	pfd;
	//pfd.events = POLLIN;
	//pfd.revents = 0;
	//pfd.fd = this->server.Get();
	//polls.push_back(pfd);
	while (1)
	{
		this->ircPoll.Events(0);
		//this->ircPoll.Check(0);
		//poll(&pfd, 1, 0);
		//if (pfd.revents & POLLIN)
		if (this->ircPoll.ReadRequest("server"))
			this->AcceptConnexion();
		//if (pfd.revents & POLLIN)
		this->RecvMessage();
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
		//std::cout << "feur\n";
		//if (this->ircPoll.ReadRequest(this->server.Get()))
		//{

			std::cout << "Connexion..." << std::endl;
			this->clients.push_back(this->server.Accept());
			std::cout << "Connexion detected and accepted!" << std::endl;
		//}
	}
	catch (...)
	{
	}
}

void
Irc::RecvMessage(void)
{
	char	message[512];

	for (unsigned long i = 0 ; i < this->clients.size() ; ++i)
	{
		try
		{
			//if (this->ircPoll.ReadRequest(this->clients[i]->Get()))
			//{
			//	this->clients[i]->Recv(message);
			//	std::cout << message << std::endl;
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
