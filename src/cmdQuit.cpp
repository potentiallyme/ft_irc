#include "../includes/Irc.hpp"

void	Server::cmdQuit(int i)
{
	// std::cout << "cmdQuit" << " : "  << i << string_array[0] << std::endl;
	int fd = _fds[i].fd;
	CloseClients(fd);
    close(fd);
	std::cout<<PURPLE<<"Client["<< fd <<"]"<<RED<<" has disconnected"<<RESET<<std::endl;
	// (void)i;
	// (void)string_array;
}
