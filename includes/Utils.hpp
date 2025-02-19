/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:19:48 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:26:35 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <poll.h>
# include <vector>

namespace Utils
{
	pollfd
	CreatePollfd(int fd, int eventToTarget);

	const pollfd&
	SearchPollfd(const std::vector<pollfd>& pfd, const int& fdTargeted);

	struct NonExistingPollfd : std::exception { virtual const char* what(void) const throw() { return ("Non-existing pollfd."); }};
}

#endif
