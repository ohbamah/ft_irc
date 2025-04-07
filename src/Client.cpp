/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:28:02 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/07 16:09:24 by bama             ###   ########.fr       */
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
	this->invisible = false;
	this->FlushBuffer();
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

const Client::Str&
Client::GetServername(void) const
{
	return (this->servername);
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

void 
Client::AddChannel(Channel* channel) 
{    
    if (std::find(channels.begin(), channels.end(), channel) == channels.end()) {
        channels.push_back(channel);
    }
}

void 
Client::RemoveChannel(Channel* channel) 
{
    std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
    if (it != channels.end()) {
        channels.erase(it);
    }
}

void
Client::SetInvisible(bool invisible)
{
	this->invisible = invisible;
}

void
Client::FlushBuffer(void)
{
	this->message_index = 0;
	std::memset(this->message, 0, IRC_MSG_SIZE);
}

// BufferSize available to store data
// If return value is ONE or ZERO, there is no space available
unsigned int
Client::GetBufferSpaceAvailable(void) const
{
	return (IRC_MSG_SIZE - this->message_index);
}

char*
Client::GetMessage(void)
{
	return (this->message);
}

char*
Client::GetBuffer(void)
{
	return (&this->message[this->message_index]);
}

void
Client::ResetBufferIndex(void)
{
	this->message_index = 0;
}

void
Client::BufferIndexAddBy(unsigned int bytes)
{
	this->message_index += bytes;
}

void
Client::ResizeBuffer(unsigned int req1_size)
{
	//char	tmp[IRC_MSG_SIZE] = {0};
	//int		size = strlen(&this->message[req1_size]);
	//std::memcpy(tmp, &this->message[req1_size], size);
	//std::memset(this->message, 0, IRC_MSG_SIZE);
	//std::memcpy(this->message, tmp, size);
	std::size_t	size = strlen(&this->message[req1_size]);
	std::memcpy(this->message, &this->message[req1_size], size);
	std::memset(&this->message[size], 0, IRC_MSG_SIZE - size - 1);
}

unsigned int
Client::GetBufferIndex(void)
{
	return (this->message_index);
}

void
Client::SetBufferIndex(unsigned int bytes)
{
	this->message_index = bytes;
}
