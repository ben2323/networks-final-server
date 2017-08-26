/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#ifndef USER_H_
#define USER_H_

#include <string.h>
#include "../Core/TCPSocket.h"

using namespace std;

class User {
	string userName;
	TCPSocket* socket;
	bool busy;
	string connectedRoomName;
	User* connectedUser;
	int listeningPort;

public:
	User(string userName, TCPSocket* socket);
	User(string userName, string ip, int port, int listeningPort);
	virtual ~User();

	string getUserName();
	string getIP();
	int getPort();
	TCPSocket* getSocket();
	bool isBusy();
	void setBusy(User* user);
	void setBusy(string roomName);
	string getConnectedRoomName();
	User* getConnectedUser();
	void setAvailable();

	void setListeningPort(int port);
	int getListeningPort();
};

#endif /* USER_H_ */
