/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 18:19:16 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

	/* PUBLIC */

Poll::Poll(Str* names, int* pfds, int pcount, int reqv)
{
	if (pcount <= 0 || !pfds)
		throw (Poll::InvalidFdsCount());

	for (int i = 0 ; i < pcount ; ++i)
	{
		this->pollfds[names[i]].events = reqv;
		this->pollfds[names[i]].revents = 0;
		this->pollfds[names[i]].fd = pfds[i];
	}
	this->last = this->pollfds.find(names[0])->second;
}

Poll::~Poll()
{
}

std::vector<pollfd>
Poll::Test(void)
{
	std::vector<pollfd>				ret;
	std::map<Str, pollfd>::iterator it;

	for (it = this->pollfds.begin(); it != this->pollfds.end(); ++it)
		ret.push_back(it->second);
	return (ret);
}

int
Poll::Events(int ptimeout)
{
	std::vector<pollfd>	feur = Test();
	return (poll(&feur[0], feur.size(), ptimeout));
}

void
Poll::AddFd(Str name, int fd, int reqv)
{
	pollfd	tmp;

	tmp.fd = fd;
	tmp.events = reqv;
	tmp.revents = 0;
	this->pollfds[name] = tmp;
	this->last = tmp;
}

bool
Poll::ReadRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLIN);
}

bool
Poll::NormalReadRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLRDNORM);
}

bool
Poll::PriReadRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLRDBAND);
}

bool
Poll::HighPriReadRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLPRI);
}

bool
Poll::WriteRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & (POLLOUT|POLLWRNORM));
}

bool
Poll::PriWriteRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLWRBAND);
}

bool
Poll::HupRequest(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLHUP);
}

bool
Poll::IsErrorAppear(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLERR);
}

bool
Poll::IsInvalidFd(Str name)
{
	this->last = this->pollfds.find(name)->second;
	return (this->last.revents & POLLNVAL);
}

	/* PRIVATE */

Poll::Poll(void)
{
}

Poll::Poll(const Poll&)
{
}

void
Poll::FindCorrectFd(int fd)
{
	//unsigned long	i = 0;
	//while(i < this->pollfds.size() && fd != this->pollfds[i].fd)
	//	++i;
	//if (i != this->pollfds.size())
	//	this->last = this->pollfds[i];
}
