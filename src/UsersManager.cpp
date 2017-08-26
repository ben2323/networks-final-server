/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */


#include "UsersManager.h"

UsersManager::UsersManager() {
	this->sem = new Semaphore(1);
	this->notLoggedInPeers = new vector<TCPSocket*>();
	this->loggedInUsers = new vector<User*>();
	this->isRunning = false;

	this->start();
}

void UsersManager::run(){
	this->isRunning = true;
	while(this->isRunning){
		MultipleTCPSocketsListener* listener = new MultipleTCPSocketsListener();
		sem->lock();
		listener->addSockets(*this->notLoggedInPeers);
		sem->unlock();
		TCPSocket* readySock = listener->listenToSocket();
		listener->close();
		delete listener;
		if(readySock == NULL){
			continue;
		}

		int command = TCPMessanger::readCommand(readySock);
		string userName;
		string password;
		switch(command){
			case LOGIN:
				userName = TCPMessanger::readData(readySock);
				password = TCPMessanger::readData(readySock);
				if(!login(userName, password)){
					cout<<"failed to logged in: " << userName << endl;
					TCPMessanger::sendCommand(readySock, LOGIN_FAILED);
				}
				else{
					cout << "successfully logged in user: " << userName << endl;
					TCPMessanger::sendCommand(readySock, LOGIN);
					this->addToLoggedIn(userName, readySock);
				}
				break;
			case REGISTER:
				userName = TCPMessanger::readData(readySock);
				password = TCPMessanger::readData(readySock);
				if(!this->registerUser(userName, password)){
					cout << "failed to register : " << userName << endl;
					TCPMessanger::sendCommand(readySock, REGISTER_FAILED);
				}
				else{
					cout << "successfully registered user: " << userName << endl;
					TCPMessanger::sendCommand(readySock, REGISTER);
					this->addToLoggedIn(userName, readySock);
				}
				break;
			default:
				cout<< "not login command, closing session: " << readySock->fromAddr()<<endl;
				TCPMessanger::sendCommand(readySock, EXIT);
				readySock->cclose();
				this->removePeer(readySock);
				break;
		}
	}
}

bool UsersManager::isUserExist(string userName){
	vector<string> users = this->getAllUsers();
	sem->lock();
	vector<string>::iterator it = users.begin();
	for(;it != users.end(); ++it){
		if((*it).compare(userName) == 0){
			sem->unlock();
			return true;
		}
	}
	sem->unlock();
	return false;
}

void UsersManager::addPeer(TCPSocket* peer){
	sem->lock();
	this->notLoggedInPeers->push_back(peer);
	sem->unlock();
}

void UsersManager::removePeer(TCPSocket* peer){
	sem->lock();
	vector<TCPSocket*>::iterator it = this->notLoggedInPeers->begin();
	for(;it != this->notLoggedInPeers->end(); ++it){
		if((*it) == peer){
			this->notLoggedInPeers->erase(it);
			break;
		}
	}
	sem->unlock();
}

void UsersManager::addToLoggedIn(string userName, TCPSocket* peer){
	this->removePeer(peer);
	sem->lock();
	this->loggedInUsers->push_back(new User(userName, peer));
	sem->unlock();
}

vector<string> UsersManager::getAllUsers(){
	vector<string> users;
	string line;
	sem->lock();
	ifstream usersFile("users.txt");
	if (usersFile.is_open())
	{
		while (usersFile.good())
		{
			getline(usersFile, line);
			string delimiter = " ";
			string userName = line.substr(0,line.find(delimiter));
			users.push_back(userName);
		}
	}
	usersFile.close();
	sem->unlock();
	return users;
}

vector<User*>* UsersManager::getLoggedInUsers(){
	return this->loggedInUsers;
}

bool UsersManager::isUserLoggedIn(string userName){
	sem->lock();
	vector<User*>::iterator it = this->loggedInUsers->begin();
	for(; it != this->loggedInUsers->end(); ++it){
		if((*it)->getUserName().compare(userName) == 0){
			sem->unlock();
			return true;
		}
	}
	sem->unlock();
	return false;
}

bool UsersManager::registerUser(string userName, string password){
	if(this->isUserExist(userName)){
		return false;
	}
	ofstream usersFile;
	sem->lock();
	usersFile.open ("users.txt", std::ios_base::app);
	usersFile << userName << " "<<password<<endl;
	usersFile.close();
	sem->unlock();
	return true;
}

void UsersManager::updateScore(string userName){
	ofstream scoresFile;
	sem->lock();
	scoresFile.open ("scores.txt", std::ios_base::app);
	scoresFile << userName <<endl;
	scoresFile.close();
	sem->unlock();
}

bool UsersManager::login(string userName, string password){
	if(!this->isUserExist(userName)){ // the user not exist
		return false;
	}
	if(this->isUserLoggedIn(userName)){ // the user already connected
		return false;
	}
	bool returnValue = false;
	string delimeter = " ";
	string line;
	sem->lock();
	ifstream usersFile("users.txt");
	if (usersFile.is_open())
	{
		while (usersFile.good())
		{
			getline(usersFile, line);
			if (line.compare(userName + delimeter + password)==0)
			{
				returnValue = true;
				break;
			}
		}
	}
	usersFile.close();
	sem->unlock();
	return returnValue;
}

void UsersManager::logout(User* user){
	sem->lock();
	vector<User*>::iterator it = this->loggedInUsers->begin();
	for(; it != this->loggedInUsers->end(); ++it){
		if((*it) == user){
			delete (*it);
			this->loggedInUsers->erase(it);
			sem->unlock();
			return;
		}
	}
	sem->unlock();
}

void UsersManager::close(){
	this->isRunning = false;
	this->waitForThread();

	vector<TCPSocket*>::iterator it = this->notLoggedInPeers->begin();
	for(;it!=this->notLoggedInPeers->end();++it){
		(*it)->cclose();
	}
	this->notLoggedInPeers->clear();

	vector<User*>::iterator it2 = this->loggedInUsers->begin();
	for(;it2!=this->loggedInUsers->end();++it2){
		delete (*it2);
	}
	this->loggedInUsers->clear();
}

UsersManager::~UsersManager() {
	delete this->notLoggedInPeers;
	this->notLoggedInPeers = NULL;

	delete this->loggedInUsers;
	this->loggedInUsers = NULL;

	delete this->sem;
	this->sem = NULL;
}

