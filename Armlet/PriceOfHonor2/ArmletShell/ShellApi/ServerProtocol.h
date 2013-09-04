#pragma once

#define CUSTOM_MESSAGE_LEN	140

namespace ServerProtocol {

typedef enum _MSG_ID
{
	MSG_SHOW_MESSAGE = 1,
	MSG_UPDATE_LOCK_LIST = 2,
	MSG_SET_PLAYER_NAME = 3,
	MSG_SET_PLAYER_REGENERATION = 4,
	MSG_ROOM_HIT = 8,
	MSG_TIME = 9,
	MSG_TASK_COMPLETED = 64,
	MSG_TASK_COMPLETION_CONFIRMED = 65,
} MSG_ID;

typedef unsigned char msg_t;
typedef unsigned short server_nonche_t;
typedef unsigned char armlet_nonche_t;
typedef unsigned char lock_t;
typedef unsigned char room_t;
typedef unsigned char capacity_t;
typedef unsigned char regeneration_t;

#pragma pack(push,1)
typedef struct _SRV_TO_ARMLET_HEADER {
	msg_t				msg_type;	//from MSG_ID
	server_nonche_t		srv_msg_num;
} SRV_TO_ARMLET_HEADER;

typedef struct _SRV_FROM_ARMLET_HEADER {
	msg_t msg_type;					//MSG_ID
	armlet_nonche_t		arm_msg_num;
} SRV_FROM_ARMLET_HEADER;

//MSG_SHOW_MESSAGE
//MSG_SET_PLAYER_NAME
typedef struct _SRV_STRING {
	SRV_TO_ARMLET_HEADER header;
	char string[1];					//actual number varies
} SRV_STRING;

//MSG_UPDATE_LOCK_LIST
typedef struct _SRV_UPDATE_LOCK_LIST {
	SRV_TO_ARMLET_HEADER header;
	lock_t available_locks[1];		//actual number varies
} SRV_UPDATE_LOCK_LIST;

//MSG_SET_PLAYER_REGENERATION
typedef struct _SRV_SET_PLAYER_REGENERATION {
	SRV_TO_ARMLET_HEADER header;
	regeneration_t level;			//{1,2,3}
} SRV_SET_PLAYER_REGENERATION;

//MSG_ROOM_HIT
typedef struct _SRV_ROOM_HIT {
	SRV_TO_ARMLET_HEADER header;
	room_t room_id;
	unsigned char probability;		//0..100
	unsigned char explosion_type;	//0..3
} SRV_ROOM_HIT;

//MSG_TIME
typedef struct _SRV_TIME {
	SRV_TO_ARMLET_HEADER header;
	unsigned char hours;
	unsigned char minutes;
} SRV_TIME;

//MSG_TASK_COMPLETION_CONFIRMED
typedef struct _SRV_TASK_COMPLETION_CONFIRMED {
	SRV_TO_ARMLET_HEADER header;
	armlet_nonche_t arm_msg_num;
} SRV_TASK_COMPLETION_CONFIRMED;

//MSG_TASK_COMPLETED
typedef struct _SRV_TASK_COMPLETED {
	SRV_FROM_ARMLET_HEADER header;
	server_nonche_t srv_msg_num;
	unsigned char bResult;
} SRV_TASK_COMPLETED;

#pragma pack(pop)

} //namespace

using namespace ServerProtocol;