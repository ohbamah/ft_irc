/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:26:14 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 17:23:32 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <vector>
# include <algorithm>
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

	void
	SetName(const Str& name);

	Channel::Modes&
	RefModes(void);
	std::vector<Client*>&
	RefUsers(void);
	std::vector<Client*>&
	RefAdmin(void);
	std::vector<Client*>&
	RefInvitations(void);

private:
	Str						name;
	Str						pass;
	Modes					modes;
	std::vector<Client*>	users;
	std::vector<Client*>	admin;
	std::vector<Client*>	invitations;

public:
	struct UserNotFound : std::exception { inline virtual const char* what(void) const throw() {return ("User not found in channel.\n"); } };
	struct UserAlreadyInChannel : std::exception { inline virtual const char* what(void) const throw() {return ("User is already in channel.\n"); } };
};

#endif
