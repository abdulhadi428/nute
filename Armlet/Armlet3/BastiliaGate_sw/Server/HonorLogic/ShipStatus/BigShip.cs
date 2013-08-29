using System;
using System.Collections.Generic;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class BigShip: ShipBase
    {
        public override int BoardCount
        {
            get
            {
                return 8;
            }
        }

        public override bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            throw new NotImplementedException();
        }

        public override List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return new List<ShipSubsystemStatus>();
            throw new NotImplementedException();
        }
    }
}
