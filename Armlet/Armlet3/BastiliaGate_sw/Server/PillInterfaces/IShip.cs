using System;
using System.Collections.Generic;

namespace HonorInterfaces
{
    public interface IShip
    {
        int BoardCount { get; }
        bool SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        List<ShipSubsystemStatus> GetAllSubsystemsStatus();

        Guid ShipGuid { get; set; }
        int[] PhysicalGateID { get; set; }
        string Name { get; set; }
        IGlobalModel Model { get; set; }
    }
}
