/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:25:46 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/31 01:41:47 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "Socket.hpp"
# include "Channel.hpp"

class Channel ;

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
	SetUser(const Str& pUser);
	void
	SetName(const Str& pName);
	void
	SetRealname(const Str& realname);
	void
	SetHostname(const Str& hostname);
	void
	SetServername(const Str& servername);
	void 
	SetDisconnect(bool disconnect);
	void 
	SetAuthenticated(bool authenticated);
	bool 
	isOperator(Channel* channel);

	void
	FlushBuffer(void);
	unsigned int
	GetBufferSize(void) const;
	char*
	GetMessage(void);
	char*
	GetBuffer(void);
	void
	ResetBufferIndex(void);
	void
	BufferIndexAddBy(unsigned int bytes);
	void
	SetBufferIndex(unsigned int bytes);
	void
	ResizeBuffer(unsigned int begin, unsigned int end);

private:
	SocketRemote*	remote;
	Str				nick;	// unique
	Str				user;
	Str				name;
	Str				realname;
	Str				hostname;
	Str				servername;
	bool			disconnect;
	bool 			authenticated;
	std::vector<Channel*> operatorChannels;
	char			message[512];
	unsigned int	message_index;
};

#endif
