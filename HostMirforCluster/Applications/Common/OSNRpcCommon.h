
#ifndef	_OSNRPCCOMMON_H_
#define _OSNRPCCOMMON_H_


#define	OSN_MAX_USERNAME		32			//Maximum length of the username, limited by windows
#define	OSN_MAX_PASSWORD		32			//Maximum length of the password, limited by windows


//OSN command ID
#define	OSNRPC_CMD_PING				0x0001
#define OSNRPC_CMD_RESET			0x0002
#define OSNRPC_CMD_SHUTDOWN			0x0003
#define OSNRPC_CMD_DELETE_BACKUP	0x0004
#define OSNRPC_CMD_CREATE_BACKUP	0x0005
#define OSNRPC_CMD_GET_TARGET_STATE	0x0006


// OSN Command Status
#define CMD_STATUS_SUCCESS			0x0000
#define	CMD_STATUS_INVALID_HOST		0x0001
#define	CMD_STATUS_FAILED			0x0002
#define	CMD_STATUS_SEND_FAILED		0x0003
#define	CMD_STATUS_INIT				0x0010
#define CMD_STATUS_SNAPFIND         0x0011




//OSN command flag, can not be ORed.
#define OSNRPC_CMD_ASK				0x01
#define OSNRPC_CMD_RESPONSE			0x02



typedef struct _MESSAGE_HEADER
{
	unsigned short	retStatus;						//return Status indicating result of the command execution
	char			flags;							//ASK/RESPONSE
	char			command;						//PING, CREATE_BACKUP, etc.
	unsigned short	paraNum;						//number of parameters for this command
	unsigned short	dataLength;						//parameter data buffer length
	unsigned int	timeStamp;						//The time (h*60*60+m*60+s[sec]) the message being generated
	char			initiatorName[OSN_MAX_USERNAME];
	char			initiatorWWN[OSN_MAX_PASSWORD];
} MESSAGE_HEADER, *PMESSAGE_HEADER;




//define constants
#define OSNRPC_SOCKET_CLIENT_PORT	59185		//default OSN RPC Client port
#define	OSNRPC_SOCKET_SERVICE_PORT	59186		//Default OSN Rpc service port
#define	OSNRPC_MAX_MSG_LEN			512			//Maximum message length sent by OSN Rpc service 
#define OSNRPC_MAX_MSG				2			//Maximum outstanding messages sent by the client
#define	OSNRPC_MSGHEAD_LEN			sizeof(MESSAGE_HEADER)
#define	OSNRPC_MAX_MSGPARA_LEN		OSNRPC_MAX_MSG_LEN - OSNRPC_MSGHEAD_LEN




#endif //_OSNRPCCOMMON_H_