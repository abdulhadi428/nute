using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HonorGateServer.NetworkDeliveryLevel
{
    public delegate void ArmletPayloadTransferedHandler(int nonce_id);
    public delegate void ArmletPayloadTaskCompletedHandler (int nonce_id);

    public delegate void ArmletsPayloadTranferedHandler(int[] armlets_ids, int nonce_id);

    interface IArmletDeliveryServece
    {
        void DeliverToSingleArmlet(int armlet_ID, int nonce_id, string payload);   //For any MSG_* commands
        event ArmletPayloadTransferedHandler PayloadTransferedToArmlet;
        event ArmletPayloadTaskCompletedHandler PayloadTaskCompletedByArmlet;

        void DeliverToAllArmlets(int nonce_id, string payload);                    //For MSG_ROOM_HIT usage only
        event ArmletsPayloadTranferedHandler SomeArmletsConfirmedDelivery;
    }
}
