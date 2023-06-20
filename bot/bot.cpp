#include "bot.hpp"

int	main(int ac, char **av)
{
	if (ac == 4) {

		std::string hostname = av[1];
		std::string password = av[2];
		int port = atoi(av[3]);
		int botSocket;
		struct sockaddr_in serverAddress;

		botSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (botSocket == -1) {
		std::cerr << "Error during creation of socket" << std::endl;
		return 1;
		}

		memset(&serverAddress, 0, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET; //AF_INET -> Famille IPV4
		serverAddress.sin_port = htons(port);	
		
		//	Get server address
		struct hostent *host = gethostbyname(hostname.c_str());
		if (host == NULL) {
			std::cerr << "Error during the resolution of server address" << std::endl;
			return 1;
		}
		memcpy(&serverAddress.sin_addr.s_addr, host->h_addr, host->h_length);	

		// Connexion au serveur
		if (connect(botSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
			std::cerr << "Error during connection" << std::endl;
			return 1;
		}	

		// The botSocket is connected so we can send identification
		std::cout << "Botsocket is connected to the server\nLogin..." << std::endl;	

		std::string sendPass = "PASS " + password + "\r\n";
		std::string sendNick = "NICK ircbot\r\n";
		std::string sendUser = "USER ircbot \r\n";

		send(botSocket, sendPass.c_str(), sendPass.size(), 0);
		send(botSocket, sendNick.c_str(), sendNick.size(), 0);
		send(botSocket, sendUser.c_str(), sendUser.size(), 0);

		std::cout << "The bot is logged !" << std::endl;

		// GESTION DES COMMANDES avec recv()
		std::string botJoinChannel = "JOIN #lol\r\n";

		send(botSocket, botJoinChannel.c_str(), botJoinChannel.size(), 0);
		
		int		bufferSize = 512;
		char	buffer[bufferSize];

		while (1) {
			int bytesRead = recv(botSocket, buffer, bufferSize, 0);
			if (bytesRead == 0) {
				std::cerr << "Error: connection closed" << std::endl;
				return 1;
			}
			else if (bytesRead < 0) {
				std::cerr << "Error: recv() failed" << std::endl;
				return 1;
			}
			else {
					std::cout << "Received : " << buffer << std::endl;
				}
		}
	}
}

/*bot::bot()
{
	_botSocket = 0;
}

bot::~bot()
{

}

int bot::getBotSocket()
{
	return(_botSocket);
}

int 	bot::runBot(char* buffer, User *user, Server *server)
{
	std::string message(buffer);
	std::cout << "from runbot : "  << message << std::endl;
	const char* channelName = "#bot";
	Channel *chan =  server->getChannel(channelName);

	std::string response = ":" + std::string("ircBot") + "!" + std::string("ircBot") + "@locahost " + std::string("PRIVMSG") + " " + channelName + " ";
	if (message.find("PING") != std::string::npos)
	{
	 // Répondre au PING du serveur pour éviter d'être déconnecté
	 std::string pongCommand = response.append("PONG\r\n");
	 sendBotMessageToOtherUsersInChannel(pongCommand, chan, user);
	// send(user->getSocket(), pongCommand.c_str(), pongCommand.length(), 0);
	}
	else if (message.find("bonjour") != std::string::npos)
	{
	 // Répondre au PING du serveur pour éviter d'être déconnecté
	 std::string pongCommand = std::string("bonjour ") + user->getNickname() + " " + "\r\n";
	 send(user->getSocket(), pongCommand.c_str(), pongCommand.length(), 0);
	}
	else if (message.find("msg " + std::string(channelName)) != std::string::npos)
	{
	 // Traiter les messages privés reçus sur le canal spécifié
	 std::string sender = message.substr(message.find(":") + 1, message.find("!") - 1);
	 std::string receivedMessage = message.substr(message.find(std::string(channelName)) + std::string(channelName).length() + 2);

	 // Répondre au message privé
	 std::string replyMessage = "PRIVMSG " + sender + " :Bonjour ! J'ai bien reçu votre message : " + receivedMessage + "\r\n";
	 send(user->getSocket(), replyMessage.c_str(), replyMessage.length(), 0);
	}
	return 0;
}

void    bot::sendBotMessageToOtherUsersInChannel(std::string message, Channel *channel, User *user) {

	std::map<const User*, UserAspects> userMap = channel->getUserList();
	std::map<const User*, UserAspects>::iterator userNode = userMap.begin();
	std::map<const User*, UserAspects>::iterator lastUserNode = userMap.end();

	while (userNode != lastUserNode) {
	 std::string tmp = message;
	 if (userNode->first != user)
		 send(userNode->first->getSocket(), tmp.c_str(), tmp.size(), 0);
	 userNode++;
	}
	return ;
}

void	bot::sendFromBot()
{
	const char* nickname = "ircBot";

	std::string userCommand = "USER " + std::string(nickname) + " 0 * :" + std::string(nickname) + "\r\n";
	std::string nickCommand = "NICK " + std::string(nickname) + "\r\n";
	std::string passCommand = std::string("PASS pass") + "\r\n";
	std::cout << "socketfd from send " << _botSocket << std::endl;
	usleep(1000);
	send(_botSocket, userCommand.c_str(), userCommand.length(), 0);
	usleep(1000);
	send(_botSocket, nickCommand.c_str(), nickCommand.length(), 0);
	usleep(1000);
	send(_botSocket, passCommand.c_str(), passCommand.length(), 0);
}*/
