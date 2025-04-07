/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:21:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/07 15:54:44 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

static
std::size_t
at = 0;

fd_set
Utils::CreateSetfd(int fd)
{
	fd_set	fset;

	FD_ZERO(&fset);
	FD_SET(fd, &fset);
	return (fset);
}

//#include <iostream>
//#include <cstdio>
std::string
Utils::Getline(char*& s)
{
	//std::string			ret;
	//std::size_t			pos;
	//std::string			tmp = &s[at];
//
	////std::printf("tmp %d\n", at);
	//if ((pos = tmp.find_first_of('\n')) != std::string::npos || (pos = tmp.find_first_of('\r')) != std::string::npos) //? (|| '\v' || 'r')
	//{
	//	std::size_t i = 0;
	//	for ( ; i < pos ; ++i) //, ++at
	//		ret.push_back(tmp[i]);
	//	//for ( ; s[at] == '\n' && s[at] == '\r' ; ++at)
	//	//	if (s[at] == '\0')
	//	//		break ;
	//}
	//return (ret);
	std::string	ret;
	std::size_t	pos;
	std::string	str(s);

	if ((pos = str.find_first_of('\n')) != std::string::npos || (pos = str.find_first_of('\r')) != std::string::npos)
	{
		for (std::size_t i = 0 ; i < pos ; ++i)
		{
			ret.push_back(s[i]);
			//*s = 0;
			//++s;
		}
	}
	return (ret);
}

void
Utils::ResetGetline(void)
{
	at = 0;
}
