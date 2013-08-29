using HonorLogic.ShipStatus;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HonorInterfaces;

namespace HonorLogic
{
    class ShipList : ObjectListBase<Guid, IShip>
    {
        internal List<ShipStoredData> GetDataToStore()
        {
            return GetAll().Select(a => new ShipStoredData 
            { 
                Id = a.ShipGuid,
                Gates = a.PhysicalGateID.ToList(),
                Name = a.Name,
               Subsystems = a.GetAllSubsystemsStatus() 
            }).ToList();
        }
    }
}
