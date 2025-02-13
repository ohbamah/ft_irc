/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 02:51:22 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

	/* PUBLIC */

Poll::Poll(int* pfds, int pcount)
{
	if (pcount <= 0 || !pfds)
		throw (Poll::InvalidFdsCount());
	this->count = pcount;
	this->pollfds.reserve(pcount);

	for (int i = 0 ; i < this->count ; ++i)
	{
		this->pollfds[i].events = 0;
		this->pollfds[i].revents = 0;
		this->pollfds[i].fd = pfds[i];
	}
	this->last = this->pollfds[0];
}

Poll::~Poll()
{
}

int
Poll::Check(int ptimeout)
{
	return (poll(&this->pollfds[0], this->count, ptimeout));
}

void
Poll::AddFd(int fd)
{
	pollfd	tmp;

	tmp.fd = fd;
	tmp.events = 0;
	tmp.revents = 0;
	this->pollfds.push_back(tmp);
	++this->count;
}

bool
Poll::ReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLIN);
}

bool
Poll::NormalReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLRDNORM);
}

bool
Poll::PriReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLRDBAND);
}

bool
Poll::HighPriReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLPRI);
}

bool
Poll::WriteRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & (POLLOUT|POLLWRNORM));
}

bool
Poll::PriWriteRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLWRBAND);
}

bool
Poll::HupRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLHUP);
}

bool
Poll::IsErrorAppear(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.revents & POLLERR);
}

bool
Poll::IsInvalidFd(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
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
	int	i = 0;
	while(i < count && fd != this->pollfds[i].fd)
		++i;
	if (i != count)
		this->last = this->pollfds[i];
}
