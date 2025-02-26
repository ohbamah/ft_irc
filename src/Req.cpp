/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:37:41 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/26 15:23:28 by claprand         ###   ########.fr       */
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
        size_t spacePos = currentLine.find_first_of(' ');

        Str cmd;
        if (spacePos == std::string::npos)
            cmd = currentLine;
        else
            cmd = Str(currentLine.begin(), currentLine.begin() + spacePos);

        Str input;
        if (spacePos != std::string::npos && spacePos + 1 < currentLine.length())
            input = Str(currentLine.begin() + spacePos + 1, currentLine.end());

        //std::cout << "Commande: " << cmd << " | Arguments: " << input << std::endl;

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
            std::cout << RED << "Commande inconnue : " << cmd << RESET << std::endl;
    }
}

void
Req::__CAP(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
	server.SendTo(client->GetName(), "CAP END");
}




/******************************************************/
/*                      INVITE                        */
/******************************************************/

bool is_valid_channel_name(const std::string &name){
    return !name.empty() && name[0] == '#';
}

// INVITE <nickname> <channel>
void Req::__INVITE(REQ_PARAMS)
{
    if (!client)
        return;
    
    std::stringstream ss(req);
    std::string command, targetNick, channelName;
    ss >> command >> targetNick >> channelName;

    if (targetNick.empty() || channelName.empty()) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " INVITE :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (!is_valid_channel_name(channelName)) {
        std::string errorMessage = ":localhost 403 " + client->GetName() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    Channel* channel = NULL;
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].getName() == channelName) {
            channel = &channels[i];
            break;
        }
    }
    
    if (!channel) {
        std::string errorMessage = ":localhost 403 " + client->GetName() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!channel->isOperator(client)) {
        std::string errorMessage = ":localhost 482 " + client->GetName() + " " + channelName + " :You're not channel operator\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    Client* targetClient = server.getClientByNick(targetNick);
    if (!targetClient) {
        std::string errorMessage = ":localhost 401 " + client->GetName() + " " + targetNick + " :No such nick/channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (channel->IsMember(targetClient)) {
        std::string errorMessage = ":localhost 443 " + client->GetName() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    channel->InviteUser(targetClient);
    std::string inviteMessage = ":localhost 341 " + client->GetName() + " " + targetNick + " " + channelName + "\r\n";
    if (select.CanWrite(client->GetRemote()->Get()))
        send(client->GetRemote()->Get(), inviteMessage.c_str(), inviteMessage.size(), 0);
    
    std::string targetMessage = ":" + client->GetName() + " INVITE " + targetNick + " " + channelName + "\r\n";
    if (select.CanWrite(targetClient->GetRemote()->Get()))
        send(targetClient->GetRemote()->Get(), targetMessage.c_str(), targetMessage.size(), 0);
}




/******************************************************/
/*                      JOIN                          */
/******************************************************/

void Req::__JOIN(REQ_PARAMS)
{
    if (currentLine.empty()) {
        std::cerr << "Error: currentLine is empty!" << std::endl;
        return;
    }
    
    std::istringstream iss(currentLine);
    std::string command, channelName, key;
    std::getline(iss, command, ' ');
    if (!std::getline(iss, channelName, ' ') || channelName.empty()) {
        std::cerr << "Error: Missing or invalid channel name!" << std::endl;
        return;
    }
    
    if (std::getline(iss, key)) {
        if (key.empty()) {
            key.clear();
        }
    }
    
    if (!client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
 
    if (channelName.empty()) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (!is_valid_channel_name(channelName)) {
        std::string errorMessage = ":localhost 403 " + client->GetName() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    Channel* channel = server.FindChannel(channelName);
    if (!channel) {
        std::cout << MAGENTA << "Channel not found. Creating new channel: " << channelName << RESET << std::endl;
        server.CreateChannel(channelName);
        channel = server.FindChannel(channelName);
        
        if (!channel) {
            std::cerr << RED << "Error: Channel creation failed!" << RESET << std::endl;
            return;
        }
        
    try {
    channel->AddUser(client);
    } catch (const Channel::UserAlreadyInChannel& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
    channel->ElevateUser(client);
    } catch (const Channel::UserNotFound& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
        
        std::cout << GREEN << "Channel created successfully." << RESET << std::endl;
    } else {
        std::cout << MAGENTA << "Channel found: " << channelName << RESET << std::endl;
    }
    
    if (channel->isFull()) {
        std::string errorMessage = ":localhost 471 " + client->GetName() + " " + channelName + " :Cannot join channel (+l)\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (channel->isInviteOnly() && !channel->isInvited(client)) {
        std::string errorMessage = ":localhost 473 " + client->GetName() + " " + channelName + " :Cannot join channel (+i)\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (channel->isBanned(client)) {
        std::string errorMessage = ":localhost 474 " + client->GetName() + " " + channelName + " :Cannot join channel (+b)\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (channel->hasKey() && channel->GetPass() != key) {
        std::string errorMessage = ":localhost 475 " + client->GetName() + " " + channelName + " :Cannot join channel (+k)\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    try {
        channel->AddUser(client);
    } catch (const Channel::UserAlreadyInChannel& e) {
        //std::cerr << "Error: " << e.what() << std::endl;
    }
    // channel->AddUser(client);
    server.sendChanInfos(client, channel);
}




/******************************************************/
/*                      KICK                          */
/******************************************************/

void
Req::__KICK(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}




/******************************************************/
/*                      MODE                          */
/******************************************************/

void
Req::__MODE(REQ_PARAMS)
{
	UNUSED_REQ_PARAMS
}



/******************************************************/
/*                      NICK                          */
/******************************************************/

static bool	
isForbidden(char c)
{
	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!'
		|| c == '@' || c == '.')
		return (true);
	else
		return (false);
}

static bool	
containsInvalidCharacters(std::string nickname)
{
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return (true);
	
	for (size_t i = 0; i < nickname.size(); i++) {
			if (isForbidden(nickname[i]) == true)
				return (true);
	}
	return (false);			
}

void 
Req::__NICK(REQ_PARAMS) 
{
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

    std::string oldNick = client->GetNick();
    if (oldNick.empty())
        oldNick = client->GetName();
    client->SetNick(newNick);
    std::cout << GREEN << oldNick + " changed his nickname to " + newNick << RESET << std::endl;
}




/******************************************************/
/*                      PASS                          */
/******************************************************/

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
    if (client->GetAuthenticated() == true)
        std::cout << "\e[32m" << "New client connected! Socket FD: " << client->GetRemote()->Get() << "\e[0m" << std::endl;

}




/******************************************************/
/*                      TOPIC                         */
/******************************************************/

void
Req::__TOPIC(REQ_PARAMS)
{
    size_t spacePos = currentLine.find(' '); 
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " TOPIC :Not enough parameters\r\n";
        send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
        std::string remaining = currentLine.substr(spacePos + 1);
        size_t nextSpacePos = remaining.find(' ');
    
        std::string channel = (nextSpacePos == std::string::npos) ? remaining : remaining.substr(0, nextSpacePos);
        std::string topic = (nextSpacePos == std::string::npos) ? "" : remaining.substr(nextSpacePos + 1);
    
        Channel* currentChannel = server.GetChannel(channel);
        if (!currentChannel) {
            std::string errorMessage = ":localhost 403 " + channel + " :No such channel\r\n";
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            return;
        }
    
        if (!currentChannel->IsMember(client)) {
            std::string errorMessage = ":localhost 442 " + channel + " :You're not on that channel\r\n";
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            return;
        }
    
        if (topic.empty()) {
            if (currentChannel->GetTopic().empty()) {
                std::string noTopicMessage = ":localhost 331 " + client->GetName() + " " + channel + " :No topic is set\r\n";
                send(client->GetRemote()->Get(), noTopicMessage.c_str(), noTopicMessage.size(), 0);
            } else {
                std::string topicMessage = ":localhost 332 " + client->GetName() + " " + channel + " :" + topic + "\r\n";
                send(client->GetRemote()->Get(), topicMessage.c_str(), topicMessage.size(), 0);
            }
        } else { 
            if (topic[0] == ':') {
                topic = topic.substr(1); 
            }
            currentChannel->SetTopic(topic);
            std::string topicChangeMessage = ":" + client->GetName() + " TOPIC " + channel + " :" + topic + "\r\n";
            server.BroadcastToChannel(currentChannel, topicChangeMessage, &select);
        }
}




/******************************************************/
/*                      USER                          */
/******************************************************/

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

    size_t secondSpacePos = currentLine.find_first_of(' ', spacePos + 1);
    if (secondSpacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t thirdSpacePos = currentLine.find_first_of(' ', secondSpacePos + 1);
    if (thirdSpacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t realnamePos = currentLine.find_first_of(':', thirdSpacePos + 1);
    if (realnamePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    Str username = Str(currentLine.begin() + spacePos + 1, currentLine.begin() + secondSpacePos);
    Str hostname = Str(currentLine.begin() + secondSpacePos + 1, currentLine.begin() + thirdSpacePos);
    Str servername = Str(currentLine.begin() + thirdSpacePos + 1, currentLine.begin() + realnamePos);
    Str realname = Str(currentLine.begin() + realnamePos + 1, currentLine.end());

    if (client->GetUser() != "") {
        std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    client->SetUser(username);
    client->SetRealname(realname);
    client->SetHostname(hostname);
    client->SetServername(servername);

    std::string welcomeMessage = ":localhost 001 " + client->GetNick() + " :Welcome to the ft_irc : " + client->GetNick() + "!" + client->GetUser() + "@" + "localhost\r\n";
    if (select.CanWrite(client->GetRemote()->Get()))
        send(client->GetRemote()->Get(), welcomeMessage.c_str(), welcomeMessage.size(), 0);

    std::string joinMessage = ":" + client->GetNick() + " has joined";
    std::cout << GREEN << joinMessage << RESET << std::endl;
}

#define REQ_PARAMS const char* line, Server& server, Client* client
