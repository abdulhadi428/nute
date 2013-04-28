using System;

namespace NetworkLevel.NetworkDeliveryLevel
{

    public class PlayerStatusUpdate
    {
        public byte armlet_id { get; set; }
        public byte new_room { get; set; }
        public byte new_blood { get; set; }
    }

    public interface IArmletDeliveryServece
    {
        void DeliverToSingleArmlet(byte armlet_id, short nonce_id, byte[] payload);   //For any MSG_* commands
        void DeliverToAllArmlets(byte[] nonce_id, byte[] payload);                    //For MSG_ROOM_HIT usage only

        event Action<byte, byte[]> TXCompleted;                             // byte armlet_id, byte[] nonce_id
        event Action<PlayerStatusUpdate[]> ArmletsStatusUpdate;             // Array of player/armlet status changes
        event Action<byte, byte[]> ArmletSendsData;                         // byte armlet_id, byte[] payload

    }
}
