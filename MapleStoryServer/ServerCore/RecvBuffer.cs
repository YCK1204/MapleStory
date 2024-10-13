using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public class RecvBuffer
    {
        ArraySegment<byte> _buffer;

        int _readPos;
        int _writePos;

        public RecvBuffer(int bufferLength)
        {
            _buffer = new ArraySegment<byte>(new byte[bufferLength], 0, bufferLength);
        }

        public int ReadSize { get { return _writePos - _readPos; } }
        public int WriteSize { get { return _buffer.Count - _writePos; } }

        public ArraySegment<byte> ReadSegment { get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _readPos, ReadSize); } }
        public ArraySegment<byte> WriteSegment { get { return new ArraySegment<byte>(_buffer.Array, _buffer.Offset + _writePos, WriteSize); } }
        public void Clear()
        {
            int readSize = ReadSize;
            if (_readPos == _writePos)
            {
                _readPos = _writePos = 0;
            }
            else
            {
                Array.Copy(_buffer.Array, _buffer.Offset + _readPos, _buffer.Array, _buffer.Offset, readSize);
                _readPos = 0;
                _writePos = readSize;
            }
        }
        public bool OnRead(int numOfBytes)
        {
            if (ReadSize < numOfBytes)
                return false;
            _readPos += numOfBytes;
            return true;
        }

        public bool OnWrite(int numOfBytes)
        {
            if (WriteSize < numOfBytes)
                return false ;
            _writePos += numOfBytes;
            return true;
        }
    }
}
