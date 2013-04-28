using System;
using System.Linq;
using System.Text;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace HonorLogic
{
    class Armlet : IArmletInfo
    {
        private readonly GlobalModel _model;
        
        public Armlet(byte armletId, GlobalModel model)
        {
            _model = model;
            Id = armletId;
        }

        public void SetName(string text)
        {
            Name = text;
            _model.SavePersistent("Armlet", Id, Name);
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

        public void Update(PlayerStatusUpdate playerStatusUpdate)
        {
            BloodLevel = playerStatusUpdate.new_blood;
            Room = playerStatusUpdate.new_room;
        }
    }
}
