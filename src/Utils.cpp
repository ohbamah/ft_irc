/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:21:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/02 19:27:57 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

fd_set
Utils::CreateSetfd(int fd)
{
	fd_set	fset;

	FD_ZERO(&fset);
	FD_SET(fd, &fset);
	return (fset);
}

std::string
Utils::Getline(char*& s)
{
	std::string			ret;
	std::size_t			pos;
	static std::size_t	at = 0;
	std::string			tmp = &s[at];

	if ((pos = tmp.find_first_of('\n')) != std::string::npos || (pos = tmp.find_first_of('\r')) != std::string::npos) //? (|| '\v' || 'r')
	{
		std::size_t i = 0;
		for ( ; i < pos ; ++i)
			ret.push_back(tmp[i]);
		//ret.push_back('\n');//'\r'
		at += pos + 1;
		if (pos == tmp.size() - 1)
			at = 0;
	}
	return (ret);
}
