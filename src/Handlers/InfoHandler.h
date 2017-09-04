
#ifndef INFOHANDLER_H_
#define INFOHANDLER_H_

#include <string.h>
#include <vector>
#include <map>
#include "../Socket/Semaphore.h"
#include "../Users/User.h"
#include "../Messenger/TCPMessenger.h"
#include "../UsersManager.h"

using namespace std;

class InfoHandler {
	Semaphore* sem;
	UsersManager* usersManager;

private:
	void sendAllUsers(TCPSocket* sock);
	void sendConnectedUsers(TCPSocket* sock);


public:
	InfoHandler(UsersManager* usersManager);

	void handleGetUsers(TCPSocket* sock);
	void handleGetScore(TCPSocket* sock);
	void handleGetConnectedUsers(TCPSocket* sock);
	void updateScore(TCPSocket* sock);


	virtual ~InfoHandler();
};

#endif /* INFOHANDLER_H_ */
