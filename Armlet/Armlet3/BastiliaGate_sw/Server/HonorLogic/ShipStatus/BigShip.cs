using System;
using System.Collections.Generic;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class BigShip: IShip
    {
        public int BoardCount
        {
            get
            {
                return 8;
            }
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
        public IGlobalModel Model { get; set; }
        public List<int> ShipRoomsIDs { get; set; }
    }
}
