using System;
using System.Collections.Generic;

namespace HonorInterfaces
{
    public interface IShip : IDisposable
    {
        int SubsystemsCount { get; }
        bool SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        List<ShipSubsystemStatus> GetAllSubsystemsStatus();

        List<byte> ShipRoomsIDs { get; }

        Guid ShipGuid { get; set; }
        byte[] PhysicalGateID { get; set; }
        string Name { get; set; }
        IGlobalModel Model { get; set; }

        string GetRoomName(byte roomsID);

        event Action<ShipSubsystemStatus> SubsystemUpdated;
        void HitRandomRoom(RanmaRepairSeverity severity);

        bool IsOnline { get; }
        event Action OnlineChanged;
    }
}
