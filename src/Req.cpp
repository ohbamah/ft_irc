/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:37:41 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/07 16:42:53 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"

const unsigned int
Req::nReqfun = REQ_COUNT;

Str Req::currentLine; 

bool
Req::disconnected = false;

const Str
Req::reqname[REQ_COUNT] = {"CAP", "INVITE", "JOIN", "KICK", "MODE", "NICK", "PASS", "TOPIC", "USER", "PRIVMSG", "QUIT", "PART"};

void
(*Req::reqfun[REQ_COUNT])(REQ_PARAMS) = {Req::__CAP, Req::__INVITE, Req::__JOIN, Req::__KICK, Req::__MODE, Req::__NICK, Req::__PASS, Req::__TOPIC, Req::__USER, Req::__PRIVMSG, Req::__QUIT, Req::__PART};

//static Str get_line(char** txt)
//{
//    Str ret;
//    
//    if (**txt == '\n')
//        (*txt)++;
//    
//    while (**txt && **txt != '\n')
//        ret.push_back(*((*txt)++));
//    
//    if (**txt == '\n')
//        (*txt)++;
//        
//    return ret;
//}

void 
Req::Check(Select& select, Server& server, std::vector<Channel>& channels, Client* client)
{
	char*	req = client->GetMessage();
	bool	enter = false;

	//std::cout << "req: \"\e[31m" << req << "\e[0m\"" << std::endl;
	while (!((currentLine = Utils::Getline(req)).empty()))
	{
		Req::disconnected = false;
		unsigned int i = 0;

		std::cout << "\"\e[33m" << currentLine << "\e[0m\"" << std::endl;
		enter = true;
		if (!currentLine.empty() && currentLine[currentLine.size() - 1] == '\n')
			currentLine.erase(currentLine.size() - 1);
		if (!currentLine.empty() && currentLine[currentLine.size() - 1] == '\r')
			currentLine.erase(currentLine.size() - 1);
			
		size_t spacePos = currentLine.find(' ');
		Str cmd;
		Str input;
			
		if (spacePos == std::string::npos)
		{
			cmd = currentLine;
			input = "";
		}
		else
		{
			cmd = currentLine.substr(0, spacePos);
			input = currentLine.substr(spacePos + 1);
		}

		bool found = false;
		//std::cout << "cmd : " << cmd << std::endl;
		if (cmd == "PONG" || cmd == "WHO")
		{
			//std::cout << "PONG\n";
			found = true;
		}
		else if (cmd == "PING")
		{
			if (select.CanWrite(client->GetRemote()->Get()))
				send(client->GetRemote()->Get(), "PONG", 4, 0);
			found = true;
		}
		else
		{
			for (int i = 0; i < REQ_COUNT; ++i)
			{
				if (cmd[0] && !reqname[i].compare(cmd))
				{
					reqfun[i](REQ_DATA);
					found = true;
					break;
				}
			}
		}
		if (Req::disconnected == true)
			break ;
		if (!found)
			std::cout << RED << "Commande inconnue : " << cmd << RESET << std::endl;
		//std::cout << "before resize: (current line = " << currentLine << " [" << currentLine.size() << "])\n" << req << std::endl;
		//std::cout << "i " << i << "\n";
		i = currentLine.size();
		for ( ; req[i] && (req[i] == '\n' || req[i] == '\r') ; ++i)
			;
		client->ResizeBuffer(i);
		//std::cout << "after resize: " << req << std::endl;
	}
	if (Req::disconnected == false && enter == true && (req[0] == '\n' || req[0] == '\r' || req[0] == '\0'))
	{
		//std::cout << "flushed\n";
		client->FlushBuffer();
		Utils::ResetGetline();
	}
}


