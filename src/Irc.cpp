/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 19:30:32 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(int port, const char* pass) : server(port)
{
	this->server.Listen();

	FControl::SetFlags(this->server.Get(), FControl::NonBlock);
	this->event.AddFd("server", this->server.Get(), Poll::ReadReq);
	while (1)
	{
		this->event.Events(0);
		this->RecvMessage();
		if (this->event.ReadRequest("server"))
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
		Str					nickname;
		itos << "client" << this->server.RefClients().size();

		this->server.Accept(itos.str());
		Client*	localClient = this->server.FindClientByName(itos.str());

		this->HandleClientConnexion(localClient);
		nickname = localClient->GetNick();
		this->event.AddFd(nickname, localClient->GetRemote()->Get(), Poll::WriteReq);

		std::cout << "\e[32m" << nickname << " is successfuly connected!\e[0m" << std::endl;
	}
	catch (...)
	{
		std::cout << "\e[31mConnexion echouee...\e[0m" << std::endl;
	}
}

void
Irc::RecvMessage(void)
{
	char	message[512];

	for (unsigned long i = 0 ; i < this->server.RefClients().size() ; ++i)
	{
		try
		{
			std::stringstream	itos;
			itos << i;
			if (this->event.WriteRequest(this->server.RefClients()[i]->GetName()))
			{
				this->server.RecvFrom(i, message, 512);
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
