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
            var encodedBytes = new List<int> { StartMarker };

            foreach (byte currentByte in bytes)
            {
                // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
                if (currentByte == StartMarker)
                {
                    encodedBytes.Add(EncodeMarker);
                    encodedBytes.Add(currentByte + EncodeMarker + 1);
                }
                else if (currentByte == EndMarker)
                {
                    encodedBytes.Add(EncodeMarker);
                    encodedBytes.Add(currentByte + EncodeMarker + 1);
                }
                else if (currentByte == EncodeMarker)
                {
                    encodedBytes.Add(EncodeMarker);
                    encodedBytes.Add(currentByte + EncodeMarker + 1);
                }
                else
                {
                    encodedBytes.Add(currentByte);
                }
            }

            encodedBytes.Add(EndMarker);

            return encodedBytes.Select(x => (byte)x).ToArray();
        }

        public static byte[] Decode(byte[] bytes)
        {
            var decodedBytes = new List<int>();

            // 1 skips the start marker, -1 omits the end marker
            for (int i = 1; i < bytes.Length - 1; i++)
            {
                int currentByte = bytes[i];

                if (currentByte == EncodeMarker)
                {
                    // coding scheme: 0 <-> 2 3 , 1 <-> 2 4, 2 <-> 2 5
                    i++;
                    int nextByte = bytes[i];
                    currentByte = nextByte - EncodeMarker - 1;
                }

                // re-use the receive buffer for the decoded data
                decodedBytes.Add(currentByte);
            }

            return decodedBytes.Select(x => (byte)x).ToArray();
        }
    }
}