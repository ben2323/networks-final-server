/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#ifndef SESSIONSHANDLER_H_
#define SESSIONSHANDLER_H_

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <vector>
#include <map>
#include "../Core/Semaphore.h"
#include "../Models/User.h"
#include "../Common/TCPMessanger.h"
#include "../UsersManager.h"

using namespace std;

class SessionsHandler {
	Semaphore* sem;
	map<string, string>* sessions;
	UsersManager* usersManager;
private:
	void startSessionWithUser(User* sourceUser, User* targetUser);
	User* getUserByUserName(string userName);
public:
	SessionsHandler(map<string,string>* sessions, UsersManager* usersManager);

	void handleStartSession(User* sourceUser);
	void handleCloseSession(User* sourceUser);

	void addSession(string a, string b);
	void removeSession(string a, string b);

	virtual ~SessionsHandler();
};

#endif /* SESSIONSHANDLER_H_ */