void 
sendWelcomeMessages(Client* client, Select& select) {
    if (!client)
        return;

    std::string welcomeMessage = ":localhost 001 " + client->GetNick() + " :Welcome to the ft_irc Network, " + client->GetNick() + "!" + client->GetUser() + "@localhost\r\n";
    std::string yourHostMessage = ":localhost 002 " + client->GetNick() + " :Your host is localhost, running version 1.0\r\n";
    std::string createdMessage = ":localhost 003 " + client->GetNick() + " :This server was created some time ago\r\n";
    std::string usermode = ":localhost 004 " + client->GetNick() + " : IRC server 1.0 usermode chanmode\r\n";
    std::string motdStart = ":localhost 375 " + client->GetNick() + " :- localhost Message of the Day -\r\n";
    std::string motd = ":localhost 376 " + client->GetNick() + " :End of /MOTD command.\r\n";
        

    int clientSocket = client->GetRemote()->Get();
    if (select.CanWrite(clientSocket)) {
        send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
        usleep(1000);
        send(clientSocket, yourHostMessage.c_str(), yourHostMessage.size(), 0);
        usleep(1000);
        send(clientSocket, createdMessage.c_str(), createdMessage.size(), 0);
        usleep(1000);
        send(clientSocket, usermode.c_str(), usermode.size(), 0);
        usleep(1000);
        send(clientSocket, motdStart.c_str(), motdStart.size(), 0);
        usleep(1000);
        send(clientSocket, motd.c_str(), motd.size(), 0);
        
    }
}

/******************************************************/
/*                      CAP                           */
/******************************************************/

void 
Req::__CAP(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    
    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        return;
    }
    
    std::string subCommand = currentLine.substr(spacePos + 1);
    
    if (subCommand.find("LS") == 0) {
        std::string response = ":localhost CAP * LS :multi-prefix\r\n";
        if (select.CanWrite(client->GetRemote()->Get())) {
            send(client->GetRemote()->Get(), response.c_str(), response.size(), 0);
        }
    }
    else if (subCommand.find("REQ") == 0) {
        size_t capStartPos = subCommand.find(":");
        if (capStartPos != std::string::npos) {
            std::string requestedCap = subCommand.substr(capStartPos + 1);
            if (requestedCap == "multi-prefix") {
                std::string response = ":localhost CAP * ACK :multi-prefix\r\n";
                if (select.CanWrite(client->GetRemote()->Get())) {
                    send(client->GetRemote()->Get(), response.c_str(), response.size(), 0);
                }
            }
        }
    }
    else if (subCommand == "END") {
    }
}

/******************************************************/
/*                      INVITE                        */
/******************************************************/

bool 
is_valid_channel_name(const std::string &name){
    return !name.empty() && name[0] == '#';
}

