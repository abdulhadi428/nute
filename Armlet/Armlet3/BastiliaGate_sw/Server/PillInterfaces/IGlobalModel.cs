using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PillInterfaces
{
    public interface IGlobalModel
    {
        IEnumerable<IArmletInfo> GetArmlets();
        event Action ArmletListUpdated;
        event Action<byte> NewGateOnline;
        IGateModel GetGateModel(byte gateId);
        void SendMessageToAll(string text);
        void SendSetName(string text, IArmletInfo armletId);
        void SendShowMessage(IArmletInfo armlet, string text);
        void SetSetPlayerRegen(IArmletInfo armlet, byte regen);
        void SendSetLockList(IArmletInfo armlet, byte[] lockList);
        void SendRoomHit(byte roomId, byte hitChance);
        void UpdateAllNames();
    }
}
