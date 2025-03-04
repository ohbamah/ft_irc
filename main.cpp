/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:21:34 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/04 15:31:56 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

static void	no_args(void)
{
	std::cout << "./ft_irc \e[31m[PORT]\e[0m \e[31m[PASSWORD]\e[0m" << std::endl;
}

static void	signal_handle(int signum)
{
	(void)signum;
	Irc::exitReq = true;
}

int	main(int ac, char** av)
{
	signal(SIGINT, signal_handle);
	if (ac != 3)
		no_args();
	else
	{
		try {
			Irc	irc(std::atoi(av[1]), av[2]);
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
	return (0);
}
