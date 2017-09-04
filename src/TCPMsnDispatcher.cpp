
#include "TCPMsnDispatcher.h"


TCPMsnDispatcher::TCPMsnDispatcher(UsersManager* usersManager) {
	this->sem = new Semaphore(1);
	this->sessions = new map<string, string>();
	this->usersManager = usersManager;

	this->sessionsHandler = new SessionsHandler(this->sessions, this->usersManager);
	this->infoHandler = new InfoHandler(this->usersManager);

	this->isRunning = false;

	this->start();
}

void TCPMsnDispatcher::run(){
	this->isRunning = true;
	while(this->isRunning){
		MultipleTCPSocketsListener* socketsListener = new MultipleTCPSocketsListener();
		vector<User*>* loggedUsers = this->usersManager->getLoggedInUsers();
		sem->lock();
		vector<User*>::iterator it = loggedUsers->begin();
		for(; it != loggedUsers->end(); ++it){
			User* user = (*it);
			socketsListener->addSocket(user->getSocket());
		}
		sem->unlock();
		TCPSocket* readySock = socketsListener->listenToSocket();
		socketsListener->close();
		delete socketsListener;

		if(readySock == NULL) continue;

		User* readyUser = getUserBySocket(readySock);

		// handle socket incoming data
		handle(readyUser);
	}
}

User* TCPMsnDispatcher::getUserBySocket(TCPSocket* sock){
	vector<User*>* loggedUsers = this->usersManager->getLoggedInUsers();
	sem->lock();
	vector<User*>::iterator it = loggedUsers->begin();
	for(; it != loggedUsers->end(); ++it){
		User* user = (*it);
		if(user->getSocket() == sock){
			sem->unlock();
			return user;
		}
	}
	sem->unlock();
	return NULL;
}

void TCPMsnDispatcher::handle(User* user){
	TCPSocket* userSocket = user->getSocket();
	int command = TCPMessanger::readCommand(userSocket);
	switch(command){
		case OPEN_SESSION_WITH_USER:
			this->sessionsHandler->handleStartSession(user);
			break;
		case CLOSE_SESSION_WITH_USER:
			this->sessionsHandler->handleCloseSession(user);
			break;
		case EXIT:
			this->disconnectUser(user);
			break;
		case GET_USERS_LIST:
			this->infoHandler->handleGetUsers(userSocket);
			break;

		case GET_CONNECTED_USERS:
			this->infoHandler->handleGetConnectedUsers(userSocket);
			break;
		case UPDATE_SCORE:
			this->infoHandler->updateScore(userSocket);
			break;
		case GET_SCORE:
			this->infoHandler->handleGetScore(userSocket);
			break;
		default:
			cout << command << " unexpected command, closing connection with: " << user->getUserName() << endl;
			this->disconnectUser(user);
			break;

	}
}

void TCPMsnDispatcher::disconnectUser(User* user){
	if(user->isBusy()){

		this->sessionsHandler->removeSession(user->getUserName(), user->getConnectedUser()->getUserName());
		TCPMessanger::sendCommand(user->getSocket(), CLOSE_SESSION_WITH_USER);

		user->setAvailable();
	}
	TCPMessanger::sendCommand(user->getSocket(), EXIT);
	this->usersManager->logout(user);
}

map<string, string> TCPMsnDispatcher::getSessions(){
	return *this->sessions;
}



void TCPMsnDispatcher::close(){
	this->isRunning = false;
	this->waitForThread();
}

TCPMsnDispatcher::~TCPMsnDispatcher() {
	delete this->sem;
	this->sem = NULL;

	delete this->sessionsHandler;
	this->sessionsHandler = NULL;

	delete this->infoHandler;
	this->infoHandler = NULL;

	this->sessions->clear();
	delete this->sessions;
	this->sessions = NULL;
}
