#include "../includes/Channel.hpp"


Channel::Channel(const std::string& n): name(n), userLimits(100), topic("")  // limits set to?
{
}

const std::string& Channel::getName() const
{
    return (name);
}

void Channel::addClient(Client* client)
{
    if (clients.size() >= static_cast<std::size_t>(userLimits))
    {
        std::cerr << "Channel is full! Cannot add client." << std::endl;
        return ;
    }
    clients.push_back(client);
    std::cerr << "Client" << client->getNickname() << " joined channel: " << name << std::endl;
}

// void Channel::removeClient(Client* client)
// {
//     for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
//     {
//         if (*it == client)
//         {
//             clients.erase(it);
//             std::cout << "Client" << client->getNickname() << " left channel: " << name << std::endl;
//             break ;
//         }
//     }
//     std::cerr << "Client not found in the channel!" << std::endl;;
// }


void Channel::broadcastMessage(std::string msg) //server msg
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        clients[i]->sendMessage(msg);
    }
}

void Channel::broadcastMessage(const std::string &message, Client *excludeClient)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it != excludeClient)
		{
			(*it)->sendMessage(message);
		}
	}
}

void Channel::changeCh_pwd(std::string newCh_pwd)
{
    Ch_pwd = newCh_pwd;
    std::cout << "Channel password updated.\n";
}

bool Channel::isOperator(Client* client) const
{
    for (std::vector<Client*>::const_iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

void Channel::addOperator(Client* client)
{
    if (client == NULL)
    {
        std::cerr << "Error: Cannot add a null client as operator." << std::endl;
        return;
    }
    if (!isOperator(client))
    {
        operators.push_back(client);
        std::cout << "Added operator: " << client->getName() << std::endl;
    }
    else
    {
        std::cout << "Client is already an operator: " << client->getName() << std::endl;
    }
}

void Channel::removeOperator(Client* client)
{
    for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == client) {
            operators.erase(it);
            break;
        }
    }
}

void Channel::kickClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
	{
		clients.erase(it);
		client->removeChannel(this);
		std::cout << "Client " << client->getNickname() << " has been removed from channel " << name << std::endl;
	}
	else
	{
		std::cerr << "Client not found in channel " << name << "!" << std::endl;
	}
}

void Channel::inviteClient(Client* client)
{
    if (std::find(clients.begin(), clients.end(), client) != clients.end())
    {
        std::cerr << "Error: Client " << client->getNickname() << " is already in the channel " << name << "." << std::endl;
        return ;
    }
    clients.push_back(client);
    std::cout << "Client " << client->getNickname() << " has been invited to the channel " << name << "." << std::endl;
}

// void Channel::addInvitedClient(Client* client) {
//     if (std::find(invitedClients.begin(), invitedClients.end(), client) == invitedClients.end()) {
//         invitedClients.push_back(client);
//     }
// }

// void Channel::removeInvitedClient(Client* client) {
//     std::vector<Client*>::iterator it = std::find(invitedClients.begin(), invitedClients.end(), client);
//     if (it != invitedClients.end()) {
//         invitedClients.erase(it);
//     }
// }

const std::vector<Client*>& Channel::getInvitedClients() const {
    return invitedClients;
}

void Channel::setTopic(const std::string& newTopic)
{
    topic = newTopic;
    std::cout << "Channel topic updated to: " << topic << std::endl;
}

std::string Channel::getTopic() const
{
    return (topic);
}

std::string intToString(int num)
{
    if (num == 0) return "0";
    std::string result;
    bool isNegative = (num < 0);
    if (isNegative) num = -num;

    while (num > 0)
    {
        result.insert(result.begin(), '0' + (num % 10));
        num /= 10;
    }

    if (isNegative) result.insert(result.begin(), '-');
    return result;
}

