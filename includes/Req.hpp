/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:33:17 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:31:01 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
#define REQ_HPP

# include <istream>
# include <streambuf>
# include "Server.hpp"
# include "Channel.hpp"
# include "Irc.hpp"

//class Irc;

# define REQ_PARAMS	Server& server, std::vector<Channel>& channels, Client* client, char* req
# define REQ_DATA	server, channels, client, req
# define UNUSED_REQ_PARAMS	(void)server; (void)channels, (void)client; (void)req;
# define REQ_COUNT	9

typedef std::string Str;

namespace Req
{
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
