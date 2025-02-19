/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:19:48 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 15:05:10 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <poll.h>
# include <sys/select.h>
# include <vector>

// Creer class Select
namespace Utils
{
	fd_set
	CreateSetfd(int fd);

	struct NonExistingPollfd : std::exception { virtual const char* what(void) const throw() { return ("Non-existing pollfd."); }};
}

#endif
