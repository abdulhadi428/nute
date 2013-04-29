#ifndef _NAMESPACE_PROTECT_
	#error "Use ArmletShell.h"
#endif

typedef enum _MSG_ID
{
	MSG_SHOW_MESSAGE = 1,
	MSG_UPDATE_LOCK_LIST = 2,
	MSG_SET_PLAYER_NAME = 3,
	MSG_SET_PLAYER_REGENERATION = 4,
	MSG_ROOM_HIT = 8,
	MSG_GET_PLAYER_INFO	 = 12,
	MSG_TASK_COMPLETED = 64,
	MSG_TASK_COMPLETION_CONFIRMED = 65,
} MSG_ID;

#pragma pack(push,1)
typedef struct _SRV_HEADER {
	ubyte_t MsgId;				//MSG
	uword_t noncheId;
} SRV_HEADER, *PSRV_HEADER;

typedef struct _SRV_SHOW_MESSAGE {
	SRV_HEADER header;
	char Message[140];
} SRV_SHOW_MESSAGE, *PSRV_SHOW_MESSAGE;

typedef struct _SRV_GET_PLAYER_INFO {
	SRV_HEADER header;
	ubyte_t BloodCapacity;
	ubyte_t ToxinCapacity;
	//MORE
} SRV_GET_PLAYER_INFO, *PSRV_GET_PLAYER_INFO;

#pragma pack(pop)

/*
        public void SetName(string text)
        {
            Name = text;
            _model.SavePersistent();
            SendCommand(MessageId.MSG_SET_PLAYER_NAME, text);
        }

        private void SendCommand(MessageId messageId, string text)
        {
            if (text.Length > 140)
            {
                throw new ArgumentOutOfRangeException("text");
            }
            SendCommand(messageId, Encoding.GetEncoding(1251).GetBytes(text));
        }

        private void SendCommand(MessageId messageId, params byte[] data)
        {
            _model.SendPayload(Id, new[] {(byte) messageId}.Concat(data).ToArray());
        }

        public void SendMessage(string text)
        {
            SendCommand(MessageId.MSG_SHOW_MESSAGE, text);
        }

        public void SetRegeneration(byte selectedValue)
        {
            SendCommand(MessageId.MSG_SET_PLAYER_REGENERATION, selectedValue);
        }

        public void SetLockList(byte[] lockList)
        {
            SendCommand(MessageId.MSG_UPDATE_LOCK_LIST, lockList);
        }

        public byte Id { get; private set; }
        public byte Room { get; private set; }
        public byte BloodLevel { get; private set; }
        public string Name { get; private set; }

        public void Update(NetworkLevel.WCFServices.PlayerUpdate playerStatusUpdate)
        {
            BloodLevel = playerStatusUpdate.NewBlood;
            Room = playerStatusUpdate.NewRoom;
        }
*/