void 
Req::__INVITE(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    if (!client)
        return;
    
    std::stringstream ss(currentLine);
    std::string command, targetNick, channelName;
    ss >> command >> targetNick >> channelName;

    if (client->GetAuthenticated() == false || client->GetNick().empty() || client->GetUser().empty()){
        std::string errorMessage = ":localhost 421 " + client->GetNick()  + " INVITE :Need to be logged in\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return ;
    }

    if (targetNick.empty() || channelName.empty()) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " INVITE :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (targetNick == client->GetNick()) {
        std::string errorMessage = ":localhost 441 " + client->GetNick() + " " + targetNick + " " + channelName + " :You cannot invite yourself\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
    }

    Channel* channel = server.FindChannel(channelName);
    if (!is_valid_channel_name(channelName) || !channel) {
        std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!channel->isOperator(client)) {
        std::string errorMessage = ":localhost 482 " + client->GetNick() + " " + channelName + " :You're not channel operator\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    Client* targetClient = server.getClientByNick(targetNick);
    if (!targetClient) {
        std::string errorMessage = ":localhost 401 " + client->GetNick() + " " + targetNick + " :No such nick/channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (channel->IsMember(targetClient)) {
        std::string errorMessage = ":localhost 443 " + client->GetNick() + " " + targetNick + " " + channelName + " :is already on channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    channel->InviteUser(targetClient);
    std::string targetMessage = ":localhost 341 " + client->GetNick() + " " + targetNick + " " + channelName + "\r\n";
    if (select.CanWrite(targetClient->GetRemote()->Get()))
        send(targetClient->GetRemote()->Get(), targetMessage.c_str(), targetMessage.size(), 0);
}

/******************************************************/
/*                      JOIN                          */
/******************************************************/

bool 
isValidChannelName(std::string name) 
{
    if (name.empty())
        return false;
    
    if (name[0] != '#')
        return false;
    
    size_t len = name.size();
    if (len < 2 || len > 50)
        return false;
    
    for (size_t i = 1; i < len; i++) {
        if (name[i] == ' ' || name[i] == ',' || name[i] == ':')
            return false;
        if (!std::isalnum(name[i]) && name[i] != '_' && name[i] != '-' && name[i] != '.')
            return false;
    }
    
    return true;
}

void 
Req::__JOIN(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    if (currentLine.empty()) {
        std::cerr << "Error: currentLine is empty!" << std::endl;
        return;
    }

    if (client->GetAuthenticated() == false || client->GetNick().empty() || client->GetUser().empty()) {
      std::string errorMessage = ":localhost 421 " + client->GetNick() + " JOIN :Need to be logged in\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::istringstream iss(currentLine);
    std::string command, channelsStr, keysStr;
    std::getline(iss, command, ' ');
    std::getline(iss, channelsStr, ' ');
    std::getline(iss, keysStr);

    if (channelsStr.empty()) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " JOIN :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::vector<std::string> channelNames;
    std::vector<std::string> keys;
    std::stringstream channelStream(channelsStr);
    std::stringstream keyStream(keysStr);
    std::string temp;

    while (std::getline(channelStream, temp, ',')) {
        channelNames.push_back(temp);
    }

    while (std::getline(keyStream, temp, ',')) {
        keys.push_back(temp);
    }

    for (size_t i = 0; i < channelNames.size(); i++) {
        std::string channelName = channelNames[i];
        std::string key = (i < keys.size()) ? keys[i] : "";

        if (!isValidChannelName(channelName)) {
            std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + channelName + " :No such channel\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            continue;
        }

        Channel* channel = server.FindChannel(channelName);
        if (!channel) {
            server.CreateChannel(channelName);
            channel = server.FindChannel(channelName);
            if (!key.empty()) {
                channel->SetPass(key);
            }
            try {
                channel->AddUser(client);
                client->AddChannel(channel);
                std::cout << GREEN << client->GetNick() << " has created and joined channel " << channelName << RESET << std::endl;
                if (channel->GetMemberCount() == 1) 
                    channel->ElevateUser(client);
            } catch (const Channel::UserAlreadyInChannel& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        if (channel->isFull()) {
            std::string errorMessage = ":localhost 471 " + client->GetNick() + " " + channelName + " :Cannot join channel (+l)\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            continue;
        }

        if (channel->isInviteOnly() && !channel->isInvited(client)) {
            std::string errorMessage = ":localhost 473 " + client->GetNick() + " " + channelName + " :Cannot join channel (+i)\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            continue;
        }

        if (channel->hasKey() && channel->GetPass() != key) {
            std::string errorMessage = ":localhost 475 " + client->GetNick() + " " + channelName + " :Cannot join channel (+k)\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            continue;
        }

        try {
            channel->AddUser(client);
            client->AddChannel(channel);
            std::cout << GREEN << client->GetNick() << " has joined channel " << channelName << RESET << std::endl;
        } catch (const Channel::UserAlreadyInChannel& e) {}

        std::string joinMessage = ":" + client->GetNick() + "!" + client->GetUser() + "@localhost JOIN " + channelName + "\r\n";
        server.BroadcastToChannel(channel, joinMessage, &select);

        if (select.CanWrite(client->GetRemote()->Get()))
            server.sendChanInfos(client, channel);
    }
}

/******************************************************/
/*                      KICK                          */
/******************************************************/

void 
Req::__KICK(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    
    std::istringstream iss(currentLine);
    std::string command, channelName, targetsStr;
    iss >> command >> channelName >> targetsStr;
    
    std::string reason;
    size_t reasonPos = currentLine.find(targetsStr) + targetsStr.length();
    if (reasonPos < currentLine.length()) {
        reason = currentLine.substr(reasonPos);
        reason = reason.substr(reason.find_first_not_of(" \t"));
        if (!reason.empty() && reason[0] == ':') {
            reason = reason.substr(1);
        }
    }
    
    if (reason.empty()) {
        reason = "No reason given";
    }
    
    if (!client->GetAuthenticated() || client->GetNick().empty() || client->GetUser().empty()) {
        std::string errorMessage = ":localhost 421 " + client->GetNick() + " KICK :Need to be logged in\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    Channel* channel = server.FindChannel(channelName);
    if (!channel) {
        std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!channel->IsMember(client)) {
        std::string errorMessage = ":localhost 442 " + client->GetNick() + " " + channelName + " :You're not on that channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!channel->isOperator(client)) {
        std::string errorMessage = ":localhost 482 " + client->GetNick() + " " + channelName + " :You're not channel operator\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    std::vector<std::string> targetNicks;
    std::stringstream targetStream(targetsStr);
    std::string temp;
    while (std::getline(targetStream, temp, ',')) {
        targetNicks.push_back(temp);
    }
    
    for (size_t i = 0; i < targetNicks.size(); i++) {
        std::string targetNick = targetNicks[i];
        Client* targetClient = server.FindClient(targetNick);
        
        if (!targetClient || !channel->IsMember(targetClient)) {
            std::string errorMessage = ":localhost 441 " + client->GetNick() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            continue;
        }
        
        std::string kickMsg = ":" + client->GetNick() + "!" + client->GetUser() + "@localhost KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
        
        server.BroadcastToChannel(channel, kickMsg, &select);
        
        channel->RevokeUser(targetClient);
        
        std::cout << GREEN << "User " << targetNick << " was kicked from " << channelName << " by " << client->GetNick() << " (" << reason << ")" << RESET << std::endl;
    }
}

/******************************************************/
/*                      MODE                          */
/******************************************************/

void 
Req::__MODE(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    std::istringstream iss(currentLine);
    std::string command, channelName, modes, mdp, userNick;
    std::getline(iss, command, ' ');

    if (!std::getline(iss, channelName, ' ') || channelName.empty()) {
        std::cerr << "Error: Invalid or empty channel name!" << std::endl;
        return;
    }

    if (channelName[0] != '#')
        return;
        
    Channel* channel = server.FindChannel(channelName);
    if (!channel) {
        std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!std::getline(iss, modes, ' ') || modes.empty()) {
        // std::cerr << "Error: Modes string is empty!" << std::endl;
        return;
    }

    if (!channel->isOperator(client)) {
        std::string errorMessage = ":localhost 481 " + client->GetNick() + " :Permission Denied - You're not a channel operator\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    bool addMode = true;
    for (size_t i = 0; i < modes.size(); ++i) {
        char mode = modes[i];

        if (mode == '+') {
            addMode = true;
            continue;
        }
        if (mode == '-') {
            addMode = false;
            continue;
        }
        switch (mode) {
            case 'b':
                return;
            case 'i':
            if (addMode){
                channel->SetInviteOnly(addMode);
                std::cout << GREEN << "This channel is invite-only. You need an invitation to join." << RESET << std::endl;
                break;
            } else {
                channel->SetInviteOnly(addMode);
            }
                break; //? Break ?
            case 'k':  
                if (addMode) {
                    if (std::getline(iss, mdp)) {
                        channel->SetPass(mdp);
                        std::cout << GREEN << "The channel " << channelName << " is now requiring a key to join." << RESET << std::endl;
                    } else {
                        std::cerr << RED << "Error: No password provided for channel " << channelName << RESET << std::endl;
                        return;
                    }
                } else {
                    channel->SetPass("");
                }
                break;

            case 'l':
                if (addMode) {
                    int maxClients;
                    iss >> maxClients;
                    channel->setMaxClients(maxClients);
                    std::cout << GREEN << "The maximum number of clients is now set to " << maxClients << "." << RESET << std::endl;
                } else {
                    channel->setMaxClients(50);
                }
                break;

            case 't':
                if (addMode){
                    channel->setTopicRestricted(addMode);
                    std::cout << GREEN << "The topic of this channel is now restricted to operators only." << RESET << std::endl;
                    break;
                }
                else {
                    channel->setTopicRestricted(addMode);
                    std::cout << GREEN << "The topic of this channel is NOT restricted to operators only." << RESET << std::endl;
                    break;
                }
            
            case 'o':
                if (std::getline(iss, userNick)) {
                    Client* targetClient = server.FindClient(userNick);
                    if (targetClient && channel->IsMember(targetClient)) {
                        channel->setOperator(targetClient);
                        std::cout << GREEN << "The user " << targetClient->GetNick() << " has been promoted to operator." << RESET << std::endl;
                    } else {
                        std::cerr << RED << "Error: User " << userNick << " not found in channel " << channelName << RESET << std::endl;
                    }
                } else {
                    std::cerr << RED << "Error: No nickname provided for +o/-o" << RESET << std::endl;
                }
                break;

            default:
                std::cerr << RED << "Error: Unknown mode." << RESET << std::endl;
                break;
        }
    }
    
    std::string modeResponse = ":" + client->GetNick() + " MODE " + channelName + " " + modes + "\r\n";
    if (select.CanWrite(client->GetRemote()->Get())) {
        server.BroadcastToChannel(channel, modeResponse.c_str(), &select);
    }
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
    UNUSED_REQ_PARAMS;
    size_t spacePos = currentLine.find_first_of(' ');
    
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        std::string errorMessage = ":localhost 431 " + client->GetNick() + " :No nickname given\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    std::string newNick = currentLine.substr(spacePos + 1);
    size_t endPos = newNick.find_first_of("\r\n");
    if (endPos != std::string::npos) {
        newNick = newNick.substr(0, endPos);
    }
    newNick.erase(newNick.find_last_not_of(" \t\r\n") + 1);
    
    if (containsInvalidCharacters(newNick)) {
        std::string errorMessage = ":localhost 432 " + newNick + " :Erroneous nickname\r\n";
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
    if (oldNick.empty()) {
        oldNick = client->GetName();
        if (oldNick.empty()) {
            oldNick = "*";
        }
    }

    std::string hostname = client->GetHostname();
    if (hostname.empty()) {
        hostname = "localhost";
        client->SetHostname(hostname);
    }

    client->SetNick(newNick);
    client->MarkNickAsSet();
    
    std::cout << GREEN << oldNick << " updated: " << oldNick << " is now known as " << newNick << "!" << RESET << std::endl;

    std::string userPrefix = client->GetUser().empty() ? "unknown" : client->GetUser();
    std::string nickMsg = ":" + oldNick + "!" + userPrefix + "@" + hostname + " NICK :" + newNick + "\r\n";

    std::vector<Channel *> clientChannels = server.GetChannelsOfClient(client);
    for (size_t i = 0; i < clientChannels.size(); i++) {
        Channel *channel = clientChannels[i];
        channel->broadcastMessage(client, nickMsg, select);
        channel->UpdateNick(client, newNick);
    }

    if (select.CanWrite(client->GetRemote()->Get())) {
        send(client->GetRemote()->Get(), nickMsg.c_str(), nickMsg.size(), 0);
    }
}

/******************************************************/
/*                      PASS                          */
/******************************************************/

void 
Req::__PASS(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    if (client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 462 " + client->GetNick() + " :You may not reregister\r\n";
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos) {
        spacePos = currentLine.length();
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " :Not enough parameters\r\n";
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string password = currentLine.substr(spacePos + 1);
    if (password != server.GetPassword()) {
        std::string errorMessage = ":localhost 464 " + client->GetNick() + " :Password incorrect\r\n";
		if (select.CanWrite(client->GetRemote()->Get()))
			send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    

    client->SetAuthenticated(true);
    if (client->GetAuthenticated() == true) {
        std::cout << "\e[32mNew client connected! Socket FD: " << client->GetRemote()->Get() << "\e[0m" << std::endl;
    }
}


/******************************************************/
/*                      TOPIC                         */
/******************************************************/
void 
Req::__TOPIC(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    
    if (client->GetAuthenticated() == false || client->GetNick().empty() || client->GetUser().empty()) {
        std::string errorMessage = ":localhost 421 " + client->GetNick()  + " TOPIC :Need to be logged in\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t spacePos = currentLine.find(' ');
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " TOPIC :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string remaining = currentLine.substr(spacePos + 1);
    size_t nextSpacePos = remaining.find(' ');

    std::string channel = (nextSpacePos == std::string::npos) ? remaining : remaining.substr(0, nextSpacePos);
    std::string topic = (nextSpacePos == std::string::npos) ? "" : remaining.substr(nextSpacePos + 1);

    Channel* channelName = server.GetChannel(channel);
    if (!channelName) {
        std::string errorMessage = ":localhost 403 " + channel + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (!channelName->IsMember(client)) {
        std::string errorMessage = ":localhost 442 " + channel + " :You're not on that channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (channelName->isTopicRestricted()) {
        if (!channelName->isOperator(client)) {
            std::string errorMessage = ":localhost 482 " + channel + " :You're not channel operator\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            return;
        }
    }

    if (topic.empty()) {
        if (channelName->GetTopic().empty()) {
            std::string noTopicMessage = ":localhost 331 " + client->GetNick() + " " + channel + " :No topic is set\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), noTopicMessage.c_str(), noTopicMessage.size(), 0);
        } else {
            std::string topicMessage = ":localhost 332 " + client->GetNick() + " " + channel + " :" + channelName->GetTopic() + "\r\n";
            if (select.CanWrite(client->GetRemote()->Get()))
                send(client->GetRemote()->Get(), topicMessage.c_str(), topicMessage.size(), 0);
        }
        return;
    }

    if (topic.find(' ') != std::string::npos && topic[0] != ':') {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " TOPIC :Topic must start with ':' if it contains multiple words\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (!topic.empty() && topic[0] == ':') {
        topic = topic.substr(1);
    }

    channelName->SetTopic(topic);
    std::string topicChangeMessage = ":" + client->GetNick() + " TOPIC " + channel + " :" + topic + "\r\n";
    server.BroadcastToChannel(channelName, topicChangeMessage, &select);
}

/******************************************************/
/*                      USER                          */
/******************************************************/

void 
Req::__USER(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    if (client->GetAuthenticated() == false) {
        std::string errorMessage = "You may start by command PASS to log in with the password\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return ;
    }

    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t secondSpacePos = currentLine.find_first_of(' ', spacePos + 1);
    if (secondSpacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t thirdSpacePos = currentLine.find_first_of(' ', secondSpacePos + 1);
    if (thirdSpacePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    size_t realnamePos = currentLine.find_first_of(':', thirdSpacePos + 1);
    if (realnamePos == std::string::npos) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    Str username = Str(currentLine.begin() + spacePos + 1, currentLine.begin() + secondSpacePos);
    Str hostname = Str(currentLine.begin() + secondSpacePos + 1, currentLine.begin() + thirdSpacePos);
    Str servername = Str(currentLine.begin() + thirdSpacePos + 1, currentLine.begin() + realnamePos);
    Str realname = Str(currentLine.begin() + realnamePos + 1, currentLine.end());

    if (realname.empty()) {
        std::string errorMessage = ":localhost 411 " + client->GetNick() + " :No realname\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return ;
    }

    if (client->GetUser() != "") {
        std::string errorMessage = ":localhost 462 " + client->GetNick() + " :You may not reregister\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    client->SetUser(username);
    client->SetRealname(realname);
    client->SetHostname(hostname);
    client->SetServername(servername);

    if (client->GetAuthenticated() == true && client->HasSetNick()) {
        if (select.CanWrite(client->GetRemote()->Get())){
            sendWelcomeMessages(client, select);
        }
    std::cout << GREEN << "Authentication successful, " << client->GetNick() << " is now connected." << RESET << std::endl;
    }
}

/******************************************************/
/*                      PRIVMSG                       */
/******************************************************/
void 
Req::__PRIVMSG(REQ_PARAMS) 
{
    UNUSED_REQ_PARAMS;
    if (!client)
        return;

    if (client->GetAuthenticated() == false || client->GetNick().empty() || client->GetUser().empty()){
        std::string errorMessage = ":localhost 421 " + client->GetNick() + " KICK :Need to be logged in\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::stringstream ss(currentLine);
    std::string command, target, message;

    ss >> command;
    std::getline(ss, target, ':');
    std::getline(ss, message);

    if (target.empty() || message.empty()) {
        std::string errorMessage = ":localhost 411 " + client->GetNick() + " :No recipient or text to send\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string trimmedTarget = target;
    if (!trimmedTarget.empty() && trimmedTarget[0] == ' ') 
        trimmedTarget = trimmedTarget.substr(1);

    std::vector<std::string> targets;
    std::string singleTarget;
    std::stringstream targetStream(trimmedTarget);

    while (targetStream >> singleTarget) {
        targets.push_back(singleTarget);
    }

    for (std::size_t i = 0; i < targets.size(); ++i) {
        const std::string& target = targets[i];
        if (target.empty()) continue; 
        
        if (target[0] == '#') { 
            Channel* channel = server.FindChannel(target);
            if (!channel) {
                std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + target + " :No such channel\r\n";
                if (select.CanWrite(client->GetRemote()->Get()))
                    send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            } else {
                if (channel->HasClient(client)) {
                    std::string errorMessage = ":" + client->GetNick() + " PRIVMSG " + target + " " + message + "\r\n";
                    channel->broadcastMessage(client, errorMessage, select);
                } else {
                    std::string errorMessage = ":localhost 442 " + client->GetNick() + " " + target + " :You're not on that channel\r\n";
                    if (select.CanWrite(client->GetRemote()->Get()))
                        send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
                }
            }
        } else {
            Client* recipient = server.getClientByNick(target);
            if (!recipient) {
                std::string errorMessage = ":localhost 401 " + client->GetNick() + " " + target + " :No such nick\r\n";
                if (select.CanWrite(client->GetRemote()->Get()))
                    send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
            } else {
                std::string msgToSend = ":" + client->GetNick() + "!" + client->GetUser() + "@localhost " + client->GetNick() + " PRIVMSG " + target + " " + message + "\r\n";
                if (select.CanWrite(client->GetRemote()->Get()))
                    send(recipient->GetRemote()->Get(), msgToSend.c_str(), msgToSend.size(), 0);
            }
        }
    }
}

/******************************************************/
/*                      QUIT                          */
/******************************************************/
void 
Req::__QUIT(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    if (!client) return;
    
    std::vector<Channel*> userChannels = server.GetChannelsOfClient(client);
    for (std::vector<Channel*>::iterator it = userChannels.begin(); it != userChannels.end(); ++it) {
        if (*it) {
            (*it)->RevokeUser(client);
        }
    }

    std::string quitMessage = ":" + client->GetNick() + " QUIT :Client exited\r\n";
    for (std::vector<Channel*>::iterator it = userChannels.begin(); it != userChannels.end(); ++it) {
        if (*it)
            server.BroadcastToChannel(*it, quitMessage, &select);
    }

    int clientFd = client->GetRemote()->Get();
    if (clientFd) {
        select.RemoveExcpReq(clientFd);
        select.RemoveReadReq(clientFd);
        select.RemoveWriteReq(clientFd);
    }

    server.Disconnect(client);
	Req::disconnected = true;
    std::cout << RED << "Déconnexion!" << RESET << std::endl;
}

/******************************************************/
/*                      PART                          */
/******************************************************/
void Req::__PART(REQ_PARAMS)
{
    UNUSED_REQ_PARAMS;
    
    if (!client->GetAuthenticated()) {
        std::string errorMessage = ":localhost 451 " + client->GetNick() + " :You have not registered\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    size_t spacePos = currentLine.find_first_of(' ');
    if (spacePos == std::string::npos || spacePos + 1 >= currentLine.length()) {
        std::string errorMessage = ":localhost 461 " + client->GetNick() + " PART :Not enough parameters\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    std::string params = currentLine.substr(spacePos + 1);
    std::string channelName;
    std::string partMessage = "Leaving";
    
    size_t msgPos = params.find_first_of(' ');
    if (msgPos != std::string::npos) {
        channelName = params.substr(0, msgPos);
        size_t colonPos = params.find(':', msgPos);
        if (colonPos != std::string::npos) {
            partMessage = params.substr(colonPos + 1);
        }
    } else {
        channelName = params;
    }
    
    Channel* channel = server.FindChannel(channelName);
    if (!channel) {
        std::string errorMessage = ":localhost 403 " + client->GetNick() + " " + channelName + " :No such channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    if (!channel->HasClient(client)) {
        std::string errorMessage = ":localhost 442 " + client->GetNick() + " " + channelName + " :You're not on that channel\r\n";
        if (select.CanWrite(client->GetRemote()->Get()))
            send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    
    std::string partMsg = ":" + client->GetNick() + "!" + client->GetUser() + "@localhost PART " + channelName + " :" + partMessage + "\r\n";
    server.BroadcastToChannel(channel, partMsg, &select);
    server.RemoveClientChannel(client, channelName);
    if (channel->IsEmpty()) {
        server.DeleteChannel(channelName);
    }
    std::cout << GREEN << client->GetNick() << " left channel " << channelName << RESET << std::endl;
}
