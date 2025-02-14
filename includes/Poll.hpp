/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/14 16:13:03 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
#define POLL_HPP

# include <poll.h>
# include <exception>
# include <vector>
# include <map>
# include <string>

typedef std::string	Str;

class Poll
{
public:
	Poll(void);
	Poll(Str names[], int pfds[], int pcount, short reqv = Poll::AllReq);
	~Poll();

	int
	Events(int timeout);

	void
	AddFd(Str name, int fd, short reqv = Poll::AllReq);

	bool
	ReadRequest(Str name);

	bool
	NormalReadRequest(Str name);

	bool
	PriReadRequest(Str name);

	bool
	HighPriReadRequest(Str name);

	bool
	WriteRequest(Str name);

	bool
	PriWriteRequest(Str name);

	bool
	HupRequest(Str name);

	bool
	IsErrorAppear(Str name);

	bool
	IsInvalidFd(Str name);

public:
	Poll(const Poll&);

	void
	FindCorrectFd(int fd);

	pollfd*
	SearchPFD(const Str& name);

public:
	std::vector<Str>		names;
	std::vector<pollfd>		pfd;
	pollfd*					last;

public:
	struct InvalidFdsCount : std::exception { virtual const char* what(void) const throw() { return ("Invalid count of fds."); }};
	struct NameIDDoesntExist : std::exception { virtual const char* what(void) const throw() { return ("Name ID does not exist."); }};

public:
	enum
	{
		AllReq		= POLLIN | POLLOUT | POLLHUP | POLLRDNORM | POLLRDBAND | POLLPRI | POLLWRNORM | POLLWRBAND | POLLERR | POLLNVAL,
		ReadReq		= POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI,
		WriteReq	= POLLOUT | POLLWRNORM | POLLWRBAND,
		RdWrReq		= Poll::ReadReq | Poll::WriteReq,
		ErrorReq	= POLLERR | POLLNVAL,
		ExitReq		= POLLHUP
	};
};

#endif
