using System.Collections.Generic;
using HonorInterfaces;

namespace HonorLogic
{
    public static class Pills
    {
        private class PillType : IPillType
        {
            public PillType()
            {
                DefaultCount = 1;
            }
            
            public string Name { get; set; }
            public int Id { get;  set; }
            public int DefaultCount { get; private set; }

            public override string ToString()
            {
                return Id + " " + Name;
            }
        }

        public static IEnumerable<IPillType> List
        {
            get { return ListData; }
        }

        // When this list updated, update client list 
        // /Armlet/ArmletShell/PriceOfHonor/MedCure.h

        private static readonly List<IPillType> ListData = new List<IPillType>
            {
                new PillType {Name = "Общее обезболивающее - анальгетик", Id = 0},
                new PillType {Name = "Спазмолитик", Id = 1},
                new PillType {Name = "Жаропонижающее", Id = 2},
                new PillType {Name = "Аспиратор", Id = 3},
                new PillType {Name = "Фактор свертываемости крови", Id = 4},
                new PillType {Name = "Искусственная кровь", Id = 5},
                new PillType {Name = "Искусственная кожа", Id = 6},
                new PillType {Name = "Миорелаксант", Id = 7},
                new PillType {Name = "Пак нанохирургов для внутренних органов", Id = 8},
                new PillType {Name = "Местный наркоз", Id = 9},
                new PillType {Name = "Антибиотик", Id = 10},
                new PillType {Name = "Абсорбент", Id = 11},
                new PillType {Name = "Наногипс", Id = 12},
                new PillType {Name = "Наноэкзоскелет", Id = 13},
                new PillType {Name = "Волшебная таблетка (мастерская)", Id = 14},
                new PillType {Name = "Пытка: ожог", Id = 20},
                new PillType {Name = "Пытка: конечность", Id = 21},
                new PillType {Name = "Пытка: глаз", Id = 22},
                new PillType {Name = "Пытка: удушение", Id = 23},
                new PillType {Name = "Смертельный яд", Id = 24},
                new PillType {Name = "Пытка: утопление", Id = 25},
                new PillType {Name = "Пытка: огонь", Id = 26},
            };
    }
}
