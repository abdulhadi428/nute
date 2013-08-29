using System;
using System.Collections.Generic;
using System.Text;

namespace HonorInterfaces
{
    public interface IPillType
    {
        string Name { get; }
        int Id { get; }
        int DefaultCount { get; }

    }
}
