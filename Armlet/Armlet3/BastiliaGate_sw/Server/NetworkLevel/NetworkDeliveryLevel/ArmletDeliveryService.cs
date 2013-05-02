using System;
using System.Threading.Tasks;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class ArmletDeliveryService : IArmletDeliveryServece
    {
        

        
        public Task<ArmletDeliveryStatus> DeliverToSingleArmlet(byte armlet_id, byte[] payload)
        {
            var task = new Task<ArmletDeliveryStatus>(() =>
                {
                    foreach (var gateWcfServiceCallback in GateConnectionPool.GateConnections)
                    {
                        gateWcfServiceCallback.Value.SendToArmlet(armlet_id, payload);
                    }
                    return ArmletDeliveryStatus.SuccessDeliver;
                });
            task.Start();
            return task;

        }

        public void DeliverToAllArmlets(byte broadcast_id, byte[] payload)
        {
            foreach (var gateWcfServiceCallback in GateConnectionPool.GateConnections)
            {
                for (byte armlet_id = 10; armlet_id < 109; armlet_id++)
                {
                    gateWcfServiceCallback.Value.SendToArmlet(armlet_id, payload);
                }
            }
        }

        public event Action<byte, byte> BroadcastTXCompleted;
        public event Action<WCFServices.PlayerUpdate[]> ArmletsStatusUpdate;
        public event Action<byte, byte[]> ArmletSendsData;

        internal void onTxCompleted(byte gate_id, byte[] armlet_id_and_result)
        {
            //Хитрая логика обработки Комлитов
            //Да нахер комплиты? Кому они нужны, я вас спрашиваю?
        }
        internal void onArmletStatusUpdate(PlayerUpdate[] updates)
        {
            var ArmletsStatusUpdateHandler = ArmletsStatusUpdate;
            if (ArmletsStatusUpdateHandler != null) ArmletsStatusUpdateHandler(updates);

        }
        internal void onArmletSendsData(byte armlet_id, byte[] payload)
        {
            var ArmletSendsDataHandler = ArmletSendsData;
            if (ArmletSendsDataHandler != null) ArmletSendsDataHandler(armlet_id, payload);
        }

    }
}
