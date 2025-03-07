/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/07 16:17:53 by bama             ###   ########.fr       */
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
	while (Irc::exitReq == false) {
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
Irc::HandleClientConnexion(Client* local)
{
	char	message[512];
	local->GetRemote()->Recv(message);
	Req::Check(this->sync, this->server, this->channels, local, message);
}

void
Irc::AcceptConnexion(void)
{
	try
	{
		std::stringstream	itos;
		itos << "client" << Server::cpt++;

		std::cout << "accept()\n";
		this->server.Accept(itos.str());
		std::cout << "accept finished()\n";

		Client*	localClient = this->server.FindClientByName(itos.str()); // ByNickName
		this->sync.AddReadReq(localClient->GetRemote()->Get());
		this->sync.AddWriteReq(localClient->GetRemote()->Get());
		this->sync.AddExcpReq(localClient->GetRemote()->Get());
		std::cout << "\e[32m" << "Connexion reussie!\e[0m" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		//std::cout << "\e[31mConnexion echouee...\e[0m" << std::endl;
	}
}

// void
// Irc::HandleClients(void)
// {
// 	char	message[512];

// 	for (unsigned long i = 0 ; i < this->server.RefClients().size() ; ++i)
// 	{
// 		try
// 		{
// 			std::stringstream	itos;
// 			itos << i;
// 			int clientFd = this->server.RefClients()[i]->GetRemote()->Get();
// 			if (this->sync.Exception(clientFd))
// 			{
// 				this->DisconnectAnyone(this->server.RefClients()[i]);
// 				i = 0;
// 			}
// 			else if (this->sync.CanRead(clientFd))
// 			{
// 				if (this->server.RecvFrom(i, message, 512) <= 0)
// 				{
// 					this->DisconnectAnyone(this->server.RefClients()[i]);
// 					i = 0;
// 					std::cout << "\e[31mDeconnexion!\e[0m\n";
// 					//std::exit(0);
// 				}
// 				std::cout << message << std::endl;
// 			}
// 		}
// 		catch (Socket::FailedRecv& e)
// 		{
// 		}
// 	}
// }

void Irc::HandleClients(void) {
    char message[512];

    for (unsigned long i = 0; i < this->server.RefClients().size(); ++i) {
        int clientFd = this->server.RefClients()[i]->GetRemote()->Get();

        if (this->sync.CanRead(clientFd)) {
            int bytesReceived = this->server.RecvFrom(i, message, sizeof(message) - 1);

            if (bytesReceived <= 0) {
                this->DisconnectAnyone(this->server.RefClients()[i]);
                std::cout << "\e[31mDéconnexion!\e[0m\n";
                i--;
            } else {
                message[bytesReceived] = '\0';
               // std::cout << "Message reçu : " << message;
                Req::Check(this->sync, this->server, this->channels, this->server.RefClients()[i], message);
            }
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
