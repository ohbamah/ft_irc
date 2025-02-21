/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:37:41 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/21 15:38:45 by claprand         ###   ########.fr       */
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

Str Req::currentLine; 

void Req::Check(REQ_PARAMS)
{
    while (!((currentLine = get_line(&req)).empty()))
    {
        Str cmd = Str(currentLine.begin(), currentLine.begin() + currentLine.find_first_of(' '));
        for (int i = 0; i < REQ_COUNT; ++i)
		{
            if (!reqname[i].compare(cmd))
                reqfun[i](REQ_DATA);
			//std::cout << "TEST\n";
		}
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

void Req::__PASS(REQ_PARAMS)
{
	std::cout << client->GetAuthenticated() << std::endl;
    if (client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
		std::cout << errorMessage << std::endl;
        return;
    }

    size_t spacePos = currentLine.find(' ');
    if (spacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
		std::cout << errorMessage << std::endl;
        return;
    }

    std::string password = currentLine.substr(spacePos + 1);
    if (password != server.GetPassword()) {
        std::string errorMessage = ":localhost 464 " + client->GetName() + " :Password incorrect\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
		std::cout << errorMessage << std::endl;
        client->SetDisconnect(true); 
        return;
    }
    
    client->SetAuthenticated(true);
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

#define REQ_PARAMS const char* line, Server& server, Client* client

