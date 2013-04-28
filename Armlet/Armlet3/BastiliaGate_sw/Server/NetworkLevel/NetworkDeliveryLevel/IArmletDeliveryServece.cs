using System;
using System.Threading.Tasks;

namespace NetworkLevel.NetworkDeliveryLevel
{

    public class PlayerStatusUpdate
    {
        public byte armlet_id { get; set; }
        public byte new_room { get; set; }
        public byte new_blood { get; set; }
    }

    public enum ArmletDeliveryStatus
    {
        SuccessDeliver = 0,
        AllGatesRefused = 1,
        RetryLimitedReached = 2
    }

    public interface IArmletDeliveryServece
    {
        Task<ArmletDeliveryStatus> DeliverToSingleArmlet(byte armlet_id, byte[] payload);   //For any MSG_* commands
        void DeliverToAllArmlets(byte broadcast_id, byte[] payload);                    //For MSG_ROOM_HIT usage only

        event Action<byte, byte> BroadcastTXCompleted;                             // byte armlet_id, byte broadcast_id
        event Action<PlayerStatusUpdate[]> ArmletsStatusUpdate;             // Array of player/armlet status changes
        event Action<byte, byte[]> ArmletSendsData;                         // byte armlet_id, byte[] payload

    }
}
