/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:21:42 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/19 14:26:46 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

pollfd
Utils::CreatePollfd(int fd, int eventToTarget)
{
	pollfd	ret;

	ret.events = eventToTarget;
	ret.fd = fd;
	ret.revents = 0;
	return (ret);
}

const pollfd&
Utils::SearchPollfd(const std::vector<pollfd>& pfd, const int& fdTargeted)
{
	for (unsigned long i = 0 ; i < pfd.size() ; ++i)
		if (pfd[i].fd == fdTargeted)
			return (pfd[i]);
	throw (NonExistingPollfd());
}
