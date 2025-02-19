/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 17:35:41 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

#include <sys/select.h>

Irc::Irc(int port, const char* pass) : server(port, FControl::NonBlock)
{
	this->server.Listen();

	this->sync.AddReadReq(this->server.Get());
	while (1)
	{
		this->sync.SnapEvents(0);
		this->HandleClients();
		if (this->sync.CanRead(this->server.Get()))
			this->AcceptConnexion();
	}
}	

Irc::~Irc()
{
	for (unsigned long i = 0 ; i < this->server.RefClients().size() ; ++i)
		delete(this->server.RefClients()[i]);
}

void
Irc::HandleClientConnexion(Client* local)
{
	char	message[512];
	local->GetRemote()->Recv(message);
	Req::Check(this->server, this->channels, local, message);
}

void
Irc::AcceptConnexion(void)
{
	try
	{
		std::cout << "\e[34mConnexion...\e[0m" << std::endl;
		std::stringstream	itos;
		itos << "client" << this->server.RefClients().size();

		this->server.Accept(itos.str());
		Client*	localClient = this->server.FindClientByName(itos.str());
		this->sync.AddReadReq(localClient->GetRemote()->Get());
		this->sync.AddExcpReq(localClient->GetRemote()->Get());

		std::cout << "\e[32m" << "successfuly connected!\e[0m" << std::endl;
	}
	catch (...)
	{
		std::cout << "\e[31mConnexion echouee...\e[0m" << std::endl;
	}
}

void
Irc::HandleClients(void)
{
	char	message[512];

	for (long i = 0 ; (unsigned long)i < this->server.RefClients().size() ; ++i)
	{
		try
		{
			std::stringstream	itos;
			itos << i;
			int clientFd = this->server.RefClients()[i]->GetRemote()->Get();
			if (this->sync.Exception(clientFd))
			{
				this->DisconnectAnyone(this->server.RefClients()[i]);
				i = -1;
			}
			else if (this->sync.CanRead(clientFd))
			{
				if (this->server.RecvFrom(i, message, 512) <= 0)
				{
					this->DisconnectAnyone(this->server.RefClients()[i]);
					i = -1;
					std::exit(0);
				}
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

void
Irc::DisconnectAnyone(Client* c)
{
	int clientFd = c->GetRemote()->Get();

	this->sync.RemoveExcpReq(clientFd);
	this->sync.RemoveReadReq(clientFd);
	this->sync.RemoveWriteReq(clientFd);
	for (unsigned long i = 0 ; i < this->channels.size() ; ++i)
		this->channels[i].Disconnect(c);
	this->server.Disconnect(c);
}
