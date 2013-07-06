 using System;
using System.Collections.Generic;
 using System.Linq;
 using HonorUtils;
using NetworkLevel.NetworkDeliveryLevel;
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
            
            _service.GateConnected+= id => IfMe(id, () => SetOnline(true));
            _service.GateDisConnected += id => IfMe(id, () => SetOnline(false));

            _service.PillConnectedStatus += (id, status) => SetPilStatus(status[0] == 0);
            _service.PillDataRead +=(id, data) => IfMe(id, () => RaisePillDataArrived(data));
            
            Online = true;
        }

        private void RaisePillDataArrived(byte[] arg3)
        {
            var address = arg3[0];
            var status = arg3[1] == 0;
            if (address != 0)
            {
                return;
            }

            if (!status)
            {
                SetPilStatus(false); // Error read, let's think offline
                return;
            }
            if (PillDataArrived != null)
            {
                PillDataArrived(arg3.Skip(2).ToArray());
            }
        }

        private readonly object _syncRoot = new object();

        private void SetPilStatus(bool status)
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
                if (status)
                {
                    ReadPill();
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
            if (!online)
            {
                SetPilStatus(false);
            }
        }

        public bool PillOnline { get; private set; }

        public bool Online { get; private set; }
        public event Action PillOnlineChanged;
        public event Action GateOnlineChanged;
        public event Action<byte[]> PillDataArrived;

        public void ActivatePin()
        {
            try
            {
                _service.SendPinSignal(_gateId, new byte[] {0, 0x02, 0x10});
            }
            catch (GateNotConnectedException)
            {
                SetOnline(false);
            }
        }

        public void WritePill(int p, int charges, byte pillAddress = 0)
        {
            try
            {
                _service.SendPillWhite(_gateId, new [] { pillAddress}.Concat(Utils.ToByteArray(p, charges)).ToArray());
            }
            catch (GateNotConnectedException)
            {
                SetOnline(false);
            }
        }

        public IEnumerable<IPillType> PillTypes
        {
            get { return Pills.List; }
        }

        public void RefreshPillStatus(byte pillAddress = 0x00)
        {
            try
            {
                _service.CheckIfPillConnected(_gateId, new[] {pillAddress});
            }
            catch (GateNotConnectedException)
            {
                SetOnline(false);
            }
        }

        private void ReadPill(byte pillAddress = 0x00, byte length = 8)
        {
            try
            {
                _service.SendPillRead(_gateId, new[] { pillAddress, length});
            }
            catch (GateNotConnectedException)
            {
                SetOnline(false);
            }
        }
    }
}