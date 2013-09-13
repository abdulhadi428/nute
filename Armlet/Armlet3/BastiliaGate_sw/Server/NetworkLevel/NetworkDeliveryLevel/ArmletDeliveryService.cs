using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceModel;
using System.Threading;
using HonorInterfaces;
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
            var gatesToPutOffline = new List<byte>();
            foreach (var callback in GateConnectionPool.GateConnections.ToArray())
            {
                try
                {
                    callback.Value.SendToArmlet(armletId, payload);
                }
                catch (CommunicationException )
                {
                    gatesToPutOffline.Add(callback.Key);
                    //throw;
                }
            }
            if (gatesToPutOffline.Any())
            {
                throw new GateNotConnectedException(gatesToPutOffline.ToArray());
            }
        }


        public void DeliverToArmlets(byte[] armletIds, byte[] payload)
        {
            var gatesToPutOffline = new List<byte>();
            foreach (var callback in GateConnectionPool.GateConnections.ToArray())
            {
                try
                {
                    foreach (var armletId in armletIds)
                    {
                        callback.Value.SendToArmlet(armletId, payload);
                        Thread.Sleep(20);
                    }
                }
                catch (CommunicationException)
                {
                    gatesToPutOffline.Add(callback.Key);
                    //throw;
                }
            }
            if (gatesToPutOffline.Any())
            {
                throw new GateNotConnectedException(gatesToPutOffline.ToArray());
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