void Channel::setMode(const std::string& modeStr, const std::string& value, Client* client)
{
    if (modeStr.empty())
    {
        client->sendMessage(":localhost 461 MODE :Not enough parameters");
        return;
    }

    char operation = modeStr[0]; // '+' or '-'
    if (operation != '+' && operation != '-')
    {
        client->sendMessage(":localhost 501 :Unknown MODE flag");
        return;
    }

    // Apply modes one by one
    for (size_t i = 1; i < modeStr.size(); ++i)
    {
        char mode = modeStr[i];
        switch (mode)
        {
            case 'i': // Invite-only
                inviteOnly = (operation == '+');
                broadcastMessage(":Server MODE " + name + " " + operation + "i");
                break;

            case 't': // Topic restrictions
                topicRestricted = (operation == '+');
                broadcastMessage(":Server MODE " + name + " " + operation + "t");
                break;

            case 'k': // Channel password
                if (operation == '+')
                {
                    Ch_pwd = value;
                    broadcastMessage(":Server MODE " + name + " " + operation + "k " + value);
                }
                else
                {
                    Ch_pwd.clear();
                    broadcastMessage(":Server MODE " + name + " " + operation + "k");
                }
                break;

            case 'o': // Operator privileges
            {
                Server serv;
                Client* targetClient = serv.findClientByNickname(value);
                if (!targetClient)
                {
                    client->sendMessage(":localhost 401 " + value + " :No such nick/channel");
                    break;
                }

                if (operation == '+')
                {
                    if (!isOperator(targetClient))
                    {
                        addOperator(targetClient);
                        broadcastMessage(":Server MODE " + name + " " + operation + "o " + value);
                    }
                    else
                    {
                        client->sendMessage(":localhost 443 " + value + " " + name + " :is already an operator");
                    }
                }
                else
                {
                    removeOperator(targetClient);
                    broadcastMessage(":Server MODE " + name + " " + operation + "o " + value);
                }
                break;
            }

            case 'l': // User limit
                if (operation == '+')
                {
					
                    userLimits = std::atoi(value.c_str());
                    broadcastMessage(":Server MODE " + name + " " + operation + "l " + intToString(userLimits));
                }
                else
                {
                    userLimits = -1; // Remove limit
                    broadcastMessage(":Server MODE " + name + " " + operation + "l");
                }
                break;

            default:
                client->sendMessage(":localhost 472 " + std::string(1, mode) + " :is unknown mode char to me");
                break;
        }
    }
}


std::string Channel::getMode(const std::string& mode) const
{
    std::map<std::string, std::string>::const_iterator it = modes.find(mode);
    if (it != modes.end())
        return (it->second);
    return ("");
}

bool Channel::isEmpty() const
{
    return clients.empty();
}

bool Channel::isClientInChannel(Client* client) const
{
    return std::find(clients.begin(), clients.end(), client) != clients.end();
}

void Channel::joinChannel(Client* client, const std::string& password)
{
    if (std::find(clients.begin(), clients.end(), client) != clients.end())
    {
        client->sendMessage(":localhost 443 " + client->getNickname() + " " + name + " : You are already in the channel\n");
        return;
    }
    // check invite
    if (inviteOnly) {
        if (!client->isInvited(client, this))
        {
            client->sendMessage(":localhost 473 " + name + " : You need an invitation to join\n");
            return;
        }
    }
    // check password
    if (!Ch_pwd.empty() && Ch_pwd != password) {
        client->sendMessage(":localhost 475 " + name + " : Incorrect channel password\n");
        return;
    }
    // check amount of clients
    if (clients.size() >= static_cast<std::size_t>(userLimits)) {
        client->sendMessage(":localhost 471 " + name + " : Channel is full\n");;
        return;
    }
    //first one in channel
    if (operators.empty()) {
        operators.push_back(client);
        client->sendMessage("You are now an operator in channel " + name);
    }
    clients.push_back(client);
    client->addChannel(this);

    broadcastMessage(":" + client->getNickname() + " JOIN " + name + "\n");
    std::cout << "Client " << client->getNickname() << " successfully joined channel " << name << std::endl;
}


void Channel::partChannel(Client* client, const std::string& message)
{
    std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);
    if (it == clients.end()) {
        client->sendMessage(":localhost 441 " + client->getNickname() + " " + name + " :They aren't on that channel\n");
        return;
    }
    std::string partMessage = client->getNickname() + " has left the channel " + name;
    if (!message.empty()) {
        partMessage += " (" + message + ")";
    }
    broadcastMessage(partMessage);
    clients.erase(it);
    client->removeChannel(this);
}

std::vector<Client*> Channel::getClients(void)
{
	return this->clients;
}
