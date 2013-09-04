using HonorLogic.ShipStatus;
using System;
using System.Collections.Generic;
using System.Linq;
using HonorLogic.Storage;

namespace HonorLogic
{
    class ShipList : ObjectListBase<Guid, ShipBase>
    {
        internal List<ShipStoredData> GetDataToStore()
        {
            return GetAll().Select(a => new ShipStoredData 
            { 
                Id = a.ShipGuid,
                Gates = a.PhysicalGateID.ToList(),
                Name = a.ShipName,
               Subsystems = a.GetAllSubsystemsStatus() ,
               Rooms = a.Rooms
            }).ToList();
        }

        public ShipBase GetShipByGuid(Guid id)
        {
            return GetAll().Single(s => s.ShipGuid == id);
        }
    }
}
