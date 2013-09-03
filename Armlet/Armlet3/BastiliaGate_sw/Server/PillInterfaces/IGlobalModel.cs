using System;
using System.Collections.Generic;

namespace HonorInterfaces
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
        void SendRoomHit(byte roomId, byte hitChance, HitType hitType = HitType.Random);
        void UpdateAllNames();
        IEnumerable<IShip> GetShips();
        IEnumerable<byte> RoomsWithPeople { get; }
        bool IsShipGate(byte gateId);
        bool ConnectToSimulator();
    }
}
