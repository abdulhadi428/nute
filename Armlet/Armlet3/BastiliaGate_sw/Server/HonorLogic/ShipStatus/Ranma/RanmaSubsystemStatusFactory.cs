using System;
using System.Collections.Generic;
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
                RanmaRepairSeverity.Easy, new ushort[]
                {
                    	0x00AA, 0xC0EA, 0x30BA, 0xC0D5, 0x55F0, 0x0F02, 0xF022, 0xC0C8, 0x2727, 0x7766,
0x0CAE, 0xC3C3, 0x5AFA, 0xDDD5, 0x55D5, 0x0F2F, 0xF0F2, 0x0F0D, 0xCF8A, 0x6644
                }
            },
            {
                RanmaRepairSeverity.Medium, new ushort[]
                {
                    0x0EEA, 0xD1EA, 0x35BE, 0xD195, 0x7DF0, 0x2F52, 0x7C22, 0x50E8, 0x2755, 0x7EE6,
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
            var singleOrDefault = Table.Where(t => t.Value.Contains(tableValue)).ToArray();
            return !singleOrDefault.Any() ? RanmaRepairSeverity.NotDamaged : singleOrDefault.Single().Key;
        }

        public static ushort MakeUInt16(this byte[] info)
        {
            if (BitConverter.IsLittleEndian)
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
