using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class LakShip: ShipBase
    {
        public override int BoardCount
        {
            get
            {
                return 4;
            }
        }

        public override bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            throw new NotImplementedException();
        }

        public override List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            throw new NotImplementedException();
        }
    }
}
