namespace SNESControl.SaveToFile
{
    using System;
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
    }
}