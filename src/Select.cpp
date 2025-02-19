/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Select.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:18:49 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 15:59:03 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Select.hpp"

Select::Select(void) : max_fd(0)
{
}

Select::~Select()
{
}


int
Select::CanRead(int fd)
{
	if (std::find(this->rdfd.begin(), this->rdfd.end(), fd) == this->rdfd.end())
		throw (Select::FDNotInSelect());
	return (FD_ISSET(fd, &this->rd_set));
}

int
Select::CanWrite(int fd)
{
	if (std::find(this->wrfd.begin(), this->wrfd.end(), fd) == this->wrfd.end())
		throw (Select::FDNotInSelect());
	return (FD_ISSET(fd, &this->wr_set));
}

int
Select::Exception(int fd)
{
	if (std::find(this->exfd.begin(), this->exfd.end(), fd) == this->exfd.end())
		throw (Select::FDNotInSelect());
	return (FD_ISSET(fd, &this->ex_set));
}

void
Select::SnapEvents(int timeout)
{
	this->__ResetSets();
	timeval	tv;
	fd_set*	rdbuf = static_cast<fd_set*>(0);
	fd_set* wrbuf = static_cast<fd_set*>(0);
	fd_set* exbuf = static_cast<fd_set*>(0);
	if (this->rdfd.size() > 0)
		rdbuf = &this->rd_set;
	if (this->wrfd.size() > 0)
		wrbuf = &this->wr_set;
	if (this->exfd.size() > 0)
		exbuf = &this->ex_set;
	tv.tv_sec = 0;
	tv.tv_usec = timeout * 1000;
	if (select(this->max_fd + 1, &this->rd_set, &this->wr_set, &this->ex_set, &tv) == -1)
		throw (Select::FatalError());
}

void
Select::AddWriteReq(int fd)
{
	this->wrfd.push_back(fd);
	FD_SET(fd, &this->wr_set);
	if (this->max_fd < fd)
		this->max_fd = fd;
}

void
Select::AddReadReq(int fd)
{
	this->rdfd.push_back(fd);
	FD_SET(fd, &this->rd_set);
	if (this->max_fd < fd)
		this->max_fd = fd;
}

void
Select::AddExcpReq(int fd)
{
	this->exfd.push_back(fd);
	FD_SET(fd, &this->ex_set);
	if (this->max_fd < fd)
		this->max_fd = fd;
}

void
Select::RemoveWriteReq(int fd)
{
	std::vector<int>::iterator	it = std::find(this->wrfd.begin(), this->wrfd.end(), fd);
	this->wrfd.erase(it);
	this->__ResetSets();
}

void
Select::RemoveReadReq(int fd)
{
	std::vector<int>::iterator	it = std::find(this->rdfd.begin(), this->rdfd.end(), fd);
	this->rdfd.erase(it);
	this->__ResetSets();
}

void
Select::RemoveExcpReq(int fd)
{
	std::vector<int>::iterator	it = std::find(this->exfd.begin(), this->exfd.end(), fd);
	this->exfd.erase(it);
	this->__ResetSets();
}

void
Select::__ResetSets(void)
{
	FD_ZERO(&this->rd_set);
	FD_ZERO(&this->wr_set);
	FD_ZERO(&this->ex_set);
	for (unsigned int i = 0 ; i < this->rdfd.size() ; ++i)
		FD_SET(this->rdfd[i], &this->rd_set);
	for (unsigned int i = 0 ; i < this->wrfd.size() ; ++i)
		FD_SET(this->wrfd[i], &this->wr_set);
	for (unsigned int i = 0 ; i < this->exfd.size() ; ++i)
		FD_SET(this->exfd[i], &this->ex_set);
}
