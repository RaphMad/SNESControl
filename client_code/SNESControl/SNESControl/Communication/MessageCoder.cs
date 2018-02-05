namespace SNESControl
{
    using System.Collections.Generic;
    using System.Linq;

    public static class MessageCoder
    {
        public static byte StartMarker = 0;
        public static byte EndMarker = 1;
        public static byte EncodeMarker = 2;

        public static byte[] Encode(byte[] bytes)
        {
            var encodedBytes = new List<byte> { StartMarker };

            foreach (byte currentByte in bytes)
            {
                if (NeedsEncoding(currentByte))
                {
                    encodedBytes.Add(EncodeMarker);
                    encodedBytes.Add(Encode(currentByte));
                }
                else
                {
                    encodedBytes.Add(currentByte);
                }
            }

            encodedBytes.Add(EndMarker);

            return encodedBytes.ToArray();
        }

        private static bool NeedsEncoding(byte value)
        {
            return value == StartMarker || value == EncodeMarker || value == EndMarker;
        }

        private static byte Encode(byte value)
        {
            unchecked
            {
                // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
                return (byte)(value + EncodeMarker + 1);
            }
        }

        public static byte[] Decode(byte[] bytes)
        {
            var decodedBytes = new List<int>();

            // 1 skips the start marker, -1 omits the end marker
            for (int i = 1; i < bytes.Length - 1; i++)
            {
                if (bytes[i] == EncodeMarker)
                {
                    i++;
                    decodedBytes.Add(Decode(bytes[i]));
                }
                else
                {
                    decodedBytes.Add(bytes[i]);
                }
            }

            return decodedBytes.Select(x => (byte)x).ToArray();
        }

        private static byte Decode(byte value)
        {
            unchecked
            {
                // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
                return (byte)(value - EncodeMarker - 1);
            }
        }
    }
}