using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorLogic.ShipStatus
{
    public class BigShip: IShip
    {
        private ShipType _shipType = ShipType.BigShip;
        public ShipType GetShipType()
        {
            return _shipType;
        }

        public bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            throw new NotImplementedException();
        }

        public List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            return new List<ShipSubsystemStatus>();
            throw new NotImplementedException();
        }

        public Guid ShipGuid { get; set; }

        public int[] PhysicalGateID { get; set; }

       public string Name { get; set; }
        public GlobalModel Model { get; set; }
    }
}
