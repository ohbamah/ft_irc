/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/16 18:41:21 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
#define POLL_HPP

# include <poll.h>
# include <exception>
# include <vector>
# include <map>
# include <string>
# include <algorithm>

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
	AddFd(const Str& name, int fd, short reqv = Poll::AllReq);

	bool
	ReadRequest(const Str& name);

	bool
	NormalReadRequest(const Str& name);

	bool
	PriReadRequest(const Str& name);

	bool
	HighPriReadRequest(const Str& name);

	bool
	WriteRequest(const Str& name);

	bool
	PriWriteRequest(const Str& name);

	bool
	HupRequest(const Str& name);

	bool
	IsErrorAppear(const Str& name);

	bool
	IsInvalidFd(const Str& name);

	void
	ChangePollName(const Str& oldname, const Str& newname);

private:
	Poll(const Poll&);

	pollfd*
	SearchPFD(const Str& name);

private:
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
