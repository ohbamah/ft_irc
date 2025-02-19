/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:49:42 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

Irc::Irc(int port, const char* pass) : server(port, FControl::NonBlock)
{
	this->server.Listen();

	this->pfd.push_back(Utils::CreatePollfd(this->server.Get(), POLLIN));
	while (1)
	{
		std::cout << this->pfd.size() << std::endl;
		poll(&this->pfd[0], this->pfd.size(), 0);
		this->HandleClients();
		if (Utils::SearchPollfd(this->pfd, this->server.Get()).revents & POLLIN)
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
		this->pfd.push_back(Utils::CreatePollfd(localClient->GetRemote()->Get(), POLLIN | POLLOUT));

		std::cout << "\e[32m" << nickname << " is successfuly connected!\e[0m" << std::endl;
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

	for (unsigned long i = 0 ; i < this->server.RefClients().size() ; ++i)
	{
		try
		{
			std::stringstream	itos;
			itos << i;
			if (Utils::SearchPollfd(this->pfd, this->server.RefClients()[i]->GetRemote()->Get()).revents & (POLLHUP|POLLERR|POLLNVAL))
			{
				// del client* in std::vector of this->channels
				this->server.Disconnect(this->server.RefClients()[i]);
				std::cout << "POLLHUP\n";
			}
			else if (Utils::SearchPollfd(this->pfd, this->server.RefClients()[i]->GetRemote()->Get()).revents & (POLLIN|POLLOUT|POLLRDNORM|POLLRDBAND|POLLPRI|POLLWRNORM|POLLWRBAND))
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
