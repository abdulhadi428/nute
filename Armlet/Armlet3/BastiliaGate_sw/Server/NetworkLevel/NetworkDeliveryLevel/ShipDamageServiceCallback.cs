using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NetworkLevel.ShipDamageService;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class ShipDamageServiceCallback: ShipDamageService.IServerDamageContractCallback
    {
        public bool DamageShip(Guid shipGuid, ShipSubsystemStatus subsystemStatus)
        {
            throw new NotImplementedException();
        }

        public bool DestroyShip(Guid shipGuid)
        {
            throw new NotImplementedException();
        }
    }
}
