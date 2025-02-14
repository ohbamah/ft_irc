/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/14 16:14:10 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

	/* PUBLIC */

Poll::Poll(void) : last(NULL)
{
}

Poll::Poll(Str* names, int* pfds, int pcount, short reqv)
{
	if (pcount <= 0 || !pfds)
		throw (Poll::InvalidFdsCount());

	pollfd	tmp;
	for (int i = 0 ; i < pcount ; ++i)
	{
		tmp.events = reqv;
		tmp.revents = 0;
		tmp.fd = pfds[i];
		this->names.push_back(names[i]);
		this->pfd.push_back(tmp);
	}
	this->last = SearchPFD(names[0]);
}

Poll::~Poll()
{
}

pollfd*
Poll::SearchPFD(const Str& name)
{
	std::vector<Str>::iterator	it;
	unsigned long				i = 0;

	for (it = this->names.begin(); it != this->names.end(); ++it)
	{
		if (!it->compare(name))
			return (&this->pfd.at(i));
		++i;
	}
	return (NULL);
}

int
Poll::Events(int ptimeout)
{
	return (poll(&this->pfd[0], this->pfd.size(), ptimeout));
}

void
Poll::AddFd(Str name, int fd, short reqv)
{
	pollfd	tmp;

	tmp.fd = fd;
	tmp.events = reqv;
	tmp.revents = 0;
	this->names.push_back(name);
	this->pfd.push_back(tmp);
	this->last = &tmp;
}

bool
Poll::ReadRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLIN);
}

bool
Poll::NormalReadRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLRDNORM);
}

bool
Poll::PriReadRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLRDBAND);
}

bool
Poll::HighPriReadRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLPRI);
}

bool
Poll::WriteRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & (POLLOUT));
}

bool
Poll::PriWriteRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLWRBAND);
}

bool
Poll::HupRequest(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLHUP);
}

bool
Poll::IsErrorAppear(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLERR);
}

bool
Poll::IsInvalidFd(Str name)
{
	this->last = SearchPFD(name);
	if (!this->last)
		throw (Poll::NameIDDoesntExist());
	return (this->last->revents & POLLNVAL);
}

	/* PRIVATE */

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
