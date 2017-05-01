#pragma once

#ifndef WINRT_FILE_H
#define WINRT_FILE_H

#ifdef HAVE_CONFIG_H
#include <windows.h>
#endif

#include <memory>
#include <functional>

namespace winrt
{
  class FileHandle;
  typedef ::std::shared_ptr<FileHandle> FileHandlePtr;
}

winrt::FileHandlePtr CreateFileW(
  LPCWSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  void* lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  void* hTemplateFile
);

winrt::FileHandlePtr CreateFileA(
  LPCSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  void* lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  void* hTemplateFile
);

DWORD GetFileSize(
  winrt::FileHandlePtr file,
  LPDWORD lpFileSizeHigh
);

DWORD SetFilePointer(
  winrt::FileHandlePtr file,
  LONG lDistanceToMove,
  PLONG lpDistanceToMoveHigh,
  DWORD dwMoveMethod
);

BOOL SetEndOfFile(
  winrt::FileHandlePtr file
);

BOOL ReadFile(
  winrt::FileHandlePtr file,
  LPVOID lpBuffer,
  DWORD nNumberOfBytesToRead,
  LPDWORD lpNumberOfBytesRead,
  void* lpOverlapped
);

BOOL WriteFile(
  winrt::FileHandlePtr file,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  void* lpOverlapped
);

void CloseHandle(winrt::FileHandlePtr& pFile);


#endif