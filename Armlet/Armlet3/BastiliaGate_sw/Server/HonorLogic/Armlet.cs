using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace HonorLogic
{
    public class Armlet : IArmletInfo
    {
        public Armlet(byte armletId, IArmletDeliveryServece service)
        {
            throw new NotImplementedException();
        }

        public void SetName(string text)
        {
            throw new NotImplementedException();
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
