/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#include "InfoHandler.h"

InfoHandler::InfoHandler(UsersManager* usersManager) {
	this->sem = new Semaphore(1);
	this->usersManager = usersManager;

}

void InfoHandler::handleGetConnectedUsers(TCPSocket* sock){
	vector<User*>* users = this->usersManager->getLoggedInUsers();
	sem->lock();
	TCPMessanger::sendCommand(sock, GET_CONNECTED_USERS);
	TCPMessanger::sendInt(sock, users->size());
	vector<User*>::iterator it = users->begin();
	for(; it != users->end(); ++it){
		TCPMessanger::sendData(sock, (*it)->getUserName());
	}
	sem->unlock();
}

void InfoHandler::handleGetUsers(TCPSocket* sock){
	vector<string> users = this->usersManager->getAllUsers();
	TCPMessanger::sendCommand(sock, GET_USERS_LIST);
	sem->lock();
	TCPMessanger::sendInt(sock, users.size());
	vector<string>::iterator it = users.begin();
	for(; it != users.end(); ++it){
		TCPMessanger::sendData(sock, *it);
	}
	sem->unlock();
}

void InfoHandler::handleGetScore(TCPSocket* sock){
	vector<string> users = this->usersManager->getAllUsers();
	TCPMessanger::sendCommand(sock, GET_SCORE);
	sem->lock();
	TCPMessanger::sendInt(sock, users.size() - 1);
	vector<string>::iterator it = users.begin();
	for(; it != users.end(); ++it){
		int count = 0;
		string line;
			ifstream scoresFile("scores.txt");
			if (scoresFile.is_open())
			{
				while (scoresFile.good())
				{
					getline(scoresFile, line);
					if (line.compare(*it)==0)
					{
						count++;
					}
				}
			}
		TCPMessanger::sendData(sock, *it);
		TCPMessanger::sendInt(sock, count);
	}
	sem->unlock();

}

void InfoHandler::updateScore(TCPSocket* sock){
	string userName = TCPMessanger::readData(sock);
	this->usersManager->updateScore(userName);
}


InfoHandler::~InfoHandler() {
	if(this->sem != NULL){
		delete this->sem;
		this->sem = NULL;
	}

	this->usersManager = NULL;
}

