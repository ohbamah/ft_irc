/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:26:14 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/25 15:29:59 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <vector>
# include <algorithm>
# include <string>
# include <iomanip>
# include <iostream>
# include "Client.hpp"

class Channel
{
	typedef std::string	Str;

public:
	struct Modes
	{
		Modes(void);
		~Modes();

		bool	invitOnly;
		bool	topicAdmOnly;
		bool	passNeeded;
		int		userLimit;
	};

public:
	Channel(const Str& name);
	Channel(const Str& name, const Str& pass);
	~Channel();

	std::string const & getName() const;

	void
	Disconnect(Client* c);

	void
	AddUser(Client* c);

	void
	ElevateUser(Client* c);

	void
	RevokeUser(Client* c);

	void
	KickUser(Client* c);

	void
	InviteUser(Client* c);

	const Channel::Modes&
	GetModes(void);
	
	const Str&
	GetName(void) const;

	const Str&
	GetPass(void) const;
	
	std::vector<Client*> 
	GetUsers() const;

	const std::string& 
	GetTopic() const;

	bool 
	IsMember(Client* client) const;

	void
	SetName(const std::string& name);
	
	void
	SetTopic(std::string topic);

	Channel::Modes&
	RefModes(void);
	std::vector<Client*>&
	RefUsers(void);
	std::vector<Client*>&
	RefAdmin(void);
	std::vector<Client*>&
	RefInvitations(void);

	bool isBanned(Client *c);
	bool isInviteOnly() const;
	bool isInvited(Client* c);
	bool isFull() const;
	bool hasKey() const;
	
private:
	Str						name;
	Str						pass;
	Str						topic;
	Modes					modes;
	std::vector<Client*>	users;
	std::vector<Client*>	admin;
	std::vector<Client*>	invitations;
	std::vector<Client *>	banned_clients;
	size_t					max_clients;
	bool					invite_only;
	

public:
	struct UserNotFound : std::exception { inline virtual const char* what(void) const throw() {return ("User not found in channel.\n"); } };
	struct UserAlreadyInChannel : std::exception { inline virtual const char* what(void) const throw() {return ("User is already in channel.\n"); } };
};

#endif
