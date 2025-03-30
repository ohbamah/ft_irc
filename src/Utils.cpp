/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:21:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/31 01:29:22 by bama             ###   ########.fr       */
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
Utils::BetterGetline(char*& s)
{
	std::string			ret;
	std::size_t			pos;
	static std::size_t	at = 0;
	std::string			tmp = &s[at];

	if ((pos = tmp.find_first_of('\n')) != std::string::npos) //? (|| '\v' || 'r')
	{
		std::size_t i = 0;
		for ( ; i < pos ; ++i)
			ret.push_back(tmp[i]);
		//ret.push_back('\n');
		at += pos;
		if (pos == tmp.size() - 1)
			at = 0;
	}
	return (ret);
}
