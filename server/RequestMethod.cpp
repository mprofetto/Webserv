/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestMethod.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 15:28:04 by achansar          #+#    #+#             */
/*   Updated: 2024/02/19 17:05:58 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

// Each function must store an error from the requestLine enum.
// Necessaire to build the response

void RequestGet() {
    return;
}


void RequestPost() {
    return;
}


void RequestDelete() {
    return;
}

void    Route::redirectRequest() {
    
    if (_get) {

    } else if (_post) {
        
    } else if (_delete) {

    } else {
        std::cout << "Error" << std::endl;
        return;
    }
    
    return;
}