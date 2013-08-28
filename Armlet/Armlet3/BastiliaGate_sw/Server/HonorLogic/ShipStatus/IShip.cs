using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
    interface IShip
    {
        ShipType GetShipType();
        bool SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        List<ShipSubsystemStatus> GetAllSubsystemsStatus();

        Guid ShipGuid { get; set; }
        int[] PhysicalGateID { get; set; }
    }
}
