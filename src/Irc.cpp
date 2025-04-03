/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/03 11:35:01 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

#include <sys/select.h>

bool
Irc::exitReq = false;

Irc::Irc(int port, const char* pass)// : server(port, FControl::NonBlock)
{
	try {
		this->server.Start(port, FControl::NonBlock);
		this->server.Listen();
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return ;
	}

	this->sync.AddReadReq(this->server.Get());
	this->mdp = pass;
	server.SetPassword(mdp);
	while (Irc::exitReq == false)
	{
		try {
			this->sync.SnapEvents(0);
			this->HandleClients();
			
			if (this->sync.CanRead(this->server.Get())) {
				this->AcceptConnexion();
			}
		} catch (const Select::FatalError& e) {
			std::cerr << "Select error: " << e.what() << std::endl;
		} catch (const std::exception& e) {
			std::cerr << "Unexpected error: " << e.what() << std::endl;
		} catch (...) {
			std::cerr << "test\n";
		}
	}
	
}	

Irc::~Irc()
{
	for (unsigned long i = 0 ; i < this->server.RefClients().size() ; ++i)
		delete(this->server.RefClients()[i]);
}

void
Irc::AcceptConnexion(void)
{
	try
	{
		std::stringstream	itos;
		itos << "client" << Server::cpt++;

		this->server.Accept(itos.str());

		Client*	localClient = this->server.FindClientByName(itos.str()); // ByNickName
		this->sync.AddReadReq(localClient->GetRemote()->Get());
		this->sync.AddWriteReq(localClient->GetRemote()->Get());
		this->sync.AddExcpReq(localClient->GetRemote()->Get());
		std::cout << "\e[32m" << "Connexion reussie!\e[0m" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void Irc::HandleClients(void)
{
	bool	disconnected;

	for (unsigned long i = 0; i < this->server.RefClients().size(); ++i)
	{
		disconnected = false;
		Client*	client = this->server.RefClients()[i];
		int		clientFd = client->GetRemote()->Get();

		if (this->sync.CanRead(clientFd))
		{
			int	bytesReceived = 0;
			if (client->GetBufferSpaceAvailable() <= 1)
				disconnected = true;
			else
				bytesReceived = this->server.RecvFrom(i, client->GetBuffer(), client->GetBufferSpaceAvailable() - 1);
			if (client->GetBufferSpaceAvailable() <= 1)
				disconnected = true;
			else
			{
				client->BufferIndexAddBy(bytesReceived);
				client->GetBuffer()[0] = '\0';
			}

			if (bytesReceived <= 0 || disconnected == true)
			{
				this->DisconnectAnyone(client);
				std::cout << "\e[31mDéconnexion!\e[0m\n";
				i--;
			}
			else if (disconnected == false)
				Req::Check(this->sync, this->server, this->channels, client);
		}
	}
}

void
Irc::SendMessage(void)
{
	
}
Str
Irc::getMdp() const {
	return mdp;
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
