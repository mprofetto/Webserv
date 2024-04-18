/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 09:59:51 by mprofett          #+#    #+#             */
/*   Updated: 2024/04/17 12:15:31 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <ctime>
# include <list>
# include <string>

class User{
	private:
		std::string				_name;
		std::string				_password;
		std::string				_session_id;
		std::string				_lang;
		std::list<std::string>	_articles;

	public:
		User();
		User(std::string name, std::string password, std::string session_id);
		~User();

		User			&operator=(User &copy);

		std::string				getName(void) const;
		std::string				getPassword(void) const;
		std::string				getSessionId(void) const;
		std::string				getLang(void) const;

		std::list<std::string>	getArticles(void) const;

		void					setLang(std::string &lang);
		void					setSessionId(std::string &session_id);
		void					addArticles(std::string article);
		void					removeArticles(std::string article);
};

#endif
