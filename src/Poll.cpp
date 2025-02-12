/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/12 19:29:15 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

	/* PUBLIC */

Poll::Poll(int* pfds, int pcount, int ptimeout)
{
	if (pcount <= 0 || !pfds)
		throw (Poll::InvalidFdsCount());
	this->count = pcount;
	this->pollfds = new pollfd[this->count];

	for (int i = 0 ; i < this->count ; ++i)
	{
		this->pollfds[i].events = 0;
		this->pollfds[i].revents = 0;
		this->pollfds[i].fd = pfds[i];
	}
	this->last = this->pollfds[0];
	poll(this->pollfds, this->count, ptimeout);
}

Poll::~Poll()
{
	delete[](this->pollfds);
}

void
Poll::AddFd(int fd)
{
	pollfd*	tmp = new pollfd[this->count + 1];
	for (int i = 0 ; i < this->count ; ++i)
		tmp[i] = pollfds[i];
	tmp[count].events = 0;
	tmp[count].revents = 0;
	tmp[count++].fd = fd;
	delete[](this->pollfds);
	this->pollfds = tmp;
}

bool
Poll::ReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLIN);
}

bool
Poll::NormalReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLRDNORM);
}

bool
Poll::PriReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLRDBAND);
}

bool
Poll::HighPriReadRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLPRI);
}

bool
Poll::WriteRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & (POLLOUT|POLLWRNORM));
}

bool
Poll::PriWriteRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLWRBAND);
}

bool
Poll::HupRequest(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLHUP);
}

bool
Poll::IsErrorAppear(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLERR);
}

bool
Poll::IsInvalidFd(int fd)
{
	if (fd != this->last.fd)
		this->FindCorrectFd(fd);
	return (this->last.events & POLLNVAL);
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
