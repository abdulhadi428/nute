using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using HonorInterfaces;
using HonorUtils;

namespace HonorLogic.ShipStatus.Ranma
{
    public static class RanmaSubsystemStatusFactory
    {
        private static readonly Dictionary<RanmaRepairSeverity, ushort[]> Table = new Dictionary
            <RanmaRepairSeverity, ushort[]>
        {
            {RanmaRepairSeverity.NotDamaged, new ushort[] {0x0}},
            {
                //RanmaRepairSeverity.Easy, new ushort[]
                //{
                //    0x0303, 0x8228, 0x1111, 0xA0A0, 0x0AA0, 0xA6A6, 0x8800,
                //    0x8808, 0x0161, 0x6116, 0x1310, 0x1F10, 0x1810, 0x4941, 0x2332,
                //    0x0F99, 0x909F, 0x4F7F, 0x0B0B, 0x7077
                //}
                RanmaRepairSeverity.Easy, new ushort[]
                {
                    0xFFFF
                }
            },
            {
                RanmaRepairSeverity.Medium, new ushort[]
                {
                    0x0EEA, 0xD1EA, 0x35BE, 0xD195, 0x7DF0, 0x2F92, 0x7C22, 0x50E8, 0x2755, 0x7EE6,
                    0x0DA4, 0xC9C7, 0x5A70, 0x9DD5, 0x7DD7, 0x8B2E, 0x72D0, 0x2E0D, 0xACC8, 0x7ADA
                }
            },
            {
                RanmaRepairSeverity.Hard, new ushort[]
                {

                    0x1EDA, 0xD16B, 0x258E, 0x4115, 0x3DE0, 0x0512, 0xB422, 0x14E0, 0x2155, 0x60E6,
                    0x09A0, 0x5885, 0x0421, 0xBDC5, 0x4595, 0x8B26, 0x5AD2, 0x06CD, 0x8ECB, 0x5A9E
                }

            },
        };

        public static RanmaRepairSeverity ToSeverity(this byte[] info)
        {
            var tableValue = info.MakeUInt16();
            return Table.Single(t => t.Value.Contains(tableValue)).Key;
        }

        public static ushort MakeUInt16(this byte[] info)
        {
            if (BitConverter.IsLittleEndian == true)
            {
                info = info.Reverse().ToArray();
            }
            var tableValue = BitConverter.ToUInt16(info, 0);
            return tableValue;
        }

        public static RanmaSubsystemTable GenerateRanmaSubsystemStatus(RanmaRepairSeverity severity)
        {
            return new RanmaSubsystemTable(Table[severity].Random());
        }
    }
}
