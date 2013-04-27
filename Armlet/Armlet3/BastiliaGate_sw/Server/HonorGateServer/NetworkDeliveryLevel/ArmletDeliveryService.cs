using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HonorGateServer.WCFServices;

namespace HonorGateServer.NetworkDeliveryLevel
{
    class ArmletDeliveryService : IArmletDeliveryServece
    {
        private GateWcfService _service;
        public ArmletDeliveryService(GateWcfService service)
        {
            _service = service;
        }


        public void DeliverToSingleArmlet(byte armlet_id, byte[] nonce_id, byte[] payload)
        {
            throw new NotImplementedException();
        }

        public void DeliverToAllArmlets(byte[] nonce_id, byte[] payload)
        {
            throw new NotImplementedException();
        }

        public event Action<byte, byte[]> TXCompleted;
        public event Action<PlayerStatusUpdate[]> ArmletsStatusUpdate;
        public event Action<byte, byte[]> ArmletSendsData;
    }
}
