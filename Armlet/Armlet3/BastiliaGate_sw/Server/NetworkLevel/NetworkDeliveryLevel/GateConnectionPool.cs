using System.Collections.Generic;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    public class GateInstance
    {
        public IGateWCFServiceCallback CallbackObject { get; set; }
        public int GateId { get; set; }

    }

    public static class  GateConnectionPool
    {
        public static Dictionary<int, GateInstance> GateConnections = new Dictionary<int, GateInstance>();

    }
}
