using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
    interface IShip
    {
        ShipType GetShipType();
        bool SetSubsystemStatus(int subsystemNum, RanmaSubsystemStatus ranmaStatus);
        List<RanmaSubsystemStatus> GetAllSubsystemsStatus();

        int PhysicalGateID { get; set; }
    }
}
