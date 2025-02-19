/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:16:29 by ymanchon         ###   ########.fr       */
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

	for ( ; i < this->names.size() ; ++i)
	{
		if (!this->names[i].compare(name))
			return (&this->pfd[i]);
	}
	throw (Poll::NameIDDoesntExist());
}

int
Poll::Events(int ptimeout)
{
	return (poll(&this->pfd[0], this->pfd.size(), ptimeout));
}

void
Poll::AddFd(const Str& name, int fd, short reqv)
{
	pollfd	tmp;

	tmp.fd = fd;
	tmp.events = reqv;
	tmp.revents = 0;
	this->names.push_back(name);
	this->pfd.push_back(tmp);
}

bool
Poll::ReadRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLIN);
}

bool
Poll::NormalReadRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLRDNORM);
}

bool
Poll::PriReadRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLRDBAND);
}

bool
Poll::HighPriReadRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLPRI);
}

bool
Poll::WriteRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & (POLLOUT));
}

bool
Poll::PriWriteRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLWRBAND);
}

bool
Poll::HupRequest(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLHUP);
}

bool
Poll::IsErrorAppear(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLERR);
}

bool
Poll::IsInvalidFd(const Str& name)
{
	this->last = SearchPFD(name);
	return (this->last->revents & POLLNVAL);
}

void
Poll::ChangePollName(const Str& oldname, const Str& newname)
{
	std::vector<Str>::iterator	it = std::find(this->names.begin(), this->names.end(), oldname);
	it->replace(it->begin(), it->end(), newname);
}

	/* PRIVATE */

Poll::Poll(const Poll&)
{
}
