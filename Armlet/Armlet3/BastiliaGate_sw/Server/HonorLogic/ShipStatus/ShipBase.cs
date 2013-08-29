using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic.ShipStatus
{
    public abstract class ShipBase : IShip
    {
        public abstract int BoardCount { get; }
        public abstract bool SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        public abstract List<ShipSubsystemStatus> GetAllSubsystemsStatus();
        public  List<byte> ShipRoomsIDs { get; set; }
        public  Guid ShipGuid { get; set; }
        public  int[] PhysicalGateID { get; set; }
        public  string Name { get; set; }
        public IGlobalModel Model { get; set; }
        public string GetRoomName(byte roomsID)
        {
            return "Отсек " + roomsID;
        }

        public event Action<ShipSubsystemStatus> SubsystemUpdated;
    }
}
