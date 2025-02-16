/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 19:28:27 by ymanchon         ###   ########.fr       */
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
# include "Poll.hpp"
# include "Server.hpp"
# include "FControl.hpp"
# include "Req.hpp"
# include "Channel.hpp"

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
	RecvMessage(void);
	void
	SendMessage(void);

private:
	Poll					event;
	Server					server;
	std::vector<Channel>	channels;
	std::vector<pollfd>		polls;
};

#endif
