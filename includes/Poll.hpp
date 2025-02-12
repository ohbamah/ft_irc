/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/12 19:28:48 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
#define POLL_HPP

# include <poll.h>
# include <exception>

class Poll
{
public:
	Poll(int pfds[], int pcount, int ptimeout);
	~Poll();

	void
	AddFd(int fd);

	bool
	ReadRequest(int fd);

	bool
	NormalReadRequest(int fd);

	bool
	PriReadRequest(int fd);

	bool
	HighPriReadRequest(int fd);

	bool
	WriteRequest(int fd);

	bool
	PriWriteRequest(int fd);

	bool
	HupRequest(int fd);

	bool
	IsErrorAppear(int fd);

	bool
	IsInvalidFd(int fd);

private:
	Poll(void);
	Poll(const Poll&);

	void
	FindCorrectFd(int fd);

private:
	pollfd*	pollfds;
	pollfd	last;
	int		count;

public:
	struct InvalidFdsCount { virtual const char* what(void) throw() { return ("Invalid count of fds."); }};
};

#endif
