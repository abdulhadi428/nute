using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace HonorLogic
{
    class Armlet : IArmletInfo
    {
        private readonly GlobalModel _model;

        private const byte MESSAGE_SET_PLAYER_NAME = 3;

        public Armlet(byte armletId, GlobalModel model)
        {
            _model = model;
            Id = armletId;
        }

        public void SetName(string text)
        {
            var payload = new[] {MESSAGE_SET_PLAYER_NAME}.Concat(Encoding.GetEncoding(1251).GetBytes(text)).ToArray();
            _model.SendPayload(Id, payload);
        }

        public void SendMessage(string text)
        {
            throw new NotImplementedException();
        }

        public void SetRegeneration(byte selectedValue)
        {
            throw new NotImplementedException();
        }

        public int Id { get; private set; }
        public int Room { get; set; }
        public int BloodLevel { get; set; }
        public string Name { get; private set; }
    }
}
