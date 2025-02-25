/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:37:41 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/25 15:56:11 by claprand         ###   ########.fr       */
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

bool is_valid_channel_name(const std::string &name){
    return !name.empty() && name[0] == '#';
}

    // ERR_TOOMANYCHANNELS 405
    // "<client> <channel> :You have joined too many channels"
    // Indicates that the JOIN command failed because the client has joined their maximum number of channels. 
    // The text used in the last param of this message may vary.


    // RPL_TOPIC (332)
    // "<client> <channel> :<topic>"
    // Sent to a client when joining the <channel> 
    // to inform them of the current topic of the channel.

    // RPL_TOPICWHOTIME (333)
    // "<client> <channel> <nick> <setat>"
    // Sent to a client to let them know who set the topic (<nick>) and when 
    // they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).

// void Req::__JOIN(REQ_PARAMS)
// {
//     if (currentLine.empty()) {
//         std::cerr << "Error: currentLine is empty!" << std::endl;
//         return;
//     }
//     std::istringstream iss(currentLine);
//     std::string command, channelName, key;
//     std::getline(iss, command, ' ');
//     if (!std::getline(iss, channelName, ' ') || channelName.empty()) {
//         std::cerr << "Error: Missing or invalid channel name!" << std::endl;
//         return;
//     }
//     if (std::getline(iss, key)) {
//         if (key.empty()) {
//             key.clear();
//         }
//     }
    
//     if (!client->GetAuthenticated()) {
//         std::string errorMessage = ":localhost 462 " + client->GetName() + " :You may not reregister\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }
 
//     if (channelName.empty()) {
//         std::string errorMessage = ":localhost 461 " + client->GetName() + " :Not enough parameters\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }

//     if (!is_valid_channel_name(channelName)) {
//         std::string errorMessage = ":localhost 403 " + client->GetName() + " " + channelName + " :No such channel\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }
    
//     Channel* channel = server.FindChannel(channelName);
//     if (!channel) {
//         std::cout << "Channel not found. Creating new channel: " << channelName << std::endl;
//         server.CreateChannel(channelName);  // Create the channel if it doesn't exist
//         channel = server.FindChannel(channelName); // Retrieve it again
        
//         if (!channel) {
//             std::cerr << "Error: Channel creation failed!" << std::endl;
//             return;
//         }
    
//         std::cout << "Channel created successfully." << std::endl;
//     } else {
//         std::cout << "Channel found: " << channelName << std::endl;
//     }
    
    
    
    
    

//     if (channel->isFull()) {
//         std::string errorMessage = ":localhost 471 " + client->GetName() + " " + channelName + " :Cannot join channel (+l)\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }

//     if (channel->isInviteOnly() && !channel->isInvited(client)) {
//         std::string errorMessage = ":localhost 473 " + client->GetName() + " " + channelName + " :Cannot join channel (+i)\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }
    
//     if (channel->isBanned(client)) {
//         std::string errorMessage = ":localhost 474 " + client->GetName() + " " + channelName + " :Cannot join channel (+b)\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }
    
//     if (channel->hasKey() && channel->GetPass() != key) {
//         std::string errorMessage = ":localhost 475 " + client->GetName() + " " + channelName + " :Cannot join channel (+k)\r\n";
//         if (select.CanWrite(client->GetRemote()->Get()))
//             send(client->GetRemote()->Get(), errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }
    
//     channel->AddUser(client);
//     server.sendChanInfos(client, channel);
// }

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
        std::cout << "Channel not found. Creating new channel: " << channelName << std::endl;
        server.CreateChannel(channelName);
        channel = server.FindChannel(channelName);
        
        if (!channel) {
            std::cerr << "Error: Channel creation failed!" << std::endl;
            return;
        }
    
        std::cout << "Channel created successfully." << std::endl;
    } else {
        std::cout << "Channel found: " << channelName << std::endl;
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

    // try {
    //     channel->AddUser(client);
    // } catch (const Channel::UserAlreadyInChannel& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }
    
    server.sendChanInfos(client, channel);
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
	if (nickname[0] == '$' || nickname[0] == ':') //|| nickname[0] == '#')
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
