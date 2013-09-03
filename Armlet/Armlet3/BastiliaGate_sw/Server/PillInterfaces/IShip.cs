using System;
using System.Collections.Generic;

namespace HonorInterfaces
{
    public interface IShip : IDisposable
    {
        void SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        List<ShipSubsystemStatus> GetAllSubsystemsStatus();

        IEnumerable<byte> ShipRoomsIDs { get; }

        string Name { get; }
        IGlobalModel Model { get; }

        string GetRoomName(byte roomsID);

        event Action<ShipSubsystemStatus> SubsystemUpdated;

        bool IsOnline { get; }
        event Action OnlineChanged;
        void DamageShip(byte damage);
        void RemoveShipFromMap();
    }
}
