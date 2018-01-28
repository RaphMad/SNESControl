namespace SNESControl.SaveToFile
{
    using System.IO;
    using System.Linq;

    public class ReplayFileReader
    {
        private readonly byte[] _bytesFromFile;

        private int _currentIndex;

        public ReplayFileReader(string fileName)
        {
            _bytesFromFile = File.ReadAllBytes(fileName);
        }

        public byte[] GetNextBytes(byte numberOfBytes)
        {
            _currentIndex += numberOfBytes;

            return _bytesFromFile.Skip(_currentIndex).Take(numberOfBytes).ToArray();
        }
    }
}