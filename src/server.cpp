
#include <iostream>
#include "TCPMessengerServer.h"
using namespace std;

void printMessngerguide(){

	cout<<"*************Command********Line*************"<<endl;
	cout<<"users - list of users" << endl;
	cout<<"connected_users - list of connected users"<<endl;
	cout<<"sessions - list of sessions"<<endl;
	cout<<"quit"<<endl;
	cout<<"*********************************************"<<endl;

}


int main(){
	cout<<"*********Welcome*To*Messenger*Server*********"<<endl;
	printMessngerguide();
	TCPMessengerServer messengerServer;
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "users"){
			messengerServer.printUsersList();
		}else if(command == "connected_users"){
			messengerServer.printConnectedUsers();
		}else if(command == "sessions"){
			messengerServer.printSessions();
		}else if(command == "quit"){
			break;
		}else{
			cout<<"error input, please use the guide"<<endl;
			printMessngerguide();
		}
	}
	messengerServer.close();
	cout<<"messenger closed. thank you and bye"<<endl;
	return 0;
}
