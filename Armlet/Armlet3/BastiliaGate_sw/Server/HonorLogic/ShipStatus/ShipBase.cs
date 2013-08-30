using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;
using HonorLogic.Storage;
using HonorUtils;

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

        public virtual void InitializeRanmaPlate()
        {
        }

        public event Action<ShipSubsystemStatus> SubsystemUpdated;

        internal static string GetDefaultRoomName(byte roomsID)
        {
            return "Отсек " + roomsID;
        }

        internal string FindRoomName(byte roomsID)
        {
            return (from roomInfo in Rooms where roomInfo.Id == roomsID select roomInfo.Name).SingleOrDefault();
        }

        public void HitRandomRoom(RanmaRepairSeverity severity)
        {
            Model.SendRoomHit(RoomsWithPeople.RandomOrDefault(), GetHitChanceFromSeverity(severity));
        }

        private byte GetHitChanceFromSeverity(RanmaRepairSeverity severity)
        {
            return (byte) (((byte) severity - 2)*25 + 50);
        }

        private IEnumerable<byte> RoomsWithPeople
        {
            get
            {
                return Rooms.Select(r => r.Id).Intersect(Model.RoomsWithPeople);
            }
        }

        
    }
}
