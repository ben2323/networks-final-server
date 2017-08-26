/*
 * Stas Mushkin 309284958
 * Noa Fialkov 204498133
 */

#include "TCPMessengerServer.h"

TCPMessengerServer::TCPMessengerServer(){
	this->sem = new Semaphore(1);
	this->serverSocket = new TCPSocket(MSNGR_PORT);
	this->usersManager = new UsersManager();
	this->isRunning = false;
	this->dispatcher = NULL;
	this->start();
}

void TCPMessengerServer::close(){
	this->isRunning = false;

	if(this->dispatcher != NULL){
		this->dispatcher->close();
		delete this->dispatcher;
		this->dispatcher = NULL;
	}

	if(this->usersManager != NULL){
		this->usersManager->close();
		delete this->usersManager;
		this->usersManager = NULL;
	}

	// close server
	this->serverSocket->cclose();
	this->waitForThread();
	delete this->serverSocket;
	this->serverSocket = NULL;
}

void TCPMessengerServer::run(){
	this->dispatcher = new TCPMsnDispatcher(this->usersManager);
	this->isRunning  = true;
	while(this->isRunning){
		TCPSocket* newPeer = this->serverSocket->listenAndAccept();

		if(newPeer == NULL) continue;

		cout<<"new peer connected - "<<newPeer->fromAddr()<<endl;
		this->usersManager->addPeer(newPeer);
	}
	if(this->dispatcher != NULL){
		this->dispatcher->close();
		delete this->dispatcher;
		this->dispatcher = NULL;
	}
}


void TCPMessengerServer::printAllUsers(){
	cout << "All Users in messenger: " << endl;
	vector<string> users = this->usersManager->getAllUsers();
	sem->lock();
	vector<string>::iterator it = users.begin();
	for(;it != users.end(); ++it){
		cout << *it << endl;
	}
	sem->unlock();
}
void TCPMessengerServer::printConnectedUsers(){
	cout << "Connected Users now: " << endl;
	vector<User*>* users = this->usersManager->getLoggedInUsers();
	sem->lock();
	vector<User*>::iterator it = users->begin();
	for(;it != users->end(); ++it){
		cout << (*it)->getUserName() << endl;
	}
	sem->unlock();
}
void TCPMessengerServer::printSessions(){
	cout << "All Sessions in messenger: " << endl;

	map<string, string> sessions = this->dispatcher->getSessions();
	sem->lock();
	map<string, string>::iterator it = sessions.begin();
	for(;it != sessions.end(); ++it){
		cout<< (*it).first << " --> " << (*it).second << endl;
	}
	sem->unlock();
}


TCPMessengerServer::~TCPMessengerServer(){
	delete this->sem;
	this->sem = NULL;
}
