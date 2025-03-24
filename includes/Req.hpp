/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:33:17 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/24 16:20:19 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
#define REQ_HPP

# include <istream>
# include <streambuf>
# include <cstdlib> 
# include "Server.hpp"
# include "Channel.hpp"
# include "Irc.hpp"
# include "Select.hpp"

#define DEF_COLOR "\033[0;39m"
#define GREY "\033[0;90m"
#define RED "\033[0;91m"
#define GREEN "\033[0;92m"
#define YELLOW "\033[0;93m"
#define BLUE "\033[0;94m"
#define MAGENTA "\033[0;95m"
#define CYAN "\033[0;96m"
#define WHITE "\033[0;97m"
#define ORANGE "\033[38;5;214m"
#define RESET   "\033[0m"

//class Irc;

# define REQ_PARAMS	Select& select, Server& server, std::vector<Channel>& channels, Client* client, char* req
# define REQ_DATA	select, server, channels, client, req
# define UNUSED_REQ_PARAMS	(void)select, (void)server; (void)channels, (void)client; (void)req;
# define REQ_COUNT	15

typedef std::string Str;

namespace Req
{
	extern Str currentLine;
	void
	Check(REQ_PARAMS);

	void
	__USER(REQ_PARAMS);
	void
	__PASS(REQ_PARAMS);
	void
	__CAP(REQ_PARAMS);
	void
	__NICK(REQ_PARAMS);
	void
	__KICK(REQ_PARAMS);
	void
	__INVITE(REQ_PARAMS);
	void
	__TOPIC(REQ_PARAMS);
	void
	__MODE(REQ_PARAMS);
	void
	__JOIN(REQ_PARAMS);
	void 
	__PRIVMSG(REQ_PARAMS);
	void 
	__QUIT(REQ_PARAMS);
	void 
	__PART(REQ_PARAMS);
	void 
	__PING(REQ_PARAMS);
	void 
	__PONG(REQ_PARAMS);
	void
	__WHO(REQ_PARAMS);


	extern const unsigned int	nReqfun;
	extern const Str			reqname[REQ_COUNT];
	extern void (*reqfun[REQ_COUNT])(REQ_PARAMS);
};

#endif
