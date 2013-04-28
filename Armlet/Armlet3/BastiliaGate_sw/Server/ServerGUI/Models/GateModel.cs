using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NetworkLevel.NetworkDeliveryLevel;
using PillInterfaces;

namespace ServerGUI.Models
{
    public class GateModel
    {
        private readonly IGateDeliveryService _service;
        private readonly byte _gateId;

        public GateModel(IGateDeliveryService service, byte gateId)
        {
            _service = service;
            _gateId = gateId;
            
            _service.GateDisConnected += id => IfMe(id, () => SetOnline(true));
            _service.GateDisConnected += id => IfMe(id, () => SetOnline(false));

            _service.PillConnectedStatus += (id, status) => IfMe(id, () => SetPillStatus(status));
            _service.PillDataRead +=(id, status, data) => IfMe(id, () => RaisePillDataArrived(status, data));
        }

        private void RaisePillDataArrived(bool status, byte[] arg3)
        {
            if (!status)
            {
                SetPillStatus(false);
                return;
            }
            if (PillDataArrived != null)
            {
                PillDataArrived(arg3);
            }
        }

        private readonly object _syncRoot = new object();

        private void SetPillStatus(bool status)
        {
            lock (_syncRoot)
            {
                if (status != PillOnline)
                {
                    PillOnline = status;
                    if (PillOnlineChanged != null)
                    {
                        PillOnlineChanged();
                    }
                }
            }
        }

        private void IfMe(byte id, Action action)
        {
            if (_gateId == id)
            {
                action();
            }
        }

        private void SetOnline(bool online)
        {
            Online = online;
            if (GateOnlineChanged != null)
            {
                GateOnlineChanged();
            }
        }

        public bool PillOnline { get; private set; }

        public bool Online { get; private set; }
        public event Action PillOnlineChanged;
        public event Action GateOnlineChanged;
        public event Action<byte[]> PillDataArrived;

        public void ActivatePin()
        {
            _service.SendPinSignal(_gateId, new byte[] {0, 0x02, 10});
        }

        public IEnumerable<IPillType> PillTypes { get; set; }

        private static byte[] ToByteArray(params int[] intArray)
        {
            return intArray.SelectMany(BitConverter.GetBytes).ToArray();
        }

        public void WritePill(int p, int charges)
        {
            var payload = BitConverter.GetBytes(p).Concat(BitConverter.GetBytes(charges)).ToArray();
            _service.SendPillWhite(_gateId, ToByteArray(p, charges));
        }

        public void RefreshPillStatus()
        {
            _service.CheckIfPillConnected(_gateId, new byte[] {});
        }
    }
}
