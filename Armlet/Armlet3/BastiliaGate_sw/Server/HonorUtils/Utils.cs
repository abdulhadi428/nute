using System;
using System.Collections.Generic;
using System.Linq;
using HonorUtils.Annotations;

namespace HonorUtils
{
    public static  class Utils
    {
        private static readonly Random Rnd = new Random();
        public static IEnumerable<byte> ToBytes(params int[] intArray)
        {
            return intArray.SelectMany(BitConverter.GetBytes);
        }

        [CanBeNull]
        public static T RandomOrDefault<T>(this ICollection<T> col)
        {
            return col.Count == 0 ? default (T) : GetRnd(col);
        }

        private static T GetRnd<T>(ICollection<T> col)
        {
            var val = Rnd.Next(col.Count - 1);
            return col.Skip(val).First();
        }

        [CanBeNull]
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
