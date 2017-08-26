/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#ifndef USERSMANAGER_H_
#define USERSMANAGER_H_

#include <string.h>
#include <map>
#include <vector>
#include<iostream>
#include<fstream>
#include "Core/MThread.h"
#include "Core/TCPSocket.h"
#include "Core/MultipleTCPSocketsListener.h"
#include "Common/TCPMessanger.h"
#include "Common/TCPMessengerProtocol.h"
#include "Models/User.h"
#include "Core/Semaphore.h"

using namespace std;

class UsersManager : public MThread{
	vector<TCPSocket*>* notLoggedInPeers;
	vector<User*>* loggedInUsers;
	bool isRunning;
	Semaphore* sem;
private:
	void addToLoggedIn(string userName, TCPSocket* peer);
	void removePeer(TCPSocket* peer);
public:
	UsersManager();
	virtual ~UsersManager();

	bool login(string userName,string password);
	bool registerUser(string userName, string password);
	void updateScore(string userName);
	void logout(User* user);

	vector<string> getAllUsers();
	bool isUserExist(string userName);
	bool isUserLoggedIn(string userName);
	vector<User*>* getLoggedInUsers();

	void addPeer(TCPSocket* peer);
	void run();
	void close();
};

#endif /* USERSMANAGER_H_ */
