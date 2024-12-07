// #include "../includes/Irc.hpp"

// void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array, std::string buffer)
// {
// 	// if (this->isRegistered(i) == 0)
// 	// {
// 	// 	std::cout << "Client not registered"  << std::endl;
// 	// 	return ;
// 	// }
// 	std::ostringstream str1;
// 	str1 << this->_clients[i - 1].getFd();
// 	std::string fd_string = str1.str();
// 	std::string msg;
// 	if (string_array.size() < 3)
// 	{
// 		msg = "[Error 461] client " + fd_string + " : Wrong number of parameters\n";
// 		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 		return;
// 	}


// 	std::string message;
// 	if (string_array[1][0] == '#')
// 	{
// 		int pos = buffer.find("#");
// 		int pos2 = buffer.find(" ", pos) + 1;
// 		message = buffer.substr(pos2);

// 		int k = 0;
// 		while (k < static_cast<int>(getChannels().size()))
// 		{
// 			if (getChannels()[k]->getName() == string_array[1])
// 				break;
// 			k++;
// 		}
// 		if (k == static_cast<int>(getChannels().size()))
// 		{
// 			msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such channel\n";
// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 			return;
// 		}

// 		for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != _channels.end(); it++)
// 		{
// 			if ((*it)->getName() == string_array[1])
// 			{
// 				int k = 0;
// 				// std::cout << "[" << this->_clients[i - 1].getNickname() << "]" << std::endl;
// 				while (k < static_cast<int>((*it)->getClients().size()))
// 				{
// 					if ((*it)->getClients()[k]->getNickname() == this->_clients[i - 1].getNickname())
// 						break;
// 					k++;
// 				}
// 				// std::cout << k << " vs " << (*it).getClients().size() << std::endl;
// 				if (k == static_cast<int>((*it)->getClients().size()))
// 				{
// 					msg = "[Error 404] client " + fd_string + " " + string_array[1] + " : Cannot send to channel\n";
// 					send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 					return;
// 				}
#include "../includes/Irc.hpp"

void	Server::cmdPrivmsg(int i, std::vector<std::string> string_array, std::string buffer)
{
	// if (this->isRegistered(i) == 0)
	// {
	// 	std::cout << "Client not registered"  << std::endl;
	// 	return ;
	// }
	std::ostringstream str1;
	str1 << this->_clients[i - 1].getFd();
	std::string fd_string = str1.str();
	std::string msg;
	if (string_array.size() < 3)
	{
		msg = "[Error 461] client " + fd_string + " : Wrong number of parameters\n";
		send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
		return;
	}


	std::string message;
	if (string_array[1][0] == '#')
	{
		int pos = buffer.find("#");
		int pos2 = buffer.find(" ", pos) + 1;
		message = buffer.substr(pos2);

		int k = 0;
		while (k < static_cast<int>(getChannels().size()))
		{
			if (getChannels()[k]->getName() == string_array[1])
				break;
			k++;
		}
		if (k == static_cast<int>(getChannels().size()))
		{
			msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such channel\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}

		for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != _channels.end(); it++)
		{
			if ((*it)->getName() == string_array[1])
			{
				int k = 0;
				// std::cout << "[" << this->_clients[i - 1].getNickname() << "]" << std::endl;
				while (k < static_cast<int>((*it)->getClients().size()))
				{
					if ((*it)->getClients()[k]->getNickname() == this->_clients[i - 1].getNickname())
						break;
					k++;
				}
				// std::cout << k << " vs " << (*it).getClients().size() << std::endl;
				if (k == static_cast<int>((*it)->getClients().size()))
				{
					msg = "[Error 404] client " + fd_string + " " + string_array[1] + " : Cannot send to channel\n";
					send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
					return;
				}


				
				
// 				int j = 0;
// 				while (j < static_cast<int>((*it)->getClients().size()))
// 				{
// 					std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + message + "\n";
// 					send((*it)->getClients()[j]->getFd(), msg.c_str(), msg.length(), 0);
// 					j++;
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		int pos = buffer.find(string_array[1]);
// 		int pos2 = buffer.find(" ", pos) + 1;
// 		message = buffer.substr(pos2);

// 		int k = 0;
// 		while (k < static_cast<int>(getClients().size()))
// 		{
// 			if (getClients()[k].getNickname() == string_array[1])
// 				break;
// 			k++;
// 		}
// 		if (k == static_cast<int>(getClients().size()))
// 		{
// 			msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such nickname\n";
// 			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
// 			return;
// 		}
// 		for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
// 		{
// 			if ((*it).getNickname() == string_array[1])
// 			{
// 				std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + message + "\n";
// 				// std::cout << msg << std::endl;
// 				send((*it).getFd(), msg.c_str(), msg.length(), 0);
// 				// send((*it).getFd(), string_array[2].c_str(), string_array[2].length(), 0);
// 			}
// 		}
// 	}
// }
				int j = 0;
				while (j < static_cast<int>((*it)->getClients().size()))
				{
					std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + message + "\n";
					send((*it)->getClients()[j]->getFd(), msg.c_str(), msg.length(), 0);
					j++;
				}
				break;
			}
		}
	}
	else
	{
		int pos = buffer.find(string_array[1]);
		int pos2 = buffer.find(" ", pos) + 1;
		message = buffer.substr(pos2);

		int k = 0;
		while (k < static_cast<int>(getClients().size()))
		{
			if (getClients()[k].getNickname() == string_array[1])
				break;
			k++;
		}
		if (k == static_cast<int>(getClients().size()))
		{
			msg = "[Error 401] client " + fd_string + " " + string_array[1] + " : No such nickname\n";
			send(this->_clients[i - 1].getFd(), msg.c_str(), msg.length(), 0);
			return;
		}
		for (std::vector<Client>::iterator it = this->_clients.begin(); it != _clients.end(); it++)
		{
			if ((*it).getNickname() == string_array[1])
			{
				std::string msg = "[" + this->_clients[i - 1].getNickname() + "] : " + message + "\n";
				// std::cout << msg << std::endl;
				send((*it).getFd(), msg.c_str(), msg.length(), 0);
				// send((*it).getFd(), string_array[2].c_str(), string_array[2].length(), 0);
			}
		}
	}
}
