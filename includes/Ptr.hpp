/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ptr.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:44:40 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/14 15:06:08 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PTR_HPP
#define PTR_HPP

template <typename T>
class Ptr
{
public:
	Ptr(void);
	Ptr(T);
	Ptr(T*);
	~Ptr();

	T*	operator*(void);
	T*	operator->(void);

private:
	T*	raw;
};

# include "Ptr.inl"

#endif
