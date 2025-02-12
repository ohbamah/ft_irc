/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:25:43 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/12 20:48:36 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# include <iostream>
# include <signal.h>
# include <sys/stat.h>
# include <cstdlib>

class Irc
{
public:
	int
	static Start(int port, const char* pass);

private:
};

#endif
