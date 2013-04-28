using System;

namespace NetworkLevel.NetworkDeliveryLevel
{
    class GateDeliveryService:IGateDeliveryService
    {
        public event Action<byte> GateConnected;
        public event Action<byte> GateDisConnected;
        public void CheckIfPillConnected(byte gate_id, byte[] data)
        {
            throw new NotImplementedException();
        }

        public event Action<byte, bool> PillConnectedStatus;
        public void SendPillWhite(byte gate_id, byte[] data)
        {
            throw new NotImplementedException();
        }

        public event Action<byte, bool> PillWriteCompleted;
        public void SendPillRead(byte gate_id, byte[] data)
        {
            throw new NotImplementedException();
        }

        public event Action<byte, bool, byte[]> PillDataRead;
        public void SendPinSignal(byte gate_id, byte[] data)
        {
            throw new NotImplementedException();
        }

        public event Action<byte, byte[]> PinSet;
    }
}
