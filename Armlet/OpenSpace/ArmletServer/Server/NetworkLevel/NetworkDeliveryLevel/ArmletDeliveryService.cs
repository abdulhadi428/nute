using System;
using System.Linq;
using System.Threading;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class ArmletDeliveryService : IArmletDeliveryServece
    {
        internal void OnTxCompleted(byte gateId, byte[] payload)
        {
            var armletId = payload[0];
            var result = payload[1] == 0;
            if (result)
            {
                OnArmletSuccess(armletId);
            }
        }

        private void OnArmletSuccess(byte armletId)
        {
            var handler = ArmletSuccess;
            if (handler != null)
            {
                ArmletSuccess(armletId);
            }
        }

        public void DeliverToSingleArmlet(byte armletId, byte[] payload)
        {
            foreach (var callback in GateConnectionPool.GateConnections.Values.ToArray())
            {
                callback.SendToArmlet(armletId, payload);
            }
        }


        public void DeliverToArmlets(byte[] armletIds, byte[] payload)
        {
            foreach (var callback in GateConnectionPool.GateConnections.Values.ToArray())
            {
                foreach (var armletId in armletIds)
                {
                    callback.SendToArmlet(armletId, payload);
                    Thread.Sleep(20);
                }
            }

        }

        public event Action<PlayerUpdate[]> ArmletsStatusUpdate;
        public event Action<byte, byte[]> ArmletSendsData;

        public event Action<byte> ArmletSuccess;

       
        internal void OnArmletStatusUpdate(PlayerUpdate[] updates)
        {
            var armletsStatusUpdateHandler = ArmletsStatusUpdate;
            if (armletsStatusUpdateHandler != null) armletsStatusUpdateHandler(updates);

        }
        internal void OnArmletSendsData(byte armletId, byte[] payload)
        {
            var armletSendsDataHandler = ArmletSendsData;
            if (armletSendsDataHandler != null) armletSendsDataHandler(armletId, payload);
        }

    }
}
