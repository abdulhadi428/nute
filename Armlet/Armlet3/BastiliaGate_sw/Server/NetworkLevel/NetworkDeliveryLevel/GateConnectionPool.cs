using System.Collections.Generic;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    

    public static class  GateConnectionPool
    {
        public static Dictionary<byte, IGateWCFServiceCallback> GateConnections = new Dictionary<byte, IGateWCFServiceCallback>();

    }
}
