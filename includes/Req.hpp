/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:33:17 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/31 01:19:27 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
#define REQ_HPP

# include <istream>
# include <streambuf>
# include <cstdlib>
# include <cstdio>
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

# define REQ_PARAMS	Select& select, Server& server, std::vector<Channel>& channels, Client* client
# define REQ_DATA	select, server, channels, client
# define UNUSED_REQ_PARAMS	(void)select, (void)server; (void)channels, (void)client;
# define REQ_COUNT	11

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

	extern const unsigned int	nReqfun;
	extern const Str			reqname[REQ_COUNT];
	extern void (*reqfun[REQ_COUNT])(REQ_PARAMS);
};

#endif
