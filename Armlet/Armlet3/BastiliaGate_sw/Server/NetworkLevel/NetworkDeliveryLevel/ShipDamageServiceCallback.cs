using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using NetworkLevel.ShipDamageService;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public class ShipDamageServiceCallback: ShipDamageService.IServerDamageContractCallback
    {
        private ShipDamageServiceObj _serviceObj;
        public ShipDamageServiceCallback(ShipDamageServiceObj serviceObj)
        {
            _serviceObj = serviceObj;
        }
        public bool DamageShip(Guid shipGuid, byte byteSeverity)
        {
            _serviceObj.FireShipDamaged(shipGuid, byteSeverity);
            return true;
        }

        public bool DestroyShip(Guid shipGuid)
        {
            _serviceObj.FireShipDestroyed(shipGuid);
            return true;
        }

       
    }
}
