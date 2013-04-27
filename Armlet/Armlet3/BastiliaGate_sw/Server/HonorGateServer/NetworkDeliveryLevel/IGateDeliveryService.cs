using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HonorGateServer.NetworkDeliveryLevel
{
    public interface IGateDeliveryService
    {
        event Action<byte> GateConnected;                       //byte gate_id
        event Action<byte> GateDisConnected;                    //byte gate_id

        void CheckIfPillConnected(byte gate_id, byte[] data);    // data[] first byte is "0", the rest - empty
        event Action<byte, bool> PillConnectedStatus;            // byte gate_id, bool connection_status

        void SendPillWhite(byte gate_id, byte[] data);          // data[] first byte is "0", the rest - actual data
        event Action<byte, bool> PillWriteCompleted;            // byte gate_id, bool write_status

        void SendPillRead(byte gate_id, byte[] data);           // data[] first byte is "0", second byte - N of bytes to read, rest - empty
        event Action<bool, byte[]> PillDataRead;                // bool read_status, byte[] data_read 

        void SendPinSignal(byte gate_id, byte[] data);          // data[] first byte is "0", second - type, third - signal length
        event Action<byte, byte[]> PinSet;                      // byte gete_id, byte[] data (data[] first byte is "0", second - success_status)
    }
}
