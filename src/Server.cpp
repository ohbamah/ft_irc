/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:36:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/24 15:16:11 by claprand         ###   ########.fr       */
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
Server::SetPassword(std::string pass)
{
	this->password = pass;
}

std::string
Server::GetPassword() const{
	return password;
}

Channel* 
Server::GetChannel(const std::string& channelName) const 
{
    std::map<std::string, Channel *>::const_iterator it = channels.find(channelName); // Supposons que `channels` est un std::map ou std::unordered_map
    if (it != channels.end()) {
        return it->second; // Retourne le pointeur vers le canal.
    }
    return NULL; // Retourne nullptr si le canal n'existe pas.
}

void 
Server::SetChannel(const std::string& channelName, Channel* channel) 
{
    channels[channelName] = channel; // Supposons que `channels` est un std::map<std::string, Channel*>.
}


bool 
Server::IsNicknameTaken(const std::string& nickname) const
{
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it)->GetNick() == nickname)
            return true;
    }
    return false;
}


void 
Server::Broadcast(const std::string& message, Client* exclude, Select* select)
{
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        Client* client = *it; 
        if (client != exclude)
        {
            if (select->CanWrite(client->GetRemote()->Get()))
            {
                send(client->GetRemote()->Get(), message.c_str(), message.size(), 0);
            }
        }
    }
}

void 
Server::BroadcastToChannel(Channel* channel, const std::string& message, Select *select) 
{
    std::vector<Client*> users = channel->GetUsers();
    for (size_t i = 0; i < users.size(); i++) {
        Client* client = users[i];
        if (select->CanWrite(client->GetRemote()->Get())) {
            send(client->GetRemote()->Get(), message.c_str(), message.size(), 0);
        }
    }
}

Channel * Server::FindChannel(std::vector<Channel>& channels, std::string const & name){
   std::vector<Channel>::iterator it = channels.begin();
    if (it->getName() == name){
            return &(*it);
    }
    return NULL;
}