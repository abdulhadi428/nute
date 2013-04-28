using System;
using System.Collections.Generic;
using HonorGateServer.NetworkDeliveryLevel;
using HonorUtils;
using PillInterfaces;

namespace HonorLogic
{
    public class GateModel : IGateModel
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

        public void WritePill(int p, int charges)
        {
            _service.SendPillWhite(_gateId, Utils.ToByteArray(p, charges));
        }

        public void RefreshPillStatus()
        {
            _service.CheckIfPillConnected(_gateId, new byte[] {});
        }
    }
}