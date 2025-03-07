/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:28:02 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/07 16:05:44 by bama             ###   ########.fr       */
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
