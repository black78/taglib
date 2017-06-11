#include "WinrtFile.h"

#include <ppl.h>
#include <pplawait.h>

#include <winrt\base.h>
#include <winrt\ppl.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>

#pragma warning(push)
#pragma warning(disable: 4467)
#include <robuffer.h>
#pragma warning(pop)

namespace com = ::Windows::Storage::Streams;

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;

namespace winrt
{
  // com::IBufferByteAccess

  struct CustomBuffer 
    : implements<CustomBuffer, IBuffer, com::IBufferByteAccess>
  {
    uint8_t* m_buffer;
    uint32_t m_length;

    CustomBuffer(uint8_t* lpBuffer, uint32_t size)
      : m_buffer(lpBuffer)
      , m_length(size)
    {
    }

    uint32_t Capacity() const
    {
      return m_length;
    }

    uint32_t Length() const
    {
      return m_length;
    }

    void Length(uint32_t value)
    {
      if (value > m_length)
      {
        throw hresult_invalid_argument();
      }

      m_length = value;
    }

    HRESULT __stdcall Buffer(uint8_t** value) final
    {
      *value = m_buffer;
      return S_OK;
    }
  };


  /////////////////////////

  class FileHandle
  {
  public:
    FileHandle(IRandomAccessStream stream)
      : _stream(std::move(stream))
    {
    }

    ~FileHandle()
    {
    }

    size_t Read(byte* buffer, size_t bufferSize) 
    {
      if (bufferSize == 0) 
      {
        return 0;
      }

      IBuffer data = make<CustomBuffer>(buffer, bufferSize);
      data = Read(data).get();
      auto dataLength = min(data.Length(), bufferSize);

      return data.Length();
    }

    size_t Write(byte* buffer, size_t bufferSize)
    {
      if (bufferSize == 0)
      {
        return 0;
      }

      IBuffer data = make<CustomBuffer>(buffer, bufferSize);
      auto dataLength = Write(data).get();
      return dataLength;
    }

    uint64_t GetLength()
    {
      return _stream.Size();
    }

    void SetLength(uint64_t value)
    {
      _stream.Size(value);
    }

    uint64_t GetPosition()
    {
      return _stream.Position();
    }

    void SetPosition(uint64_t position)
    {
      return _stream.Seek(position);
    }

  private:
    concurrency::task<IBuffer> Read(size_t bufferSize)
    {
      Buffer buffer(bufferSize);
      co_await _stream.ReadAsync(buffer, bufferSize, InputStreamOptions::None);
      return buffer.as<IBuffer>();
    }

    concurrency::task<IBuffer> Read(IBuffer buffer)
    {
      co_await _stream.ReadAsync(buffer, buffer.Capacity(), InputStreamOptions::None);
      return buffer;
    }

    concurrency::task<uint32_t> Write(IBuffer buffer)
    {
      auto writted = co_await _stream.WriteAsync(buffer);
      return writted;
    }

  private:
    winrt::Windows::Storage::Streams::IRandomAccessStream _stream;
  };

