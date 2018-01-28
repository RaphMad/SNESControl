namespace SNESControl.SaveToFile
{
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
            _bytesFromFile = File.ReadAllBytes(_fileName);
            _currentIndex = 0;
        }

        public byte[] GetNextBytes(byte numberOfBytes)
        {
            byte[] bytes = _bytesFromFile.Skip(_currentIndex).Take(numberOfBytes).ToArray();

            _currentIndex += numberOfBytes;

            return bytes;
        }
    }
}