using System;
using System.Threading.Tasks;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public interface IArmletDeliveryServece
    {
        void DeliverToSingleArmlet(byte armletId, byte[] payload);   //For any MSG_* commands

        event Action<WCFServices.PlayerUpdate[]> ArmletsStatusUpdate;             // Array of player/armlet status changes
        event Action<byte, byte[]> ArmletSendsData;                         // byte armlet_id, byte[] payload

        void DeliverToArmlets(byte[] armletIds, byte[] payload);
        event Action<byte> ArmletSuccess;
    }
}
