//============================================================================
// Name        : server.cpp
// Author      : aa
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "TCPMessengerServer.h"
using namespace std;

void printMessngerInstructions(){

	cout<<"---------------------CLI:---------------------"<<endl;
	cout<<"list_users - list all users" << endl;
	cout<<"list_connected_users - list all connected users"<<endl;
	cout<<"list_sessions - list all sessions (two clients communicating)"<<endl;
	cout<<"exit - shutdownaaaaaaaaaaaaaaaaaaa"<<endl;
	cout<<"----------------------------------------------"<<endl;
	cout<<"----------------------------------------------"<<endl;
}


int main(){
	cout<<"----------Welcome To Messenger Server---------"<<endl;
	printMessngerInstructions();
	TCPMessengerServer msngrServer;
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "list_users"){
			msngrServer.printAllUsers();
		}else if(command == "list_connected_users"){
			msngrServer.printConnectedUsers();
		}else if(command == "list_sessions"){
			msngrServer.printSessions();
		}else if(command == "exit"){
			break;
		}else{
			cout<<"wrong input, please use the cli guide"<<endl;
			printMessngerInstructions();
		}
	}
	msngrServer.close();
	cout<<"messenger was closed!. bye bye"<<endl;
	return 0;
}
