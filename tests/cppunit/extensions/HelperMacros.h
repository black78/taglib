#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

// #if WINAPI_FAMILY && (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#include "CppUnitTest.h"
#include "vs_cpp_unit_tests_utils.h"
//#else
//#include "tests_config.h"
//#endif

// #if WINAPI_FAMILY && (WINAPI_FAMILY == WINAPI_FAMILY_PC_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)

#define CPPUNIT_TEST_CLASS(className) TEST_CLASS(className)
#define CPPUNIT_TEST_SUITE_REGISTRATION(className)

#define CPPUNIT_TEST_SUITE(name)
#define CPPUNIT_TEST_SUITE_END()
#define CPPUNIT_TEST(funcName) TEST_METHOD(funcName##_vs_prox) { funcName(); }

#define CPPUNIT_ASSERT(val) Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(val)
#define CPPUNIT_ASSERT_EQUAL(expected, actual) Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(expected, actual)

//#else
//
//#define CPPUNIT_TEST_CLASS(className) CPPUNIT_TEST_CLASS(className)
//
//#endif
