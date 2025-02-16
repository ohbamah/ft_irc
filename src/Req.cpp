/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:37:41 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 19:32:30 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"

const unsigned int
Req::nReqfun = REQ_COUNT;

const Str
Req::reqname[REQ_COUNT] = {"CAP", "INVITE", "JOIN", "KICK", "MODE", "NICK", "PASS", "TOPIC", "USER"};

void
(*Req::reqfun[REQ_COUNT])(REQ_PARAMS) = {Req::__CAP, Req::__INVITE, Req::__JOIN, Req::__KICK, Req::__MODE, Req::__NICK, Req::__PASS, Req::__TOPIC, Req::__USER};

static Str	get_line(char** txt)
{
	Str	ret;

	for (int i = 0 ; **txt && **txt != '\n' ; ++i)
		ret.push_back(*((*txt)++));
	return (ret);
}

void
Req::Check(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
	Str	line;
	Str	cmd;

	while (!((line = get_line(&req)).empty()))
	{
		cmd = Str(line.begin(), line.begin() + line.find_first_of(' '));
		//std::cout << "Commande : \"" << cmd << "\"" << std::endl;
		for (int i = 0 ; i < REQ_COUNT ; ++i)
			if (!Req::reqname[i].compare(cmd))
				Req::reqfun[i](REQ_DATA);
	}
}

void
Req::__CAP(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
	server.SendTo(client->GetName(), "CAP END");
}

void
Req::__INVITE(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
	//channelsInviteUser(client);
}

void
Req::__JOIN(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}

void
Req::__KICK(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}

void
Req::__MODE(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}

void
Req::__NICK(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
	Str	line = req;
	std::cout << "NICK : " << req;
	Str	nickname = Str(line.begin() + line.find_first_of(' '), line.begin() + line.find_first_of('\n'));
	client->SetNick(nickname);
}

void
Req::__PASS(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}

void
Req::__TOPIC(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}

void
Req::__USER(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}
