#include "bot.hpp"

std::string					retreiveMessageFromBuffer(char *buffer);
std::vector<std::string>	splitString(std::string str, const char *delim);
int							botBehavior(std::string message, int botSocket);
void						rebuildMessage(std::vector<std::string> &vector, int index);

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
		std::cout << " /BOT\\ Login..." << std::endl;

		std::string sendPass = "PASS " + password + "\r\n";
		std::string sendNick = "NICK ircBot\r\n";
		std::string sendUser = "USER ircBot \r\n";

		// We send connection details to log to the server
		send(botSocket, sendPass.c_str(), sendPass.size(), 0);
		send(botSocket, sendNick.c_str(), sendNick.size(), 0);
		send(botSocket, sendUser.c_str(), sendUser.size(), 0);

		std::cout << " /BOT\\ The bot is logged !" << std::endl;

		// GESTION DES COMMANDES avec recv()
		std::string botJoinChannel = "JOIN #bot\r\n";

		std::cout << botJoinChannel << std::endl;
		if (send(botSocket, botJoinChannel.c_str(), botJoinChannel.size(), 0) <= 0) {
			std::cerr << "Error: send failed to join channel" << std::endl;
		}
		
		int		bufferSize = 512;
		char	buffer[bufferSize];	

		std::cout << "Ready to receive data!" << std::endl;
		while (1) {
			memset(buffer, 0, bufferSize);
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
					std::string message;
					// We handle the message here
					std::cout << "Received : " << buffer << std::endl;
					// First we need to parse the buffer so we retreive only the message
					message	= retreiveMessageFromBuffer(buffer);
					// Then we can proceed the bot reponses
					std::cout << "MESSAGE ===> " << message << std::endl;
					if (!message.empty())
						botBehavior(message, botSocket);
				}
		}
	}
}

std::string	retreiveMessageFromBuffer(char *buffer) {

	std::vector<std::string>	stringElements = splitString(buffer, ":");
	int							id = stringElements.size() - 1;

	rebuildMessage(stringElements, id);
	return stringElements[id];
}

void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out)
{
	char *token = strtok(const_cast<char*>(str.c_str()), delim);
	while (token != NULL)
	{
		out.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
}

std::vector<std::string> splitString(std::string str, const char *delim)
{
	std::vector<std::string> out;
	tokenize(str, delim, out);
	return (out);
}

void rebuildMessage(std::vector<std::string> &vector, int index) {

	size_t i = index + 1;
	while (i < vector.size()) {
		vector[index].append(" ");
		vector[index].append(vector[i]);
		i++;
	}
	vector[index].append("\0");
	return ;
}

int 	botBehavior(std::string message, int botSocket)
{
	const char* channelName = "#bot";

	std::string response = std::string("PRIVMSG") + " " + channelName + " :";
	if (message.find("PING") != std::string::npos)
	{
		// Répondre au PING du serveur pour éviter d'être déconnecté
	 	std::string pongCommand = response.append("PONG\r\n");
		std::cerr << "PING MESSAGE RECEIVED : sending response.." << std::endl;
		send(botSocket, pongCommand.c_str(), pongCommand.length(), 0);
		std::cerr << "Response sent" << std::endl;
	}
	// else if (message.find("bonjour") != std::string::npos)
	// {
	//  // Répondre au PING du serveur pour éviter d'être déconnecté
	//  std::string pongCommand = std::string("bonjour ") + user->getNickname() + " " + "\r\n";
	//  send(user->getSocket(), pongCommand.c_str(), pongCommand.length(), 0);
	// }
	// else if (message.find("msg " + std::string(channelName)) != std::string::npos)
	// {
	//  // Traiter les messages privés reçus sur le canal spécifié
	//  std::string sender = message.substr(message.find(":") + 1, message.find("!") - 1);
	//  std::string receivedMessage = message.substr(message.find(std::string(channelName)) + std::string(channelName).length() + 2);

	//  // Répondre au message privé
	//  std::string replyMessage = "PRIVMSG " + sender + " :Bonjour ! J'ai bien reçu votre message : " + receivedMessage + "\r\n";
	//  send(user->getSocket(), replyMessage.c_str(), replyMessage.length(), 0);
	// }
	return 0;
}
