/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 17:29:49 by ymanchon         ###   ########.fr       */
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
	void
	DisconnectAnyone(Client* c);

private:
	//Poll					event;
	//int						max_fd;		// Creer class Select
	//std::vector<fd_set>		sfd_rd;		// Creer class Select | le [0] est toujours le server
	//std::vector<fd_set>		sfd_err;	// Creer class Select | le [0] est toujours le server
	Select					sync;
	Server					server;
	std::vector<Channel>	channels;
	std::vector<pollfd>		polls;
};

#endif
