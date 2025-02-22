/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:36:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/20 15:54:45 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FControl.hpp"

Server::Server(int port, int pFControlFlags) : Socket(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
	this->Socket::SetOptions(SO_REUSEADDR | SO_REUSEPORT);
	this->Socket::Bind(port);
	FControl::SetFlags(this->Socket::Get(), pFControlFlags);
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
Server::Disconnect(const Client* ptr)
{
	std::vector<Client*>::iterator	it = std::find(this->clients.begin(), this->clients.end(), ptr);
	Client*	client = (*it);
	this->clients.erase(it);
	client->GetRemote()->Close();
	delete (client);
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
void
Server::SetPassword(std::string pass){
	this->password = pass;
}

std::string
Server::GetPassword() const{
	return password;
}

bool 
Server::IsNicknameTaken(const std::string& nickname) const
{
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it)->GetNick() == nickname) // (*it) pour accéder au Client*
            return true;
    }
    return false;
}


void 
Server::Broadcast(const std::string& message, Client* exclude, Select* select)
{
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client* client = *it; // Accéder au Client à l'aide de l'itérateur

        // Ne pas envoyer le message au client exclu
        if (client != exclude)
        {
            if (select->CanWrite(client->GetRemote()->Get()))
            {
                send(client->GetRemote()->Get(), message.c_str(), message.size(), 0);
            }
        }
    }
}

