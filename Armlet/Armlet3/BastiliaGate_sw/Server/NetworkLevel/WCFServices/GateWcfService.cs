using System;
using System.ServiceModel;
using NetworkLevel.NetworkDeliveryLevel;

namespace NetworkLevel.WCFServices
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "IwcfService" in both code and config file together.
    
    

    [ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
    public class GateWcfService : IGateWCFService
    {
        private bool _gateIsConnectedWithId = false;
        private byte _sessionGateId;
        private GateDeliveryService _gateDeliveryService;
        private ArmletDeliveryService _armletDeliveryService;
        
        public GateWcfService()
        {
            _gateDeliveryService = (GateDeliveryService) NetworkDelivery.GateDeliveryInstance;
            _armletDeliveryService = (ArmletDeliveryService) NetworkDelivery.ArmletDeliveryInstance;


            
            OperationContext.Current.Channel.Closed += (sender, args) =>
                {
                    GateConnectionPool.GateConnections.Remove(_sessionGateId);
                    _gateDeliveryService.onGateDisconnected(_sessionGateId);
                    _gateIsConnectedWithId = false;
                };

            OperationContext.Current.Channel.Faulted += (sender, args) =>
                {
                    GateConnectionPool.GateConnections.Remove(_sessionGateId);
                    _gateDeliveryService.onGateDisconnected(_sessionGateId);
                    _gateIsConnectedWithId = false;
                };
        }

        public byte RegisterGate(byte preferedGateId)
        {
            if ( _gateIsConnectedWithId )  // Duplicate RegisterGate call, an Id is allredy given - just return old Id value
            {
                return _sessionGateId;
            }

            byte newIdForGate = preferedGateId;
            if (GateConnectionPool.GateConnections.ContainsKey(newIdForGate))
            {
                newIdForGate = 1;
                while (GateConnectionPool.GateConnections.ContainsKey(newIdForGate))
                {
                    newIdForGate++;
                }
            }

            _sessionGateId = newIdForGate;
            GateConnectionPool.GateConnections.Add(newIdForGate, OperationContext.Current.GetCallbackChannel<IGateWCFServiceCallback>());
            _gateIsConnectedWithId = true;
            _gateDeliveryService.onGateConnection(newIdForGate);
            return newIdForGate;
        }

        public void TXCompleted(byte[] payload)
        {
            _armletDeliveryService.OnTxCompleted(_sessionGateId, payload);
        }

        public void ArmlteStatusUpdate(PlayerUpdate[] updates)
        {
            _armletDeliveryService.OnArmletStatusUpdate(updates);
        }

        public void ArmletSendsData(byte armlet_id, byte[] payload)
        {
            _armletDeliveryService.OnArmletSendsData(armlet_id, payload);
        }

        public void PillConnectionStatus(byte[] isConnectionResultData)
        {
            _gateDeliveryService.onPillConnectedStatus(_sessionGateId, isConnectionResultData);
        }

        public void PillWriteCompleted(byte[] writingResultData)
        {
            _gateDeliveryService.onPillWriteCompleted(_sessionGateId, writingResultData);
        }

        public void PillDataRead(byte[] readingResultData)
        {
            _gateDeliveryService.onPillDataRead(_sessionGateId, readingResultData);
        }

        public void PinSet(byte[] pinSetResultData)
        {
            _gateDeliveryService.onPinSet(_sessionGateId, pinSetResultData);
        }


        
    }
}
