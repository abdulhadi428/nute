using System;
using System.Collections.Generic;
using System.Linq;

namespace HonorUtils
{
    public static  class Utils
    {
        private static readonly Random rnd = new Random();
        public static byte[] ToByteArray(params int[] intArray)
        {
            return intArray.SelectMany(BitConverter.GetBytes).ToArray();
        }

        public static T RandomOrDefault<T>(this ICollection<T> col)
        {
            return col.Count == 0 ? default (T) : GetRnd(col);
        }

        private static T GetRnd<T>(ICollection<T> col)
        {
            var val = rnd.Next(col.Count - 1);
            return col.Skip(val).First();
        }

        public static T RandomOrDefault<T>(this IEnumerable<T> col)
        {
            return col.ToArray().RandomOrDefault();
        }

        public static T Random<T>(this ICollection<T> col)
        {
            if (col.Count == 0)
            {
                throw new InvalidOperationException();
            }
            return GetRnd(col);
        }

        public static T Random<T>(this IEnumerable<T> col)
        {
            return col.ToArray().Random();
        }
    }
}
