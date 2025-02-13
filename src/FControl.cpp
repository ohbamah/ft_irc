/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FControl.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:25:08 by bama              #+#    #+#             */
/*   Updated: 2025/02/13 01:45:15 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FControl.hpp"

int
FControl::GetFlags(int fd)
{
	return (fcntl(fd, F_GETFL));
}

int
FControl::AppendFlags(int fd, int flags)
{
	return (fcntl(fd, F_SETFL, FControl::GetFlags(fd) & flags));
}

int
FControl::SetFlags(int fd, int flags)
{
	return (fcntl(fd, F_SETFL, flags));
}

int
FControl::DupF(int fd, int dupFlags, int n)
{
	return (fcntl(fd, dupFlags, n));
}

int
FControl::FLock(int fd, int flags, flock& lockFile)
{
	return (fcntl(fd, flags, &lockFile));
}

int
FControl::FGetOwner(int fd)
{
	return (fcntl(fd, F_GETOWN));
}

int
FControl::FSetOwner(int fd, int pidOwner)
{
	return (fcntl(fd, F_SETOWN, pidOwner));
}
