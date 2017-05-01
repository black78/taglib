/***************************************************************************
    copyright           : (C) 2007 by Lukas Lalinsky
    email               : lukas@oxygene.sk
 ***************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License version   *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 *                                                                         *
 *   Alternatively, this file is available under the Mozilla Public        *
 *   License Version 1.1.  You may obtain a copy of the License at         *
 *   http://www.mozilla.org/MPL/                                           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

#ifdef _WIN32
#ifdef TAGLIB_UWP

#include <ppl.h>
#include <pplawait.h>

#include <winrt\base.h>
#include <winrt\ppl.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>

#define BaseTestsDataUrl L"ms-appx:///"

inline concurrency::task<string> testFilePathAsync(const string &filename)
{
  std::wstring relativeFilePath;
  std::wstring wFileName(filename.begin(), filename.end());
  relativeFilePath = BaseTestsDataUrl + wFileName;
  auto srcFile = co_await winrt::Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(winrt::Windows::Foundation::Uri(relativeFilePath));
  wstring wPath = srcFile.Path();
  return string(wPath.begin(), wPath.end());
}

inline concurrency::task<string> copyFileAsync(const string &filename, const string &ext) 
{
  try 
  {
    std::wstring relativeFilePath;
    std::wstring wFileName(filename.begin(), filename.end());
    std::wstring wExt(ext.begin(), ext.end());
    wFileName += wExt;

    auto tempFolder = winrt::Windows::Storage::ApplicationData::Current().TemporaryFolder();

    relativeFilePath = BaseTestsDataUrl + wFileName;
    auto srcFile = co_await winrt::Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(winrt::Windows::Foundation::Uri(relativeFilePath));
    auto targetFile = co_await tempFolder.CreateFileAsync(wFileName, winrt::Windows::Storage::CreationCollisionOption::GenerateUniqueName);
    srcFile.CopyAndReplaceAsync(targetFile);

    wstring wTargetFileName = targetFile.Path();
    return string(wTargetFileName.begin(), wTargetFileName.end());
  }
  catch (winrt::hresult_error err)
  {
    wstring message = err.message();
    auto hresult = err.code();
    throw;
  }
}

inline concurrency::task<void> deleteFileAsync(const string &filename)
{
  std::wstring wFileName(filename.begin(), filename.end());
  auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(wFileName);
  co_await file.DeleteAsync();
}

#endif
#endif

inline string testFilePath(const string &filename)
{
#ifdef TAGLIB_UWP
  return testFilePathAsync(filename).get();
#else
  return string(TESTS_DIR "data/") + filename;
#endif
}

#define TEST_FILE_PATH_C(f) testFilePath(f).c_str()

inline string copyFile(const string &filename, const string &ext)
{
  char testFileName[1024];

#ifdef _WIN32
#ifdef TAGLIB_UWP
  return copyFileAsync(filename, ext).get();
#else
  char tempDir[MAX_PATH + 1];
  GetTempPathA(sizeof(tempDir), tempDir);
  wsprintfA(testFileName, "%s\\taglib-test%s", tempDir, ext.c_str());
#endif
#else
  snprintf(testFileName, sizeof(testFileName), "/%s/taglib-test%s", P_tmpdir, ext.c_str());
#endif

  string sourceFileName = testFilePath(filename) + ext;
  ifstream source(sourceFileName.c_str(), std::ios::binary);
  ofstream destination(testFileName, std::ios::binary);
  destination << source.rdbuf();
  return string(testFileName);
}

inline void deleteFile(const string &filename)
{
#ifdef TAGLIB_UWP
  deleteFileAsync(filename).get();
#else
  remove(filename.c_str());
#endif
}

inline bool fileEqual(const string &filename1, const string &filename2)
{
  char buf1[BUFSIZ];
  char buf2[BUFSIZ];

  ifstream stream1(filename1.c_str(), ios_base::in | ios_base::binary);
  ifstream stream2(filename2.c_str(), ios_base::in | ios_base::binary);

  if(!stream1 && !stream2) return true;
  if(!stream1 || !stream2) return false;

  for(;;)
  {
    stream1.read(buf1, BUFSIZ);
    stream2.read(buf2, BUFSIZ);

    streamsize n1 = stream1.gcount();
    streamsize n2 = stream2.gcount();

    if(n1 != n2) return false;

    if(n1 == 0) break;

    if(memcmp(buf1, buf2, static_cast<size_t>(n1)) != 0) return false;
  }

  return stream1.good() == stream2.good();
}

class ScopedFileCopy
{
public:
  ScopedFileCopy(const string &filename, const string &ext, bool deleteFile=true)
  {
    m_deleteFile = deleteFile;
    m_filename = copyFile(filename, ext);
  }

  ~ScopedFileCopy()
  {
    if(m_deleteFile)
      deleteFile(m_filename);
  }

  string fileName()
  {
    return m_filename;
  }

private:
  bool m_deleteFile;
  string m_filename;
};
