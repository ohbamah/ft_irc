/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:19:48 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/31 01:12:41 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <poll.h>
# include <sys/select.h>
# include <string>
# include <vector>

// Creer class Select
namespace Utils
{
	fd_set
	CreateSetfd(int fd);
	std::string
	BetterGetline(char*& s);

	struct NonExistingPollfd : std::exception { virtual const char* what(void) const throw() { return ("Non-existing pollfd."); }};
}

#endif
