/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:28:02 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/11 15:50:53 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Client::Str pName, SocketRemote* pRemote)
{
	this->remote = pRemote;
	this->name = pName;
	this->nick = pName; // default nick
	this->realname = "";
	this->hostname = "";
	this->servername = "";
	this->authenticated = false;
	this->disconnect = false;
	this->hasSetNick = false;
}

Client::~Client()
{
	delete(this->remote);
}

const Client::Str&
Client::GetName(void) const
{
	return (this->name);
}

const Client::Str&
Client::GetNick(void) const
{
	return (this->nick);
}

const Client::Str&
Client::GetHostname(void) const
{
	return (this->hostname);
}

const Client::Str&
Client::GetUser(void) const
{
	return (this->user);
}

SocketRemote*
Client::GetRemote(void) const
{
	return (this->remote);
}

bool 
Client::GetAuthenticated() const
{
	return (this->authenticated);
}

void
Client::Disconnect(void)
{
	this->GetRemote()->Close();
}

void
Client::SetName(const Client::Str& pName)
{
	this->name = pName;
}

void
Client::SetNick(const Client::Str& pNick)
{
	this->nick = pNick;
}

void
Client::SetUser(const Client::Str& pUser)
{
	this->user = pUser;
}

void
Client::SetRealname(const Client::Str& realname)
{
	this->realname = realname;
}

void
Client::SetHostname(const Client::Str& hostname)
{
	this->hostname = hostname;
}

void
Client::SetServername(const Client::Str& servername)
{
	this->servername = servername;
}

void 
Client::SetDisconnect(bool disconnect)
{
	this->disconnect = disconnect;
}

void 
Client::SetAuthenticated(bool authenticated)
{
	this->authenticated = authenticated;
}

bool 
Client::isOperator(Channel* channel)
{
    return std::find(operatorChannels.begin(), operatorChannels.end(), channel) != operatorChannels.end();
}
bool 
Client::HasSetNick() const
{ 
	return hasSetNick; 
}

void 
Client::MarkNickAsSet() 
{ 
	hasSetNick = true; 
}

void Client::AddChannel(Channel* channel) {
    std::cout << "[Client::AddChannel] Ajout du channel: " << channel->GetName() << std::endl;
    
    if (std::find(channels.begin(), channels.end(), channel) == channels.end()) {
        channels.push_back(channel);
        std::cout << "[Client::AddChannel] Channel ajouté avec succès: " << channel->GetName() << std::endl;
    } else {
        std::cout << "[Client::AddChannel] Le client est déjà dans ce channel." << std::endl;
    }
}

void Client::RemoveChannel(Channel* channel) {
    std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    } else {
    }
}