/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Select.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:18:24 by ymanchon          #+#    #+#             */
/*   Updated: 2025/03/07 15:53:48 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_HPP
#define SELECT_HPP

# include <vector>
# include <algorithm>
# include <sys/select.h>

// Mettre a jour Select::fd_max lorsqu'un fd est Remove
// Verifier si un fd existe deja dans le set write/read/except
class Select
{
public:
	Select(void);
	~Select();

	void
	SnapEvents(int timeout);

	int
	CanRead(int fd);
	int
	CanWrite(int fd);
	int
	Exception(int fd);

	void
	AddWriteReq(int fd);
	void
	AddReadReq(int fd);
	void
	AddExcpReq(int fd);
	void
	RemoveWriteReq(int fd);
	void
	RemoveReadReq(int fd);
	void
	RemoveExcpReq(int fd);

private:
	int
	__Max(void);

	void
	__ResetSets(void);

private:
	int					max_fd;
	std::vector<int>	rdfd;
	std::vector<int>	wrfd;
	std::vector<int>	exfd;
	fd_set				rd_set;
	fd_set				wr_set;
	fd_set				ex_set;

public:
	struct FatalError : std::exception { inline virtual const char* what(void) const throw() {return ("Fatal error!"); } };
	struct NoFDSet : std::exception { inline virtual const char* what(void) const throw() {return ("No FD set..."); } };
	struct FDNotInSelect : std::exception { inline virtual const char* what(void) const throw() {return ("FD not existing in Select object!"); } };
};

#endif
