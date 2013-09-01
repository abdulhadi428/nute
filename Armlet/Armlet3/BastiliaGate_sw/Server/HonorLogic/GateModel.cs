 using System;
using System.Collections.Generic;
 using System.Linq;
 using HonorUtils;
using NetworkLevel.NetworkDeliveryLevel;
using HonorInterfaces;

namespace HonorLogic
{
    public class GateModel : IGateModel
    {
        private readonly IGateDeliveryService _service;
        private readonly byte _gateId;
        private const byte MaxReadBytes = 24;

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
            if (DeviceDataArrived != null)
            {
                DeviceDataArrived(arg3.Skip(2).ToArray());
            }
        }

        private readonly object _syncRoot = new object();

        private void SetPilStatus(bool status)
        {
            lock (_syncRoot)
            {
                if (status != DeviceOnline)
                {
                    DeviceOnline = status;
                    if (DeviceOnlineChanged != null)
                    {
                        DeviceOnlineChanged();
                    }
                }
                if (status)
                {
                    ReadDevice();
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

        public bool DeviceOnline { get; private set; }

        public bool Online { get; private set; }
        public event Action DeviceOnlineChanged;
        public event Action GateOnlineChanged;
        public event Action<byte[]> DeviceDataArrived;

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
            WriteDevice(new[] {pillAddress}.Concat(Utils.ToBytes(p, charges)).ToArray());
        }

        private void WriteDevice(byte[] bytesToWrite)
        {
            try
            {
                _service.SendPillWhite(_gateId, bytesToWrite);
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

        public void ReadPlate()
        {
            ReadDevice();
        }

        private void ReadDevice(byte pillAddress = 0x00, byte length = MaxReadBytes)
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

        public void WritePlate(byte pillAddress, int subSystemNum, byte[] repairTable)
        {
            WriteDevice(new [] {pillAddress, (byte) subSystemNum}.Concat(
                repairTable).ToArray());
        }
    }
}