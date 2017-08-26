/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__

#include <strings.h>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "Core/MThread.h"
#include "Core/TCPSocket.h"
#include "Core/MultipleTCPSocketsListener.h"
#include "Common/TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"
#include "Common/TCPMessanger.h"
#include "UsersManager.h"
#include "Models/User.h"
#include "Core/Semaphore.h"

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

	void printAllUsers();
	void printConnectedUsers();
	void printSessions();

	/**
	 * close the server
	 */
	void close();

	void run();


};
#endif
