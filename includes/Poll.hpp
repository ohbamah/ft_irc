/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 18:11:20 by ymanchon         ###   ########.fr       */
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
	Poll(Str names[], int pfds[], int pcount, int reqv = Poll::AllReq);
	~Poll();

	int
	Events(int timeout);

	void
	AddFd(Str name, int fd, int reqv = Poll::AllReq);

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

private:
	Poll(const Poll&);

	void
	FindCorrectFd(int fd);

	std::vector<pollfd>
	Test(void);

private:
	std::map<Str, pollfd>	pollfds;
	pollfd					last;

public:
	struct InvalidFdsCount : std::exception { virtual const char* what(void) const throw() { return ("Invalid count of fds."); }};

public:
	enum
	{
		AllReq		= POLLIN | POLLOUT | POLLHUP | POLLRDNORM | POLLRDBAND | POLLPRI | POLLWRNORM | POLLWRBAND | POLLERR | POLLNVAL,
		ReadReq		= POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI,
		WriteReq	= POLLOUT | POLLWRNORM | POLLWRBAND,
		ErrorReq	= POLLERR | POLLNVAL,
		ExitReq		= POLLHUP
	};
};

#endif
