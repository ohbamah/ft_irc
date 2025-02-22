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

Str Req::currentLine; 

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

void Req::Check(REQ_PARAMS)
{
    while (!((currentLine = get_line(&req)).empty()))
    {
        std::cout << currentLine << std::endl;

        size_t spacePos = currentLine.find_first_of(' ');

        Str cmd;
        if (spacePos == std::string::npos)
            cmd = currentLine;
        else
            cmd = Str(currentLine.begin(), currentLine.begin() + spacePos);

        Str input;
        if (spacePos != std::string::npos && spacePos + 1 < currentLine.length())
            input = Str(currentLine.begin() + spacePos + 1, currentLine.end());

        std::cout << "Commande: " << cmd << " | Arguments: " << input << std::endl;

        bool found = false;
        for (int i = 0; i < REQ_COUNT; ++i)
        {
            if (!reqname[i].compare(cmd))
            {
                reqfun[i](REQ_DATA);
                found = true;
                break;
            }
        }

        if (!found)
            std::cout << "Commande inconnue : " << cmd << std::endl;
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

static bool	isForbidden(char c)
{
	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!'
		|| c == '@' || c == '.')
		return (true);
	else
		return (false);
}

static bool	containsInvalidCharacters(std::string nickname)
{
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return (true);
	
	for (size_t i = 0; i < nickname.size(); i++) {
			if (isForbidden(nickname[i]) == true)
				return (true);
	}
	return (false);			
}

void Req::__NICK(REQ_PARAMS) {
    size_t spacePos = currentLine.find_first_of(' ');
    if (!client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string newNick = currentLine.substr(spacePos + 1);
    if (containsInvalidCharacters(newNick)) {
        std::string errorMessage = ":localhost 432 " + newNick + " :Erroneous nickname\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
   
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        std::string errorMessage = ":localhost 431 " + client->GetName() + " :No nickname given\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (server.IsNicknameTaken(newNick)) {
        std::string errorMessage = ":localhost 433 " + newNick + " :Nickname is already in use\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string oldNick = client->GetName();
    client->SetNick(newNick);
    oldNick = client->GetName();
    std::cout << oldNick;
    if (oldNick != newNick) {
        std::string successMessage = ":localhost " + oldNick + " changed his nickname to " + newNick + "\r\n";
        server.Broadcast(successMessage, client, &select);
    }
}


void Req::__PASS(REQ_PARAMS)
{
    if (client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos) {
        spacePos = currentLine.length();
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string password = currentLine.substr(spacePos + 1);
    if (password != server.GetPassword()) {
        std::string errorMessage = ":localhost 464 " + client->GetName() + " :Password incorrect\r\n";
        //server.SendTo(client->GetName(), errorMessage, 512);
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        client->SetDisconnect(true); 
        return;
    }
    
    client->SetAuthenticated(true);
	std::cout << client->GetAuthenticated() << std::endl;
}


void
Req::__TOPIC(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}


// Parameters: <username> <hostname> <servername> <realname>
void Req::__USER(REQ_PARAMS)
{    
    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    Str username = Str(currentLine.begin() + spacePos + 1, currentLine.end());
    if (client->GetUser() != "") {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    client->SetUser(username);
    std::string welcomeMessage = ":localhost 001 " + client->GetName() + " :Welcome to the IRC server\r\n";
    if (select.CanWrite(client->GetRemote()->Get()))
        send(client->GetRemote()->Get(), welcomeMessage.c_str(), welcomeMessage.size(), 0);
    
    std::string joinMessage = ":" + client->GetName() + " has joined\r\n";
    server.Broadcast(joinMessage, client, &select);
}


#define REQ_PARAMS const char* line, Server& server, Client* client
