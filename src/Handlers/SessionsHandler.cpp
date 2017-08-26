/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#include "SessionsHandler.h"

SessionsHandler::SessionsHandler(map<string, string>* sessions, UsersManager* usersManager) {
	this->sem = new Semaphore(1);
	this->sessions = sessions;
	this->usersManager = usersManager;

}

void SessionsHandler::handleStartSession(User* sourceUser){
	string targetUserName = TCPMessanger::readData(sourceUser->getSocket());
	User* targetUser = this->getUserByUserName(targetUserName);
	if(targetUser == NULL || targetUser->isBusy()){
		cout << targetUserName << " - no available user found" << endl;
		TCPMessanger::sendCommand(sourceUser->getSocket(), SESSION_REFUSED);
		return;
	}
	this->startSessionWithUser(sourceUser, targetUser);
}

void SessionsHandler::handleCloseSession(User* sourceUser){
	string targetUserName = TCPMessanger::readData(sourceUser->getSocket());
	User* targetUser = this->getUserByUserName(targetUserName);
	if(targetUser!=NULL){
		TCPMessanger::sendCommand(targetUser->getSocket(), CLOSE_SESSION_WITH_USER);
		targetUser->setAvailable();
	}
	sourceUser->setAvailable();
	this->removeSession(sourceUser->getUserName(), targetUserName);
}

void SessionsHandler::startSessionWithUser(User* sourceUser, User* targetUser){
	cout <<"starting session between : " << sourceUser->getUserName() << "-> " << targetUser->getUserName() << endl;
	srand(time(NULL));
	int sourcePort = rand() % 1000 + MSNGR_CLIENT_PORT;
	int targetPort;
	do{
		targetPort = rand() % 1000 + MSNGR_CLIENT_PORT;
	}while(sourcePort == targetPort);

	sourceUser->setListeningPort(sourcePort);
	targetUser->setListeningPort(targetPort);

	TCPMessanger::sendCommand(targetUser->getSocket(), OPEN_SESSION_WITH_USER);
	TCPMessanger::sendInt(targetUser->getSocket(), targetPort); // send the port should listen
	TCPMessanger::sendUser(sourceUser, targetUser->getSocket());

	TCPMessanger::sendCommand(sourceUser->getSocket(), SESSION_ESTABLISHED);
	TCPMessanger::sendInt(sourceUser->getSocket(), sourcePort); // send the port should listen
	TCPMessanger::sendUser(targetUser, sourceUser->getSocket());

	sourceUser->setBusy(targetUser);
	targetUser->setBusy(sourceUser);
	this->addSession(sourceUser->getUserName(), targetUser->getUserName());
}

void SessionsHandler::addSession(string a, string b){
	sem->lock();
	if(a.compare(b) > 0){
		this->sessions->insert(pair<string, string>(a,b));
	}
	else{
		this->sessions->insert(pair<string, string>(b,a));
	}
	sem->unlock();
}

void SessionsHandler::removeSession(string a, string b){
	string big;
	string small;
	if(a.compare(b) > 0){
		big = a;
		small = b;
	} else{
		big = b;
		small = a;
	}
	sem->lock();
	map<string, string>::iterator it = this->sessions->find(big);
	if(it == this->sessions->end()){
		sem->unlock();
		return;
	}
	cout << "closing session between: " << a << " -> " << b << endl;
	this->sessions->erase(it);
	sem->unlock();
}

User* SessionsHandler::getUserByUserName(string userName){
	vector<User*>* loggedUsers = this->usersManager->getLoggedInUsers();
	sem->lock();
	vector<User*>::iterator it = loggedUsers->begin();
	for(; it != loggedUsers->end(); ++it){
		User* user = (*it);
		if(user->getUserName().compare(userName) == 0){
			sem->unlock();
			return user;
		}
	}
	sem->unlock();
	return NULL;
}

SessionsHandler::~SessionsHandler() {
	if(this->sem != NULL){
		delete this->sem;
		this->sem = NULL;
	}

	this->sessions = NULL;
	this->usersManager = NULL;
}

