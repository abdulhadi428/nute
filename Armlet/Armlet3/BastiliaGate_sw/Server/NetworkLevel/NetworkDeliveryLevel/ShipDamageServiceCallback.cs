using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Text;
using System.Threading;
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
        public void DamageShip(Guid shipGuid, byte byteSeverity)
        {
            Thread damageThread = new Thread(() => _serviceObj.FireShipDamaged(shipGuid, byteSeverity));

            damageThread.Start();
            
            return;
        }

        public IAsyncResult BeginDamageShip(Guid shipGuid, byte byteSeverity, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndDamageShip(IAsyncResult result)
        {
            throw new NotImplementedException();
        }

        public void DestroyShip(Guid shipGuid)
        {
            _serviceObj.FireShipDestroyed(shipGuid);
            return;
        }

        public IAsyncResult BeginDestroyShip(Guid shipGuid, AsyncCallback callback, object asyncState)
        {
            throw new NotImplementedException();
        }

        public void EndDestroyShip(IAsyncResult result)
        {
            throw new NotImplementedException();
        }
    }
}
