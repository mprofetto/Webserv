/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 09:59:51 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/12 11:33:51 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <ctime>
# include <string>

class User{
	private:
		std::string	_name;
		std::string	_password;
		std::string	_session_id;
		time_t		_session_expiration_date;

	public:
		User();
		User(std::string name, std::string password, std::string session_id, time_t session_expiration_time);
		~User();

		User			&operator=(User &copy);

		std::string		getName(void) const;
		std::string		getPassword(void) const;
		std::string		getSessionId(void) const;
		time_t			getSessionExpirationDate(void) const;

		void			setSessionId(std::string &session_id);
		void			setSessionExpirationDate(time_t &session_expiration_date);

		bool			sessionExpired(time_t	&actualDate) const;
};

#endif
