/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:27:12 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 02:43:35 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
#define POLL_HPP

# include <poll.h>
# include <exception>
# include <vector>

class Poll
{
public:
	Poll(void);
	Poll(int pfds[], int pcount);
	~Poll();

	int
	Check(int timeout);

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
	Poll(const Poll&);

	void
	FindCorrectFd(int fd);

private:
	std::vector<pollfd>	pollfds;
	pollfd				last;
	int					count;

public:
	struct InvalidFdsCount : std::exception { virtual const char* what(void) const throw() { return ("Invalid count of fds."); }};
};

#endif
