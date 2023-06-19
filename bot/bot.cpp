#include "bot.hpp"

bot::bot()
{
    _botSocket = 0;
}

bot::~bot()
{

}

int 	bot::runBot(char* buffer, User *user)
{
    //const int BUFFER_SIZE = 512;
    const char* channel = "#channel";

	// std::cout << "hallo" << std::endl;
	 //int bytesRead = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
	 //std::cout << "here" << std::endl;
	 //if (bytesRead == -1) {
	 //    std::cerr << "Error when receiving data" << std::endl;
	 //    break;
	 //    std::cout << "DEBUG" << std::endl;
	 //} else if (bytesRead == 0) {
	 //    std::cout << "Deconnection server" << std::endl;
	 //    break;
	 //}
	 // Traitement des messages reçus
	 std::string message(buffer);
	 if (message.find("PING") != std::string::npos)
	 {
	     // Répondre au PING du serveur pour éviter d'être déconnecté
	     std::string pongCommand = std::string("PONG") + "\r\n";
	     send(user->getSocket(), pongCommand.c_str(), pongCommand.length(), 0);
	 }
    else if (message.find("bonjour") != std::string::npos)
    {
	 // Répondre au PING du serveur pour éviter d'être déconnecté
	 std::string pongCommand = std::string("salo en revoir") + "\r\n";
	 send(user->getSocket(), pongCommand.c_str(), pongCommand.length(), 0);
    }
	 else if (message.find("PRIVMSG " + std::string(channel)) != std::string::npos)
	 {
	     // Traiter les messages privés reçus sur le canal spécifié
	     std::string sender = message.substr(message.find(":") + 1, message.find("!") - 1);
	     std::string receivedMessage = message.substr(message.find(std::string(channel)) + std::string(channel).length() + 2);

	     // Répondre au message privé
	     std::string replyMessage = "PRIVMSG " + sender + " :Bonjour ! J'ai bien reçu votre message : " + receivedMessage + "\r\n";
	     send(user->getSocket(), replyMessage.c_str(), replyMessage.length(), 0);
	 }
    return 0;
}

void	bot::sendFromBot()
{
    const char* nickname = "ircBot";
    int i = 0;

    std::string userCommand = "USER " + std::string(nickname) + " 0 * :" + std::string(nickname) + "\r\n";
    std::string nickCommand = "NICK " + std::string(nickname) + "\r\n";
    std::string passCommand = std::string("PASS pass") + "\r\n";
    std::cout << "socketfd from send " << _botSocket << std::endl;
    usleep(1000);
    i = send(_botSocket, userCommand.c_str(), userCommand.length(), 0);
    std::cout << "i = " << i << std::endl;
    usleep(1000);
    send(_botSocket, nickCommand.c_str(), nickCommand.length(), 0);
    usleep(1000);
    send(_botSocket, passCommand.c_str(), passCommand.length(), 0);
}

int	bot::startBot(int socketFd)
{
    (void)socketFd;
    const char* server = "localhost";
    const int port = 9999;
    int on = -1;

    //ça cree la socket
    _botSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_botSocket == -1)
    {
	 std::cerr << "Error during creation of socket" << std::endl;
	 return 1;
    }
    if (ioctl(_botSocket, FIONBIO, (char *)&on) < 0) {
	 std::cerr << "Error: ioctl() failed" << std::endl;
	 exit(1);
    }
    //ça recupere l'adresse du serveur mon gars
    struct hostent* host = gethostbyname(server);
    if (host == NULL)
    {
	 std::cerr << "Error during the resolution of server address" << std::endl;
	 return 1;
    }
    //ici on configure l'adresse du serveur
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; //AF_INET -> Famille IPV4
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    memcpy(&serverAddress.sin_addr, host->h_addr, host->h_length);
    //connexion au serveur
    usleep(1000);
    if (connect(_botSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1)
    {
	 std::cerr << "Error during connection" << std::endl;
	 return 1;
    }

    std::cout << "bot connected ??" << std::endl;
    // Fermeture de la socket
   // close(botSocket);
    return 0;
}