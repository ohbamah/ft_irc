/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 02:37:01 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include <iostream>
# include <signal.h>
# include <sys/stat.h>
# include <cstdlib>
# include <vector>
#include "Poll.hpp"
#include "Socket.hpp"
#include "FControl.hpp"

class Irc
{
public:
	Irc(int port, const char* pass);
	~Irc();

	void
	AcceptConnexion(void);
	void
	RecvMessage(void);
	void
	SendMessage(void);

private:
	Poll						ircPoll;
	Socket						server;
	std::vector<SocketRemote>	clients;
};

#endif
