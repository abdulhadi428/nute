using System;
using System.Threading.Tasks;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class ArmletDeliveryService : IArmletDeliveryServece
    {
        

        
        public Task<ArmletDeliveryStatus> DeliverToSingleArmlet(byte armlet_id, byte[] payload)
        {
            //Хитрая логика отправки всем
            throw new NotImplementedException();
        }

        public void DeliverToAllArmlets(byte broadcast_id, byte[] payload)
        {
            //Хитрая логика отправки всем
            throw new NotImplementedException();
        }

        public event Action<byte, byte> BroadcastTXCompleted;
        public event Action<WCFServices.PlayerUpdate[]> ArmletsStatusUpdate;
        public event Action<byte, byte[]> ArmletSendsData;

        internal void onTxCompleted(byte gate_id, byte armlet_id)
        {
            //Хитрая логика обработки Комлитов
            throw new NotImplementedException();
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
