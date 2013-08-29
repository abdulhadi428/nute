using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Mail;
using System.Text;

namespace HonorUtils
{
    public static  class Utils
    {
        public static byte[] ToByteArray(params int[] intArray)
        {
            return intArray.SelectMany(BitConverter.GetBytes).ToArray();
        }

        public static T Random<T>(this ICollection<T> col, Random rnd)
        {
            var val = rnd.Next(col.Count - 1);
            return col.Skip(val).First();
        }
    }
}
