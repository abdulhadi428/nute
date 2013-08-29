using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public class LakShip: IShip
    {
        public int BoardCount
        {
            get
            {
                return 4;
            }
        }

        public bool SetSubsystemStatus(ShipSubsystemStatus ranmaStatus)
        {
            throw new NotImplementedException();
        }

        public List<ShipSubsystemStatus> GetAllSubsystemsStatus()
        {
            throw new NotImplementedException();
        }

        public Guid ShipGuid { get; set; }

        public int[] PhysicalGateID { get; set; }

        public string Name { get; set; }
        public IGlobalModel Model { get; set; }
    }
}
