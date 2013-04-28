using System;
using NetworkLevel.WCFServices;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class GateDeliveryService:IGateDeliveryService
    {
        public void CheckIfPillConnected(byte gate_id, byte[] data)
        {
            IGateWCFServiceCallback callbackChannel;
            if (!GateConnectionPool.GateConnections.TryGetValue(gate_id, out callbackChannel))
            {
                throw new Exception("Gate with id " + gate_id + " is not connected");
            }
           callbackChannel.CheckIfPillConnected(data);
        }
        
        public void SendPillWhite(byte gate_id, byte[] data)
        {
            IGateWCFServiceCallback callbackChannel;
            if (!GateConnectionPool.GateConnections.TryGetValue(gate_id, out callbackChannel))
            {
                throw new Exception("Gate with id " + gate_id + " is not connected");
            }
            callbackChannel.SendPillWrite(data);
        }
        
        public void SendPillRead(byte gate_id, byte[] data)
        {
            IGateWCFServiceCallback callbackChannel;
            if (!GateConnectionPool.GateConnections.TryGetValue(gate_id, out callbackChannel))
            {
                throw new Exception("Gate with id " + gate_id + " is not connected");
            }
            callbackChannel.SendPillRead(data);
        }
        
        public void SendPinSignal(byte gate_id, byte[] data)
        {
            IGateWCFServiceCallback callbackChannel;
            if (!GateConnectionPool.GateConnections.TryGetValue(gate_id, out callbackChannel))
            {
                throw new Exception("Gate with id " + gate_id + " is not connected");
            }
            callbackChannel.SendPinSignal(data);
        }

        public event Action<byte> GateConnected;
        public event Action<byte> GateDisConnected;


        public event Action<byte, byte[]> PillConnectedStatus;
        public event Action<byte, byte[]> PillWriteCompleted;
        public event Action<byte, byte[]> PillDataRead;
        public event Action<byte, byte[]> PinSet;

        internal void onGateConnection(byte gate_id)
        {
            var GateConnectedHandler = GateConnected;
            if (GateConnectedHandler != null) GateConnectedHandler(gate_id);

        }
        internal void onGateDisconnected(byte gate_id)
        {
            var GateDisConnectedHandler = GateDisConnected;
            if (GateDisConnectedHandler != null) GateDisConnectedHandler(gate_id);
        }

        internal void onPillConnectedStatus(byte gate_id, byte[] connectedData)
        {
            var PillConnectedStatusHandler = PillConnectedStatus;
            if (PillConnectedStatusHandler != null) PillConnectedStatusHandler(gate_id, connectedData);
        }
        internal void onPillWriteCompleted(byte gate_id, byte[] writeData)
        {
            var PillWriteCompletedHandler = PillConnectedStatus;
            if (PillWriteCompletedHandler != null) PillWriteCompletedHandler(gate_id, writeData);
        }
        internal void onPillDataRead(byte gate_id, byte[] readData)
        {
            var PillDataReadHandler = PillConnectedStatus;
            if (PillDataReadHandler != null) PillDataReadHandler(gate_id, readData);
        }
        internal void onPinSet(byte gate_id, byte[] pinSetData)
        {
            var PinSetHandler = PillConnectedStatus;
            if (PinSetHandler != null) PinSetHandler(gate_id, pinSetData);
        }


    }
}
