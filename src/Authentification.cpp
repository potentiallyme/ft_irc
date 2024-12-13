#include "../includes/Irc.hpp"

void	Server::checkRegistration(int i)
{
	if (this->_clients[i - 1].getNickname() != "*"
	&& !this->_clients[i - 1].getUsername().empty()
	&& this->_clients[i - 1].getPasswordIsCorrect()
	&& !this->_clients[i - 1].getIsRegistered())
	{
		this->_clients[i - 1].setIsRegistered();
		std::string messfinal = ":localhost 001 " + this->_clients[i - 1].getNickname() + " :Welcome to the Internet Relay Network :" + this->_clients[i - 1].getNickname() + "!" + this->_clients[i - 1].getUsername() + "@localhost\r\n";
		send(this->_fds[i].fd, messfinal.c_str(), messfinal.size(), 0);	
	}
}

int	Server::checkStringFormat(std::string str)
{
	int j = 0;
	while (str[j])
	{
		if (isalnum(str[j]) == 0 && str[j] != '_' && str[j] != '-')
			return 1;
		j++;
	}
	return 0;
}
