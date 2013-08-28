using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
    public class LakShip: IShip
    {
        private ShipType _shipType = ShipType.LakShip;
        public ShipType GetShipType()
        {
            return _shipType;
        }

        public bool SetSubsystemStatus(int subsystemNum, RanmaSubsystemStatus ranmaStatus)
        {
            throw new NotImplementedException();
        }

        public List<RanmaSubsystemStatus> GetAllSubsystemsStatus()
        {
            throw new NotImplementedException();
        }

        public int PhysicalGateID { get; set; }
    }
}
