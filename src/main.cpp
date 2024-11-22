#include "../includes/Irc.hpp"

int close_and_ret(std::string msg, int srv, int cli){
	if (srv > 0)
		close(srv);
	if (cli > 0)
		close(cli);
	std::cout << YELLOW << msg << RED << " error" << RESET << std::endl;
	return 1;
}

int main(int ac, char **av){
	// initial parsings
	/*if (ac != 3)
		return 1; // ? ./ircserv <port> <pwd>
*/
	// setting up socket
	int srv_status = 0, client_fd = 0;
	struct sockaddr_in srv_addr, client_addr;
	char buf[1024];
	int port = 6667;

	/* UNSURE */
	memset(&srv_addr.sin_zero, 0, sizeof(srv_addr.sin_zero));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addr.sin_port = htons(port);

	memset(&client_addr.sin_zero, 0, sizeof(client_addr.sin_zero));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(port);
	/*        */

	socklen_t client_addr_len = sizeof(client_addr);
	srv_status = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4 | SOCK_STREAM = stream socket, good for chats
	if (srv_status < 0)
		close_and_ret("socket fd", srv_status, client_fd);
	HERE("socket")
	// typecast sockaddr_in to sockaddr *, memset sin_zero padding makes it possible?
	if (bind(srv_status, (struct sockaddr *)&srv_addr, sizeof(srv_addr))< 0)
		close_and_ret("srv bind", srv_status, client_fd);
	HERE("bind")
	if (listen(srv_status, 2) < 0)
		close_and_ret("srv listen", srv_status, client_fd);
	HERE("listen")
	// how to put infinite?
	client_fd = accept(srv_status, (struct sockaddr *)&client_addr, &client_addr_len); 
	// accept connects to 1st socket request found (listen) returns a fd
	if (client_fd < 0)
		close_and_ret("client accept", srv_status, client_fd);
	HERE("accept")
	ssize_t bytes_read = read(client_fd, buf, sizeof(buf) - 1);
	if (bytes_read < 0)
		close_and_ret("client read", srv_status, client_fd);
	buf[bytes_read] = 0;
	HERE("read")
	close(srv_status);
	close(client_fd);
	(void)av;
	(void)ac;
	return 0;
}

