using System;
using System.Collections.Generic;
using System.Text;

namespace PillInterfaces
{
    public interface IPillType
    {
        string Name { get; }
        int Id { get; }
        int DefaultCount { get; }

    }
}
