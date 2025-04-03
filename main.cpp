/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:21:34 by ymanchon          #+#    #+#             */
/*   Updated: 2025/04/03 12:52:02 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"

//#include "Utils.hpp"
//int main()
//{
//	char*	test = "Naaiio\nHHHHh\nfeur\rraciste\r";
//	std::cout << Utils::Getline(test) << std::endl;
//	std::cout << Utils::Getline(test) << std::endl;
//	std::cout << Utils::Getline(test) << std::endl;
//	std::cout << Utils::Getline(test) << std::endl;
//	std::cout << Utils::Getline(test) << std::endl;
//	std::cout << Utils::Getline(test) << std::endl;
//}

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
