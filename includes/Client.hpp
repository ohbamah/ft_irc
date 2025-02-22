/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:25:46 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/21 13:40:25 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Socket.hpp"

class Client
{
public:
	typedef std::string	Str;

public:
	Client(Str nick, SocketRemote* remote);
	~Client();

	const Str&
	GetNick(void) const;
	const Str&
	GetUser(void) const;
	const Str&
	GetName(void) const;
	SocketRemote*
	GetRemote(void) const;
	bool
	GetAuthenticated(void) const;

	void
	Disconnect(void);
	void
	SetNick(const Str& nick);
	void
	SetUser(const Str& nick);
	void
	SetName(const Str& name);

	void 
	SetDisconnect(bool disconnect);
	void 
	SetAuthenticated(bool authenticated);

private:
	SocketRemote*	remote;
	Str				nick;	// unique
	Str				user;
	Str				name;
	bool			disconnect;
	bool 			authenticated;
};

#endif