  concurrency::task<winrt::FileHandlePtr> CreateFile(
    std::wstring fileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    void* lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    void* hTemplateFile
  )
  {
    try 
    {
      auto storageFile = co_await StorageFile::GetFileFromPathAsync(fileName);
      auto stream = co_await storageFile.OpenAsync(((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) ? FileAccessMode::ReadWrite : FileAccessMode::Read);
      return std::move(FileHandlePtr(new FileHandle(stream), [](FileHandle* p) {if (p != nullptr) delete p; }));
    }
    catch(...)
    {
    }

    co_return FileHandlePtr();
  }

  size_t Read(FileHandlePtr file, byte* buffer, size_t bufferSize) 
  {
    if (file)
    {
      return file->Read(buffer, bufferSize);
    }
    return 0;
  }

  size_t Write(FileHandlePtr pFile, byte* buffer, size_t bufferSize) 
  {
    return 0;
  }
}

winrt::FileHandlePtr CreateFileW(
  LPCWSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  void* lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  void* hTemplateFile
)
{
  return winrt::CreateFile(
    std::wstring(lpFileName), 
    dwDesiredAccess, 
    dwShareMode, 
    lpSecurityAttributes, 
    dwCreationDisposition, 
    dwFlagsAndAttributes, 
    hTemplateFile).get();
}

winrt::FileHandlePtr CreateFileA(
  LPCSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  void* lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  void* hTemplateFile
)
{
  std::string strFileName(lpFileName);
  return winrt::CreateFile(
    std::wstring(strFileName.begin(), strFileName.end()), 
    dwDesiredAccess, 
    dwShareMode, 
    lpSecurityAttributes, 
    dwCreationDisposition, 
    dwFlagsAndAttributes, 
    hTemplateFile).get();
}

DWORD GetFileSize(
  winrt::FileHandlePtr file,
  LPDWORD lpFileSizeHigh
)
{
  if (file)
  {
    return (DWORD)file->GetLength();
  }
  return 0;
}

DWORD SetFilePointer(
  winrt::FileHandlePtr file,
  LONG lDistanceToMove,
  PLONG lpDistanceToMoveHigh,
  DWORD dwMoveMethod
)
{
  ::SetLastError(NO_ERROR);

  long long pos = 0;
  if (lpDistanceToMoveHigh != nullptr) 
  {
    LARGE_INTEGER fullOffset;
    fullOffset.LowPart = lDistanceToMove;
    fullOffset.HighPart = (*lpDistanceToMoveHigh);
    pos = fullOffset.QuadPart;
  }
  else 
  {
    pos = lDistanceToMove;
  }

  if (file)
  {
    long long length = file->GetLength();
    long long position = file->GetPosition();

    switch (dwMoveMethod)
    {
    case FILE_BEGIN:
      {
        pos = pos;
      }break;
    case FILE_CURRENT:
      {
        pos = position + pos;
      }break;
    case FILE_END:
      {
        pos = length + pos;
      }break;
    default:
      break;
    }

    if (pos < 0)
    {
      ::SetLastError(ERROR_NEGATIVE_SEEK);
      return INVALID_SET_FILE_POINTER;
    }

    file->SetPosition(pos);

    ULARGE_INTEGER newPosition;
    newPosition.QuadPart = file->GetPosition();
    if (lpDistanceToMoveHigh != nullptr)
    {
      (*lpDistanceToMoveHigh) = newPosition.HighPart;
    }
    return newPosition.LowPart;
  }
  return INVALID_SET_FILE_POINTER;
}

BOOL ReadFile(
  winrt::FileHandlePtr file,
  LPVOID lpBuffer,
  DWORD nNumberOfBytesToRead,
  LPDWORD lpNumberOfBytesRead,
  void* lpOverlapped
)
{
  ::SetLastError(0);
  if (!file)
  {
    ::SetLastError(E_INVALIDARG);
    return FALSE;
  }

  auto readed = file->Read((byte*)lpBuffer, nNumberOfBytesToRead);
  if (lpNumberOfBytesRead != nullptr) 
  {
    (*lpNumberOfBytesRead) = readed;
  }
  return TRUE;
}

BOOL WriteFile(
  winrt::FileHandlePtr file,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  void* lpOverlapped
)
{
  ::SetLastError(0);
  if (!file)
  {
    ::SetLastError(E_INVALIDARG);
    return FALSE;
  }

  auto writed = file->Write((byte*)lpBuffer, nNumberOfBytesToWrite);
  if (lpNumberOfBytesWritten != nullptr)
  {
    (*lpNumberOfBytesWritten) = writed;
  }
  return TRUE;
}

void CloseHandle(winrt::FileHandlePtr & pFile)
{
  if (pFile) {
    pFile.reset();
  }
}

BOOL SetEndOfFile(
  winrt::FileHandlePtr file
)
{
  if (file) 
  {
    file->SetLength((uint64_t)file->GetPosition());
    return TRUE;
  }
  return FALSE;
}