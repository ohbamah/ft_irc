/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:36:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/11 15:59:08 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FControl.hpp"

int
Server::cpt = 0;

Server::Server(void) : Socket(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
}

Server::Server(int port, int pFControlFlags) : Socket(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto)
{
    this->Start(port, pFControlFlags);
}

Server::~Server()
{
    for (std::vector<Client*>::iterator it = this->clients.begin() ; it != this->clients.end() ; ++it)
    {
        this->clients.erase(it);
        (*it)->GetRemote()->Close();
        delete (*it);
    }
}

void
Server::Start(int port, int pFControlFlags)
{
    (void)pFControlFlags;
	this->Socket::SetOptions(SO_REUSEADDR | SO_REUSEPORT);
    try {
	    this->Socket::Bind(port);
	} catch (...)  {
        return ;
    }
    //FControl::SetFlags(this->Socket::Get(), pFControlFlags);
    fcntl(this->Socket::Get(), F_SETFL, O_NONBLOCK);
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
		if (!this->clients[i]->GetNick().compare(name))
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
    std::map<std::string, Channel *>::const_iterator it = channels.find(channelName);
    if (it != channels.end()) {
        return it->second;
    }
    return NULL;
}

void 
Server::SetChannel(const std::string& channelName, Channel* channel) 
{
    channels[channelName] = channel;
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

Channel* Server::FindChannel(std::string const & name) {
    std::map<std::string, Channel*>::iterator it = channels.find(name);
    if (it != channels.end()) {
        return it->second;
    }
    return NULL;
}

Client* Server::FindClient(const std::string& name) const {
    for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->GetNick() == name) {
            return *it;
        }
    }
    return NULL;
}

void Server::CreateChannel(const std::string& channelName) {
    
    Channel* newChannel = new Channel(channelName);
    channels[channelName] = newChannel;
}

void Server::sendChanInfos(Client *client, Channel *channel)
{
    if (!channel || !client)
        return;

    std::string nickname = client->GetNick();
    std::string channelName = channel->GetName();

    std::string listOfMembers;
    std::vector<Client*> users = channel->GetUsers();
    for (size_t i = 0; i < users.size(); ++i)
    {
        if (std::find(channel->RefAdmin().begin(), channel->RefAdmin().end(), users[i]) != channel->RefAdmin().end())
            listOfMembers += "@" + users[i]->GetNick() + " ";
        else
            listOfMembers += users[i]->GetNick() + " ";
    }
    
    if (!listOfMembers.empty())
    listOfMembers.erase(listOfMembers.size() - 1, 1);
    std::string joinMessage = ":" + client->GetNick() + "!" + client->GetNick() + "@localhost JOIN " + channelName + "\r\n";
    send(client->GetRemote()->Get(), joinMessage.c_str(), joinMessage.size(), 0);
    
    // Envoie de la réponse RPL_NAMREPLY (353)
    std::string namReply = ":localhost 353 " + nickname + " = " + channelName + " :" + listOfMembers + "\r\n";
    send(client->GetRemote()->Get(), namReply.c_str(), namReply.size(), 0);

    // Envoie de la réponse RPL_ENDOFNAMES (366)
    std::string endOfNames = ":localhost 366 " + nickname + " " + channelName + " :End of /NAMES list\r\n";
    send(client->GetRemote()->Get(), endOfNames.c_str(), endOfNames.size(), 0);
}

Client* Server::getClientByNick(const std::string& nick) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->GetNick() == nick)
            return clients[i];
    }
    return NULL;
}

std::vector<Channel*> Server::GetChannelsOfClient(Client* client) {
    std::vector<Channel*> channelsList;
    for (std::map<std::string, Channel*>::iterator it = this->channels.begin(); it != this->channels.end(); ++it) {
        if (it->second->HasClient(client)) {
            channelsList.push_back(it->second);
        }
    }
    return channelsList;
}

void Server::DeleteChannel(const std::string& channelName) {
    std::cout << "[Server::DeleteChannel] Suppression du channel avec le nom : " << channelName << std::endl;

    std::map<std::string, Channel*>::iterator it = channels.find(channelName);
    if (it != channels.end()) {
        channels.erase(it);
        std::cout << "[Server::DeleteChannel] Channel " << channelName << " supprimé avec succès." << std::endl;
    } else {
        std::cout << "[Server::DeleteChannel] Channel " << channelName << " non trouvé." << std::endl;
    }
}