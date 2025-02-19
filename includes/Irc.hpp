/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:35:52 by ymanchon         ###   ########.fr       */
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

private:
	//Poll					event;
	std::vector<pollfd>		pfd;
	Server					server;
	std::vector<Channel>	channels;
	std::vector<pollfd>		polls;
};

#endif
