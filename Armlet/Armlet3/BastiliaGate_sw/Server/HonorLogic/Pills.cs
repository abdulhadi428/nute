using System.Collections.Generic;
using PillInterfaces;

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
        }

        public static IEnumerable<IPillType> List
        {
            get { return ListData; }
        }

        // When this list updated, update client list 
        // /Armlet/ArmletShell/PriceOfHonor/MedCure.h

        private static readonly List<IPillType> ListData = new List<IPillType>
            {
                new PillType {Name = "Общее обезболивающее", Id = 0},
                new PillType {Name = "Спазмолитик", Id = 1},
                new PillType {Name = "Жаропонижающее", Id = 2},
                new PillType {Name = "Аспиратор", Id = 3},
                new PillType {Name = "Фактор свертываемости крови", Id = 4},
                new PillType {Name = "Искусственная кровь", Id = 5},
                new PillType {Name = "Искусственная кожа", Id = 6},
                new PillType {Name = "Миорелаксант", Id = 7},
                new PillType {Name = "Пак нанохирургов для внутренних органов", Id = 8},
                new PillType {Name = "Пак дыхательных нанохирургов", Id = 9},
            };
    }
}
