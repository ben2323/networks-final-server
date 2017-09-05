#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346
#define MSNGR_CLIENT_PORT 9999

#define CLOSE_SESSION_WITH_USER 	1
#define OPEN_SESSION_WITH_USER 		2
#define EXIT						3
#define SESSION_REFUSED				4
#define SESSION_ESTABLISHED			5
#define GET_USERS_LIST				6
#define GET_CONNECTED_USERS			7
#define LOGIN						10
#define LOGIN_FAILED				11
#define REGISTER					12
#define REGISTER_FAILED				13
#define UPDATE_SCORE				21
#define GET_SCORE					22



#endif /* TCPMESSENGERPROTOCOL_H_ */
