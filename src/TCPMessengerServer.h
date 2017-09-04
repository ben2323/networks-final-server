
#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__

#include <strings.h>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "Socket/MThread.h"
#include "Socket/TCPSocket.h"
#include "Socket/MultipleTCPSocketsListener.h"
#include "Messenger/TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"
#include "Messenger/TCPMessenger.h"
#include "UsersManager.h"
#include "Users/User.h"
#include "Socket/Semaphore.h"

using namespace std;


class TCPMessengerServer: public MThread{
	Semaphore* sem;

	// add class properties
	TCPSocket* serverSocket;
	bool isRunning;
	TCPMsnDispatcher* dispatcher;
	UsersManager* usersManager;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerServer();
	~TCPMessengerServer();

	void printConnectedUsers();
	void printSessions();
	void printUsersList();


	/**
	 * close the server
	 */
	void close();

	void run();


};
#endif
