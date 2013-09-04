using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NetworkLevel.ShipDamageService;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public class ShipDamageServiceCallback: ShipDamageService.IServerDamageContractCallback
    {
        internal static ShipDamageService.ServerDamageContractClient ShipDamageClient;

        public event Action<Guid, byte> ShipDamaged;
        public event Action<Guid> ShipDestroyed;

        public void SendToSimulator(Guid shipGuid, List<HonorInterfaces.ShipSubsystemStatus> subsystems)
        {
            var shipStatus = new ShipStatus()
                {
                    ShipGuid = shipGuid,
                };
            var subsystemStatuses = new ShipSubsystemStatus[subsystems.Count];

            foreach (var shipSubsystemStatuse in subsystems)
            {
                var status = new ShipSubsystemStatus();
                status.SubSystemNum = shipSubsystemStatuse.SubSystemNum;
                status.Severity = (NetworkLevel.ShipDamageService.RanmaRepairSeverity)shipSubsystemStatuse.ResultSeverity;
                subsystemStatuses[shipSubsystemStatuse.SubSystemNum] = status;
            }

            shipStatus.SubsystemStatuses = subsystemStatuses;
            ShipDamageClient.SetAllSubsystemsStatuses(new ShipStatus[] { shipStatus });
        }
        

        public bool DamageShip(Guid shipGuid, ShipSubsystemStatus subsystemStatus)
        {
            var handler = ShipDamaged;
            if (handler != null)
            {
                handler(shipGuid, 1);
            }
            return true;
        }

        public bool DestroyShip(Guid shipGuid)
        {
            var handler = ShipDestroyed;
            if (handler != null)
            {
                handler(shipGuid);
            }
            return true;
        }

       
    }
}
