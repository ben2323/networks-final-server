
#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>
#include "Messenger/TCPMessenger.h"
#include "UsersManager.h"
#include "Handlers/InfoHandler.h"
#include "Handlers/SessionsHandler.h"
#include "Socket/MThread.h"
#include "Socket/MultipleTCPSocketsListener.h"
#include "Socket/Semaphore.h"
#include "Socket/TCPSocket.h"

using namespace std;

class TCPMsnDispatcher : public MThread {
	Semaphore* sem;
	UsersManager* usersManager;
	map<string, string>* sessions;

	InfoHandler* infoHandler;
	SessionsHandler* sessionsHandler;

	bool isRunning;
private:
	User* getUserBySocket(TCPSocket* sock);
	User* getUserByUserName(string userName);

	void handle(User* user);
	void disconnectUser(User* user);
public:
	TCPMsnDispatcher(UsersManager* usersManager);
	virtual ~TCPMsnDispatcher();

	void close();

	void run();

	map<string, string> getSessions();

};


#endif /* TCPMSNDISPATCHER_H_ */
