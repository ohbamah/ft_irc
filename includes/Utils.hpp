/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:19:48 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/03 12:48:52 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <poll.h>
# include <sys/select.h>
# include <vector>
# include <string>

// Creer class Select
namespace Utils
{
	fd_set
	CreateSetfd(int fd);
	std::string
	Getline(char*& s);
	void
	ResetGetline(void);

	struct NonExistingPollfd : std::exception { virtual const char* what(void) const throw() { return ("Non-existing pollfd."); }};
}

#endif
