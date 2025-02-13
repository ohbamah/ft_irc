/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ptr.inl                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:46:57 by ymanchon          #+#    #+#             */
/*   Updated: 2025/02/13 16:56:06 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ptr.hpp"

template <typename T>
Ptr<T>::Ptr(void)
{
	this->raw = static_cast<T*>(0);
}

template <typename T>
Ptr<T>::Ptr(T t)
{
	this->raw = new T(t);
}

template <typename T>
Ptr<T>::Ptr(T* t)
{
	this->raw = t;
}

#include <iostream>
template <typename T>
Ptr<T>::~Ptr()
{
	std::cout << "~Ptr\n";
	if (this->raw)
		delete(this->raw);
}

template <typename T>
T*
Ptr<T>::operator*(void)
{
	return (this->raw);
}

template <typename T>
T*
Ptr<T>::operator->(void)
{
	return (this->raw);
}
