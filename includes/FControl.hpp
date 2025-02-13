/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FControl.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:20:52 by bama              #+#    #+#             */
/*   Updated: 2025/02/13 01:45:58 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FCONTROL_HPP
#define FCONTROL_HPP

# include <exception>
# include <fcntl.h>
# include <unistd.h>

namespace FControl
{
	enum Flags
	{
		NonBlock = O_NONBLOCK,
		Block = ~O_NONBLOCK,
		Append = O_APPEND,
		Sync = O_SYNC,
		Async = O_ASYNC,
		CloseExec = O_CLOEXEC	// Close file descriptor automatically after a call of 'exec()'
	};

	enum LocksCntl
	{
		CheckLocking = F_GETLK,
		TryLock = F_SETLK,
		Lock = F_SETLKW
	};

	enum DupCntl
	{
		DuplicateFD = F_DUPFD,	// Duplicate the fd
		DuplicateFDExec = F_DUPFD_CLOEXEC // Duplicate the fd and close automatically it after a call of 'exec()'
	};

	int
	GetFlags(int fd);

	int
	AppendFlags(int fd, int flags);

	int
	SetFlags(int fd, int flags);

	// @param n is the value of the FileDescriptor duplicated (equal or greater than 'n')
	int
	DupF(int fd, int whichDupSys = FControl::DuplicateFD, int n = 0);

	int
	FLock(int fd, int flags, flock& lockFile);

	int
	FSetOwner(int fd, int pidOwner);

	int
	FGetOwner(int fd);

	struct Failed : std::exception { virtual const char* what(void) const throw() { return ("Fcntl action failed"); }};
};

#endif
