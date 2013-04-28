    internal enum MessageId
    {
        // ReSharper disable InconsistentNaming
        MSG_SHOW_MESSAGE = 1,
        MSG_UPDATE_LOCK_LIST = 2,
        MSG_SET_PLAYER_NAME = 3,
        MSG_SET_PLAYER_REGENERATION = 4,
        // ReSharper restore InconsistentNaming
    }

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
        
        public Armlet(byte armletId, GlobalModel model, string name)
        {
            Name = name ?? ("Ð'Ñ_Ð°Ñ_Ð>Ð÷Ñ' " + armletId);
            _model = model;
            Id = armletId;
        }

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
    }
}
