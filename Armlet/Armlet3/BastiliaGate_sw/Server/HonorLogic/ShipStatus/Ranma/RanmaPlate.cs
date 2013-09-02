using System;
using System.Collections.Generic;
using System.Linq;
using HonorInterfaces;

namespace HonorLogic.ShipStatus.Ranma
{
    public class RanmaPlate
    {
        private readonly List<ShipSubsystemStatus> _plateStatusList = new List<ShipSubsystemStatus>();
        private readonly IGateModel _gateModel;

        public event Action OnlineChanged;

        public RanmaPlate(int subsystemsCount, IGateModel gateModel)
        {
            _gateModel = gateModel;
            for (var i = 0; i < subsystemsCount; i++)
            {
                _plateStatusList.Add(new ShipSubsystemStatus {Severity = RanmaRepairSeverity.NotDamaged, SubSystemNum = i});
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
            var realData = data.Take(16).ToArray();
            var dataAfterRepair = data.Skip(16).ToArray();
            for (int i = 0; i <= 7; i++)
            {
                byte[] subsystemData = dataAfterRepair.Skip(i * 4).Take(2).ToArray();
                var lastData = realData.Skip(i*4).Take(2).ToArray();

                this[i].Severity = lastData.ToSeverity();
                this[i].RepairedStatus = subsystemData.MakeUInt16();
            }
        }

        public void SetSubsystemSeverity(int subSystemNum, RanmaRepairSeverity ranmaRepairSeverity)
        {
            this[subSystemNum].Severity = ranmaRepairSeverity;

            var repairTable = RanmaSubsystemStatusFactory.GenerateRanmaSubsystemStatus(ranmaRepairSeverity).Bytes;
            _gateModel.WritePlate(0, subSystemNum, repairTable);
        }

        public ShipSubsystemStatus this[int index]
        {
            get
            {
                return _plateStatusList.First(a => a.SubSystemNum == index);
            }
        }

        public bool Online
        {
            get; private set;
        }
    }
}

