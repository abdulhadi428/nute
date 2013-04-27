using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HonorGateServer.NetworkDeliveryLevel
{
    class ArmletDeliveryService : IArmletDeliveryServece
    {
        private GateWcfService _service;
        public ArmletDeliveryService(GateWcfService service)
        {
            _service = service;



        }
        public void DeliverToSingleArmlet(int armlet_ID, int nonce_id, string payload)
        {
            throw new NotImplementedException();
        }

        public event ArmletPayloadTransferedHandler PayloadTransferedToArmlet;
        public event ArmletPayloadTaskCompletedHandler PayloadTaskCompletedByArmlet;

        public void DeliverToAllArmlets(int nonce_id, string payload)
        {
            throw new NotImplementedException();
        }

        public event ArmletsPayloadTranferedHandler SomeArmletsConfirmedDelivery;
    }
}
