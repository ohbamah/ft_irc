/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/12 21:13:30 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Poll.hpp"
#include "Socket.hpp"

int
Irc::Start(int port, const char* pass)
{
	Socket	sock(Socket::AddrFamily::IPv4, Socket::Type::TCP, Socket::Protocol::Auto);
	sock.Bind(port);

	SocketInstance	client1 = sock.Accept();
	return (0);
}
