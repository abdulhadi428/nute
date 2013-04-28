using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using HonorGateServer.WCFServices;

namespace HonorGateServer.NetworkDeliveryLevel
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
