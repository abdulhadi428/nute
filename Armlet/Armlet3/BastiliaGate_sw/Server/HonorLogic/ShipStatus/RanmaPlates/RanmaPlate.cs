using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;

namespace HonorLogic.ShipStatus.RanmaPlates
{
    public class RanmaPlate
    {
        private static readonly byte[] RepairedTable = {0,0};
        private readonly List<ShipSubsystemStatus> _plateStatusList = new List<ShipSubsystemStatus>();
        private readonly IGateModel _gateModel;

        public event Action OnlineChanged;

        public RanmaPlate(int subsystemsCount, IGateModel gateModel)
        {
            _gateModel = gateModel;
            for (var i = 0; i < subsystemsCount; i++)
            {
                _plateStatusList.Add(new ShipSubsystemStatus {Severity = RanmaRepairSeverity.Ready, SubSystemNum = i});
            }

            _gateModel.DeviceDataArrived += PlateDataRead;
            _gateModel.DeviceOnlineChanged += GateModelDeviceOnlineChanged;
        }

        void GateModelDeviceOnlineChanged()
        {
            Online = _gateModel.DeviceOnline;
            RaiseOnlineChanged();
        }

        private void RaiseOnlineChanged()
        {
            var handler = OnlineChanged;
            if (handler != null)
            {
                handler();
            }
        }

        public void Refresh() // Дергает вызов чтения 16-байтов с платы
        {
            _gateModel.ReadPlate();
        }

        private void PlateDataRead(byte[] data) // Вызывается в момент, когда произведено чтение байт с платы
        {
            for (int i = 0; i <= 7; i++)
            {
                byte[] subsystemData = data.Skip(i*2).Take(2).ToArray();
                if (subsystemData.ToArray().Equals(RepairedTable))
                {
                    _plateStatusList.First(a => a.SubSystemNum == i).Severity = RanmaRepairSeverity.Ready;
                }
            }
        }

        public void SetSubsystemSeverity(int subSystemNum, RanmaRepairSeverity ranmaRepairSeverity)
        {
            _plateStatusList.First(a => a.SubSystemNum == subSystemNum).Severity = ranmaRepairSeverity;

            var repairTable = RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaRepairSeverity).Bytes;
            _gateModel.WritePlate(0, subSystemNum, repairTable);
        }

        public RanmaRepairSeverity GetSubsystemSeverity(int i)
        {
            return _plateStatusList.First(a => a.SubSystemNum == i).Severity;
        }

        public ShipSubsystemStatus GetSubsystem(int i)
        {
            return _plateStatusList.First(a => a.SubSystemNum == i);
        }

        public bool Online
        {
            get; private set;
        }
    }
}

