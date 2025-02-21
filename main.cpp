/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: claprand <claprand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:21:34 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/20 15:42:13 by claprand         ###   ########.fr       */
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
	std::exit(130);
}

int	main(int ac, char** av)
{
	signal(SIGINT, signal_handle);
	if (ac != 3)
		no_args();
	else
		Irc	irc(std::atoi(av[1]), av[2]);
	return (0);
}
