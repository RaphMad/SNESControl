namespace SNESControl.SaveToFile
{
    using System.IO;

    public class ReplayFileWriter
    {
        private readonly string _fileName;

        private FileStream _file;

        private volatile bool _isClosed;

        public ReplayFileWriter(string fileName)
        {
            _fileName = fileName;
        }

        public void Open()
        {
            _file = File.Open(_fileName, FileMode.Create, FileAccess.Write);
            _isClosed = false;
        }

        public void AppendBytes(byte[] bytes)
        {
            if (!_isClosed)
            {
                _file?.Write(bytes, 0, bytes.Length);
                _file?.Flush(true);
            }
        }

        public void Close()
        {
            _file?.Dispose();
            _isClosed = true;
        }
    }
}