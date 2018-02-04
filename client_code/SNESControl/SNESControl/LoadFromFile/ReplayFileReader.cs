namespace SNESControl.SaveToFile
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;

    public class ReplayFileReader
    {
        private readonly string _fileName;

        private byte[] _bytesFromFile = {};

        private int _currentIndex;

        public ReplayFileReader(string fileName)
        {
            _fileName = fileName;
        }

        public void Read()
        {
            try
            {
                _currentIndex = 0;
                _bytesFromFile = File.ReadAllBytes(_fileName);
                //_bytesFromFile = ConvertFromSnes9xFile(File.ReadAllBytes(_fileName));
                //_bytesFromFile = ConvertFromLsnesFile(File.ReadAllLines(_fileName));
                //_bytesFromFile = ConvertFromLsnesDumpFile(File.ReadAllBytes(_fileName));
            }
            catch (Exception e)
            {
                Console.WriteLine("Could not open file for loading: " + e.Message);
                
            }
        }

        public byte[] GetNextBytes(byte numberOfBytes)
        {
            byte[] bytes = _bytesFromFile.Skip(_currentIndex).Take(numberOfBytes).ToArray();

            _currentIndex += numberOfBytes;

            return bytes;
        }

        private byte[] ConvertFromLsnesDumpFile(byte[] file)
        {
            var convertedBytes = new List<byte>();

            uint inputOffset = BitConverter.ToUInt32(file, 0x1C);
            ushort frameTime = 0;

            for (uint i = inputOffset; i < file.Length; i += 2)
            {
                byte byte1 = file[i];
                byte byte2 = file[i + 1];

                convertedBytes.Add((byte)~byte2);
                convertedBytes.Add((byte)~byte1);

                convertedBytes.AddRange(BitConverter.GetBytes(frameTime));
                frameTime += 16;
            }

            return convertedBytes.ToArray();
        }

        private byte[] ConvertFromSnes9xFile(byte[] file)
        {
            var convertedBytes = new List<byte>();

            uint inputOffset = BitConverter.ToUInt32(file, 0x1C);
            ushort frameTime = 0;

            for (uint i = inputOffset; i < file.Length; i += 2)
            {
                byte byte1 = file[i];
                byte byte2 = file[i + 1];

                convertedBytes.Add(Reverse((byte)~byte2));
                convertedBytes.Add(Reverse((byte)~byte1));

                convertedBytes.AddRange(BitConverter.GetBytes(frameTime));
                frameTime += 16;
            }

            return convertedBytes.ToArray();
        }

        // Reverses bits in a byte
        public static byte Reverse(byte inByte)
        {
            byte result = 0x00;

            for (byte mask = 0x80; Convert.ToInt32(mask) > 0; mask >>= 1)
            {
                // shift right current result
                result = (byte)(result >> 1);

                // tempbyte = 1 if there is a 1 in the current position
                var tempbyte = (byte)(inByte & mask);
                if (tempbyte != 0x00)
                {
                    // Insert a 1 in the left
                    result = (byte)(result | 0x80);
                }
            }

            return (result);
        }

        private byte[] ConvertFromLsnesFile(string[] file)
        {
            var convertedBytes = new List<byte>();
            ushort frameTime = 0;

            for (int i = 0; i < file.Length; i++)
            {
                string currentLine = file[i];

                currentLine = currentLine.Substring(7);

                byte byte1 = 255;
                byte byte2 = 255;

                if (currentLine.Contains("B")) byte1 &= 0b11111110;
                if (currentLine.Contains("Y")) byte1 &= 0b11111101;
                if (currentLine.Contains("s")) byte1 &= 0b11111011;
                if (currentLine.Contains("S")) byte1 &= 0b11110111;
                if (currentLine.Contains("u")) byte1 &= 0b11101111;
                if (currentLine.Contains("d")) byte1 &= 0b11011111;
                if (currentLine.Contains("l")) byte1 &= 0b10111111;
                if (currentLine.Contains("r")) byte1 &= 0b01111111;

                if (currentLine.Contains("A")) byte2 &= 0b11111110;
                if (currentLine.Contains("X")) byte2 &= 0b11111101;
                if (currentLine.Contains("L")) byte2 &= 0b11111011;
                if (currentLine.Contains("R")) byte2 &= 0b11110111;

                convertedBytes.Add(byte1);
                convertedBytes.Add(byte2);

                convertedBytes.AddRange(BitConverter.GetBytes(frameTime));
                frameTime += 16;

            }

            return convertedBytes.ToArray();
        }
    }
}