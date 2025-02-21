/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:33:17 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/21 15:34:34 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
#define REQ_HPP

# include <istream>
# include <streambuf>
# include "Server.hpp"
# include "Channel.hpp"
# include "Irc.hpp"
# include "Select.hpp"

//class Irc;

# define REQ_PARAMS	Select& select, Server& server, std::vector<Channel>& channels, Client* client, char* req
# define REQ_DATA	select, server, channels, client, req
# define UNUSED_REQ_PARAMS	(void)select, (void)server; (void)channels, (void)client; (void)req;
# define REQ_COUNT	9

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

	extern const unsigned int	nReqfun;
	extern const Str			reqname[REQ_COUNT];
	extern void (*reqfun[REQ_COUNT])(REQ_PARAMS);
};

#endif
