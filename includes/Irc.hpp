/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/21 14:55:24 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include <iostream>
# include <signal.h>
# include <sys/stat.h>
# include <cstdlib>
# include <vector>
# include <sstream>
# include "Server.hpp"
# include "FControl.hpp"
# include "Req.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Select.hpp"

typedef std::string Str;

class Irc
{
public:
	Irc(int port, const char* pass);
	~Irc();

	void
	HandleClientConnexion(Client* local);
	void
	AcceptConnexion(void);
	void
	HandleClients(void);
	void
	SendMessage(void);
	Str
	getMdp() const;
	void
	DisconnectAnyone(Client* c);

private:
	Select					sync;
	Server					server;
	std::vector<Channel>	channels;
	std::vector<pollfd>		polls;
	Str 					mdp;
};

#endif
