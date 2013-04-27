using System.Collections.Generic;
using System.ServiceModel;
using HonorGateServer.NetworkDeliveryLevel;

namespace HonorGateServer.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "IwcfService" in both code and config file together.
    
    

    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
    public class GateWcfService : IGateWCFService
    {
        private int _sessionGateId = -1;
        public GateWcfService()
        {
            OperationContext.Current.Channel.Closed +=Channel_Closed;
            OperationContext.Current.Channel.Faulted +=Channel_Faulted;
        }

        public int RegisterGate(int preferedGateId)
        {
            if ( _sessionGateId > 0)  // Duplicate RegisterGate call, an Id is allredy given - just return old Id value
            {
                return _sessionGateId;
            }
            int newIdForGate = preferedGateId;
            if (GateConnectionPool.GateConnections.ContainsKey(newIdForGate))
            {
                newIdForGate = 1;
                while (GateConnectionPool.GateConnections.ContainsKey(newIdForGate))
                {
                    newIdForGate++;
                }
            }
            
            GateConnectionPool.GateConnections.Add(newIdForGate, new GateInstance()
                    {CallbackObject = OperationContext.Current.GetCallbackChannel<IGateWCFServiceCallback>(), GateId = newIdForGate}
                    );

            _sessionGateId = newIdForGate;
            return newIdForGate;
        }


        private void Channel_Closed(object sender, System.EventArgs e)
        {
            GateConnectionPool.GateConnections.Remove(_sessionGateId);
        }

        private void Channel_Faulted(object sender, System.EventArgs e)
        {
            GateConnectionPool.GateConnections.Remove(_sessionGateId);
        }


        
    }
}
