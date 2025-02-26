/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 15:35:20 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/26 15:46:58 by claprand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include "Client.hpp"
# include <vector>
# include <algorithm>
# include <map>
# include <string>
# include <iomanip>
# include <iostream>
# include "Select.hpp"
# include "Channel.hpp"

class Server : private Socket
{
	typedef std::string	Str;

public:
	Server(int port, int pFControlFlags);
	~Server();

	const int&
	Get(void);
	std::vector<Client*>&
	RefClients(void);

	void
	Disconnect(const Client* ptr);
	void
	Accept(const Str& clientName);
	void
	Listen(void);

	template <typename T>
	int
	SendTo(const Str& clientName, const T& data, unsigned int nbytes = sizeof(data));
	template <typename T>
	int
	SendTo(const Str& clientName, const T* data, unsigned int nbytes);

	template <typename T>
	int
	RecvFrom(const Str& clientName, T& data, unsigned int nbytes = sizeof(data));
	template <typename T>
	int
	RecvFrom(const Str& clientName, T* data, unsigned int nbytes);

	template <typename T>
	int
	SendTo(int clientIndex, const T& data, unsigned int nbytes = sizeof(data));
	template <typename T>
	int
	SendTo(int clientIndex, const T* data, unsigned int nbytes);

	template <typename T>
	int
	RecvFrom(int clientIndex, T& data, unsigned int nbytes = sizeof(data));
	template <typename T>
	int
	RecvFrom(int clientIndex, T* data, unsigned int nbytes);

	Client*
	FindClientByName(const Str& name);

	void
	SetPassword(std::string pass);

	std::string
	GetPassword() const;

	Channel* 
	GetChannel(const std::string& channelName) const;

	void 
	SetChannel(const std::string& channelName, Channel* channel);

	void 
	Broadcast(const std::string& message, Client* exclude, Select* select);

	void 
	BroadcastToChannel(Channel* channel, const std::string& message, Select* select);

	bool 
	IsNicknameTaken(const std::string& nickname) const;

	// Channel *
	// FindChannel(std::vector<Channel>& channel, std::string const & name);

	Channel *
	FindChannel(std::string const &name);

	void 
	CreateChannel(std::string const &channelName);

	void 
	sendChanInfos(Client *client, Channel *channel);

	Client* 
	getClientByNick(const std::string& nick);
	
	Client* 
	FindClient(const std::string& name) const;
	
private:
	std::vector<Client*>	clients;
	Str						password;
	Str						channel;
	std::map<std::string, Channel *> channels;

public:
	struct CantFindClient : std::exception { inline virtual const char* what(void) const throw() {return ("Client does not exist\n"); } };
};

# include "Server.inl"

#endif
