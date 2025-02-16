/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:34:32 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 19:20:02 by ymanchon         ###   ########.fr       */
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

Channel::Channel(const Channel::Str& pName, const Str& pPass) : name(pName), pass(pPass)
{
}

Channel::~Channel()
{
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

void
Channel::SetName(const Str& pName)
{
	this->name = pName;
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

