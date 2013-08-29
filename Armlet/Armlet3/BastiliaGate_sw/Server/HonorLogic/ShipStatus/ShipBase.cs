using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;
using HonorLogic.Storage;

namespace HonorLogic.ShipStatus
{
    public abstract class ShipBase : IShip
    {
        public List<RoomInfo> Rooms { get; set; }
        public abstract int BoardCount { get; }
        public abstract bool SetSubsystemStatus(ShipSubsystemStatus subsystemStatus);
        public abstract List<ShipSubsystemStatus> GetAllSubsystemsStatus();

        public List<byte> ShipRoomsIDs
        {
            get
            {
                return Rooms.Select(r => r.Id).ToList();
            } 
        }

        public  Guid ShipGuid { get; set; }
        public  int[] PhysicalGateID { get; set; }
        public  string Name { get; set; }
        public IGlobalModel Model { get; set; }
        public string GetRoomName(byte roomsID)
        {
            return FindRoomName(roomsID) ?? GetDefaultRoomName(roomsID);
        }

        internal static string GetDefaultRoomName(byte roomsID)
        {
            return "Отсек " + roomsID;
        }

        internal string FindRoomName(byte roomsID)
        {
            return (from roomInfo in Rooms where roomInfo.Id == roomsID select roomInfo.Name).SingleOrDefault();
        }

        public event Action<ShipSubsystemStatus> SubsystemUpdated;
    }
}
