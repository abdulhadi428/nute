using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using PillInterfaces;

namespace HonorLogic
{
    class PillType : IPillType
    {
        public PillType(int id, string name)
        {
            Id = id;
            Name = name;
            DefaultCount = 1;
        }

        public string Name { get; private set; }
        public int Id { get; private set; }
        public int DefaultCount { get; private set; }
    }
}
