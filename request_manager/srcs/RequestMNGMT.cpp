/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestMNGMT.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:20:55 by achansar          #+#    #+#             */
/*   Updated: 2024/03/04 13:28:05 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestMNGMT"

int      RequestMNGMT::requestTypeSwitch(Request request) {
    
    // int method = request.getRequestLine().getMethod();

    switch (request.getRequestLine().getMethod()) {
        case 1:// GET
            break;
        case 2:// POST
            break;
        case 0:// DELETE
            break;
        default:
            break;//    ?
    }

    return 0;
}