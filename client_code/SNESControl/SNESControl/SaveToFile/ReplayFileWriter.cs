namespace SNESControl.SaveToFile
{
    using System.IO;

    public class ReplayFileWriter
    {
        private readonly FileStream _file;

        private volatile bool _isClosed;

        public ReplayFileWriter(string fileName)
        {
            _file = File.Open(fileName, FileMode.Create, FileAccess.Write);
        }

        public void AppendBytes(byte[] bytes)
        {
            if (!_isClosed)
            {
                _file.Write(bytes, 0, bytes.Length);
            }
        }

        public void Close()
        {
            _file.Dispose();
            _isClosed = true;
        }
    }
}