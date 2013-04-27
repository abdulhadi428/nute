﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HonorGateServer.NetworkDeliveryLevel
{

    public class PlayerStatusUpdate
    {
        byte armlet_id { get; set; }
        byte new_room { get; set; }
        byte new_blood { get; set; }
    }

    public interface IArmletDeliveryServece
    {
        void DeliverToSingleArmlet(byte armlet_id, byte[] nonce_id, byte[] payload);   //For any MSG_* commands
        void DeliverToAllArmlets(byte[] nonce_id, byte[] payload);                    //For MSG_ROOM_HIT usage only

        event Action<byte, byte[]> TXCompleted;                             // byte armlet_id, byte[] nonce_id
        event Action<PlayerStatusUpdate[]> ArmletsStatusUpdate;             // Array of player/armlet status changes
        event Action<byte, byte[]> ArmletSendsData;                         // byte armlet_id, byte[] payload

    }
}