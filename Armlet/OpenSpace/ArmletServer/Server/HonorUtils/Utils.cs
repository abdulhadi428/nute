using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HonorUtils
{
    public static  class Utils
    {
        public static byte[] ToByteArray(params int[] intArray)
        {
            return intArray.SelectMany(BitConverter.GetBytes).ToArray();
        }
    }
}
