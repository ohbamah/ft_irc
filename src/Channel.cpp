/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:34:32 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/24 14:51:47 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Modes::Modes(void) : invitOnly(false), topicAdmOnly(false), passNeeded(false), userLimit(0)
{
}

Channel::Modes::~Modes()
{
}

Channel::Channel(const Channel::Str& pName) : name(pName), pass(""), topic(""), max_clients(50), invite_only(false), isPermanent(false)
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

void Channel::AddUser(Client* c)
{
    if (std::find(this->users.begin(), this->users.end(), c) != this->users.end())
        throw Channel::UserAlreadyInChannel();
    
    this->users.push_back(c);
    std::vector<Client*>::iterator it = std::find(this->invitations.begin(), this->invitations.end(), c);
    if (it != this->invitations.end())
        this->invitations.erase(it);
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
    std::vector<Client*>::iterator it = std::find(this->users.begin(), this->users.end(), c);
    if (it != this->users.end())
    {
        this->users.erase(it);

        std::vector<Client*>::iterator adminIt = std::find(this->admin.begin(), this->admin.end(), c);
        if (adminIt != this->admin.end())
            this->admin.erase(adminIt);
    }
    else
        throw Channel::UserNotFound();
}

// equivalent to RevokeUser
void
Channel::KickUser(Client* c)
{
	this->RevokeUser(c);	
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

bool 
Channel::isOperator(Client* client) const 
{
	return std::find(admin.begin(), admin.end(), client) != admin.end();
}

void 
Channel::setOperator(Client* client) {
    if (!client)
        return;

    for (size_t i = 0; i < admin.size(); i++) 
	{
        if (admin[i] == client)
            return;
    }
    admin.push_back(client);
}

void
Channel::SetPass(Str pass){
	this->pass = pass;
}

void 
Channel::removeOperator(Client* client) {
	for (size_t i = 0; i < admin.size(); ++i) {
		if (admin[i] == client) {
			admin.erase(admin.begin() + i);
			break;
		}
	}
}

bool 
Channel::isTopicRestricted() const 
{ 
	return topicRestricted;
}

void 
Channel::setTopicRestricted(bool restricted) 
{ 
	topicRestricted = restricted; 
}

int Channel::getMaxClients() const 
{ 
	return max_clients; 
}

void Channel::setMaxClients(int max) 
{ 
	max_clients = max; 
}

void
Channel::SetInviteOnly(bool value)
{
	this->invite_only = value;
}

void 
Channel::broadcastMessage(Client* sender, const std::string& message, Select& select) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i] != sender) {
            std::string fullMessage = ":" + sender->GetNick() + " PRIVMSG " + name + " :" + message + "\r\n";
            if (select.CanWrite(users[i]->GetRemote()->Get())) {
                send(users[i]->GetRemote()->Get(), fullMessage.c_str(), fullMessage.size(), 0);
            }
        }
    }
}

bool 
Channel::HasClient(Client* client) 
{
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i] == client) {
			return true;
		}
	}
	return false;
}

bool 
Channel::IsEmpty() const 
{
    return users.empty();
}

bool
Channel::IsPermanent() const 
{
    return isPermanent;
}

void Channel::UpdateNick(Client *client, const std::string &newNick) {
    std::vector<Client*>::iterator it = std::find(users.begin(), users.end(), client);
    if (it != users.end()) {
        (*it)->SetNick(newNick);
    }
}
