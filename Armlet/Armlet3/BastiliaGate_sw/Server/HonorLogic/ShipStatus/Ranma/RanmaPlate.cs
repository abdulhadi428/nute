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
            foreach (var shipSubsystemStatuse in _plateStatusList)
            {
                var offset = shipSubsystemStatuse.SubSystemNum*4;

                var subsystemData = data.Skip(offset + 2).Take(2).ToArray();
                var lastData = data.Skip(offset).Take(2).ToArray();

                var severity = lastData.ToSeverity();

                shipSubsystemStatuse.Severity = severity;

                shipSubsystemStatuse.RepairedStatus = severity == RanmaRepairSeverity.NotDamaged
                    ? (ushort) 0
                    : subsystemData.MakeUInt16();
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

