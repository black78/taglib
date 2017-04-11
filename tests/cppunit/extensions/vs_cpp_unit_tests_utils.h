#ifndef _VS_CPP_UNIT_TESTS_UTILS_H
#define _VS_CPP_UNIT_TESTS_UTILS_H

#include "CppUnitTest.h"

#include <tbytevectorlist.h>
#include <tstring.h>
#include <tstringlist.h>

#include <uniquefileidentifierframe.h>
#include <synchronizedLyricsFrame.h>
#include <eventTimingCodesFrame.h>
#include <attachedpictureframe.h>
#include <flacpicture.h>
#include <asfproperties.h>
#include <asfpicture.h>
#include <asfattribute.h>
#include <mp4properties.h>
#include <mp4coverart.h>
#include <xingheader.h>

namespace Microsoft {
  namespace VisualStudio {
    namespace CppUnitTestFramework
    {
      template<> inline std::wstring ToString<unsigned short>(const unsigned short& t) { RETURN_WIDE_STRING(t); }
      template<> inline std::wstring ToString<long long>(const long long& t) { RETURN_WIDE_STRING(t); }

      template<> inline std::wstring ToString<TagLib::ByteVector>(const TagLib::ByteVector& t) 
      { 
        return std::wstring(t.begin(), t.end());
      }

      template<> inline std::wstring ToString<TagLib::String::Type>(const TagLib::String::Type& t)
      {
        switch (t)
        {
        case TagLib::String::Latin1:
          return L"Latin1";
        case TagLib::String::UTF16:
          return L"UTF16";
        case TagLib::String::UTF16BE:
          return L"UTF16BE";
        case TagLib::String::UTF8:
          return L"UTF8";
        case TagLib::String::UTF16LE:
          return L"UTF16LE";
        default:
          return L"<Unknown>";
        }
      }

      template<> inline std::wstring ToString<TagLib::String>(const TagLib::String& t)
      {
        std::wstringstream tmp;
        tmp << L"'" << t.toCWString() << L"'";
        return tmp.str().c_str();
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::UniqueFileIdentifierFrame>(const TagLib::ID3v2::UniqueFileIdentifierFrame* q)
      { 
        if (q == nullptr) 
        {
          return L"<nullptr>";
        }
        else 
        {
          RETURN_WIDE_STRING((long long)q);
        }
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::UniqueFileIdentifierFrame>(TagLib::ID3v2::UniqueFileIdentifierFrame* q)
      {
        if (q == nullptr)
        {
          return L"<nullptr>";
        }
        else
        {
          RETURN_WIDE_STRING((long long)q);
        }
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::SynchronizedLyricsFrame::Type>(const TagLib::ID3v2::SynchronizedLyricsFrame::Type& q)
      {
        switch (q)
        {
        case TagLib::ID3v2::SynchronizedLyricsFrame::Other:
          return L"Other";
        case TagLib::ID3v2::SynchronizedLyricsFrame::Lyrics:
          return L"Lyrics";
        case TagLib::ID3v2::SynchronizedLyricsFrame::TextTranscription:
          return L"TextTranscription";
        case TagLib::ID3v2::SynchronizedLyricsFrame::Movement:
          return L"Movement";
        case TagLib::ID3v2::SynchronizedLyricsFrame::Events:
          return L"Events";
        case TagLib::ID3v2::SynchronizedLyricsFrame::Chord:
          return L"Chord";
        case TagLib::ID3v2::SynchronizedLyricsFrame::Trivia:
          return L"Trivia";
        case TagLib::ID3v2::SynchronizedLyricsFrame::WebpageUrls:
          return L"WebpageUrls";
        case TagLib::ID3v2::SynchronizedLyricsFrame::ImageUrls:
          return L"ImageUrls";
        default:
          return L"Unknown";
        }
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::SynchronizedLyricsFrame::TimestampFormat>(const TagLib::ID3v2::SynchronizedLyricsFrame::TimestampFormat& q)
      {
        switch (q)
        {
        case TagLib::ID3v2::SynchronizedLyricsFrame::Unknown:
          return L"Unknown";
        case TagLib::ID3v2::SynchronizedLyricsFrame::AbsoluteMpegFrames:
          return L"AbsoluteMpegFrames";
        case TagLib::ID3v2::SynchronizedLyricsFrame::AbsoluteMilliseconds:
          return L"AbsoluteMilliseconds";
        default:
          return L"Unknown";
        }
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::EventTimingCodesFrame::TimestampFormat>(const TagLib::ID3v2::EventTimingCodesFrame::TimestampFormat& q)
      {
        switch (q)
        {
        case TagLib::ID3v2::EventTimingCodesFrame::Unknown:
          return L"Unknown";
        case TagLib::ID3v2::EventTimingCodesFrame::AbsoluteMpegFrames:
          return L"AbsoluteMpegFrames";
        case TagLib::ID3v2::EventTimingCodesFrame::AbsoluteMilliseconds:
          return L"AbsoluteMilliseconds";
        default:
          return L"Unknown";
        }
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::EventTimingCodesFrame::EventType>(const TagLib::ID3v2::EventTimingCodesFrame::EventType& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::ID3v2::AttachedPictureFrame::Type>(const TagLib::ID3v2::AttachedPictureFrame::Type& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::FLAC::Picture::Type>(const TagLib::FLAC::Picture::Type& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::ASF::Properties::Codec>(const TagLib::ASF::Properties::Codec& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::ASF::Picture::Type>(const TagLib::ASF::Picture::Type& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::ASF::Attribute::AttributeTypes>(const TagLib::ASF::Attribute::AttributeTypes& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::MP4::Properties::Codec>(const TagLib::MP4::Properties::Codec& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::MP4::CoverArt::Format>(const TagLib::MP4::CoverArt::Format& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::MPEG::XingHeader::HeaderType>(const TagLib::MPEG::XingHeader::HeaderType& q)
      {
        RETURN_WIDE_STRING(q);
      }

      template<> inline std::wstring ToString<TagLib::StringList>(const TagLib::StringList& q)
      {
        std::wstringstream tmp;
        tmp << L"[";
        bool isFirst = true;
        for (const auto& i : q) 
        {
          if (isFirst) 
          {
            isFirst = false;
          }
          else
          {
            tmp << L",";
          }

          tmp << L"'" << i.toCWString() << L"'";
        }
        tmp << L"]";
        return tmp.str().c_str();
      }

    }
  }
}

#endif // _VS_CPP_UNIT_TESTS_UTILS_H