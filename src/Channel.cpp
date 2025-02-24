/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:34:32 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/24 16:48:29 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Modes::Modes(void) : invitOnly(false), topicAdmOnly(false), passNeeded(false), userLimit(0)
{
}

Channel::Modes::~Modes()
{
}

Channel::Channel(const Channel::Str& pName) : name(pName) 
{
}

Channel::Channel(const Channel::Str& pName, const Str& pPass) : name(pName), pass(pPass), topic(""), max_clients(50), invite_only(false) 
{
}

Channel::~Channel()
{
}

std::string const & Channel::getName() const{
	return name;
}


void
Channel::Disconnect(Client* c)
{
	this->KickUser(c);
}

void
Channel::AddUser(Client* c)
{
	if (std::find(this->users.begin(), this->users.end(), c) != this->users.end())
	{
		this->users.push_back(c);
		this->invitations.erase(std::find(this->invitations.begin(), this->invitations.end(), c));
	}
	else
		throw (Channel::UserAlreadyInChannel());
}
void
Channel::ElevateUser(Client* c)
{
	if (std::find(this->users.begin(), this->users.end(), c) != this->users.end())
		this->admin.push_back(c);
	else
		throw (Channel::UserNotFound());
}
void
Channel::RevokeUser(Client* c)
{
	if (std::find(this->users.begin(), this->users.end(), c) != this->users.end())
		this->admin.erase(std::find(this->admin.begin(), this->admin.end(), c));
	else
		throw (Channel::UserNotFound());
}
void
Channel::KickUser(Client* c)
{
	
}
void
Channel::InviteUser(Client* c)
{
	if (std::find(this->users.begin(), this->users.end(), c) == this->users.end())
		this->invitations.push_back(c);
	else
		throw (Channel::UserAlreadyInChannel());
}

const Channel::Modes&
Channel::GetModes(void)
{
	return (this->modes);
}
const Channel::Str&
Channel::GetName(void) const
{
	return (this->name);
}

const Channel::Str&
Channel::GetPass(void) const
{
	return (this->pass);
}

std::vector<Client*> 
Channel::GetUsers() const
{
    return(this->users);
}

const std::string& 
Channel::GetTopic() const{
	return (this->topic);
}

void
Channel::SetTopic(std::string topic){
	this->topic = topic;
}

void
Channel::SetName(const std::string& pName)
{
	this->name = pName;
}

bool 
Channel::IsMember(Client* client) const {
    return std::find(users.begin(), users.end(), client) != users.end();
}


Channel::Modes&
Channel::RefModes(void)
{
	return (this->modes);
}

std::vector<Client*>&
Channel::RefUsers(void)
{
	return (this->users);
}

std::vector<Client*>&
Channel::RefAdmin(void)
{
	return (this->admin);
}

std::vector<Client*>&
Channel::RefInvitations(void)
{
	return (this->invitations);
}

bool 
Channel::isBanned(Client* c)
{
	return std::find(banned_clients.begin(), banned_clients.end(), c) != banned_clients.end();
}

bool 
Channel::isInviteOnly() const 
{
	return invite_only; 
}

bool 
Channel::isInvited(Client* c) 
{
	return std::find(invitations.begin(), invitations.end(), c) != invitations.end();
}

bool
Channel::isFull() const 
{ 
	return users.size() >= max_clients; 
}

bool 
Channel::hasKey() const 
{ 
	return !pass.empty();
}
