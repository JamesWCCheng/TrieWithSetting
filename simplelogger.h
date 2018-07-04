#ifndef __SIMPLELOGGER_H__
#define __SIMPLELOGGER_H__
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifdef _MSC_VER
#  define PRIoSIZE  "Io"
#  define PRIuSIZE  "Iu"
#  define PRIxSIZE  "Ix"
#  define PRIXSIZE  "IX"
#else
#  define PRIoSIZE  "zo"
#  define PRIuSIZE  "zu"
#  define PRIxSIZE  "zx"
#  define PRIXSIZE  "zX"
#endif

#include <iostream>
#include <stdio.h>
#include <utility>
#include <string>
#include <stdint.h>
#include <inttypes.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <functional>
#include <cstring> // strlen
#include <iterator>
// Helper function to implement byte array to hex string API.
namespace {
  template<class Type, class LengthType>
  auto
  ToVector(const Type* aData, LengthType aLength) -> std::vector<Type>
  {
    std::vector<Type> vec;
    std::copy(aData, aData + aLength, std::back_inserter(vec));
    return vec;
  }

  std::string bin2hex(const std::vector<unsigned char> &bin) {
    std::ostringstream oss;
    for (auto b : bin) {
      oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(b);
    }
    return oss.str();
  }

  std::string bin2hex(const std::vector<char> &bin) {
    std::ostringstream oss;
    for (auto b : bin) {
      // signed char cast to unsgined int will fill with FFFFFF, so need to cast to unsigned char first.
      oss << std::setfill('0') << std::setw(2) << std::hex <<static_cast<unsigned int>(*reinterpret_cast<unsigned char*>(&b));
    }
    return oss.str();
  }

  std::string bin2hex(const std::vector<signed char> &bin) {
    std::ostringstream oss;
    for (auto b : bin) {
      // signed char cast to unsgined int will fill with FFFFFF, so need to cast to unsigned char first.
      oss << std::setfill('0') << std::setw(2) << std::hex <<static_cast<unsigned int>(*reinterpret_cast<unsigned char*>(&b));
    }
    return oss.str();
  }

  std::string bin2hex(const unsigned char *bin, size_t len) {
    std::ostringstream oss;
    for (size_t i = 0; i < len; i++) {
      oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(bin[i]);
    }
    return oss.str();
  }

  std::string bin2hex(const char *bin, size_t len) {
    auto forcelyunsigned = reinterpret_cast<const unsigned char*>(bin);
    std::ostringstream oss;
    for (size_t i = 0; i < len; i++) {
      oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(forcelyunsigned[i]);
    }
    return oss.str();
  }

  std::string bin2hex(const signed char *bin, size_t len) {
    auto forcelyunsigned = reinterpret_cast<const unsigned char*>(bin);
    std::ostringstream oss;
    for (size_t i = 0; i < len; i++) {
      oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(forcelyunsigned[i]);
    }
    return oss.str();
  }
}

#define printf printf


#ifdef _MSC_VER
#define EZ_NONE ""
#define EZ_RED ""
#define EZ_LIGHT_RED ""
#define EZ_GREEN ""
#define EZ_LIGHT_GREEN ""
#define EZ_BLUE ""
#define EZ_LIGHT_BLUE ""
#define EZ_DARY_GRAY ""
#define EZ_CYAN ""
#define EZ_LIGHT_CYAN ""
#define EZ_PURPLE ""
#define EZ_LIGHT_PURPLE ""
#define EZ_BROWN ""
#define EZ_YELLOW ""
#define EZ_LIGHT_GRAY ""
#define EZ_WHITE ""

#else
#define EZ_NONE "\033[m"
#define EZ_RED "\033[0;32;31m"
#define EZ_LIGHT_RED "\033[1;31m"
#define EZ_GREEN "\033[0;32;32m"
#define EZ_LIGHT_GREEN "\033[1;32m"
#define EZ_BLUE "\033[0;32;34m"
#define EZ_LIGHT_BLUE "\033[1;34m"
#define EZ_DARY_GRAY "\033[1;30m"
#define EZ_CYAN "\033[0;36m"
#define EZ_LIGHT_CYAN "\033[1;36m"
#define EZ_PURPLE "\033[0;35m"
#define EZ_LIGHT_PURPLE "\033[1;35m"
#define EZ_BROWN "\033[0;33m"
#define EZ_YELLOW "\033[1;33m"
#define EZ_LIGHT_GRAY "\033[0;37m"
#define EZ_WHITE "\033[1;37m"
#endif

static const char* EZ_TAG = "EZLOG";

#define _EXPAND(x) x
#define _EXPAND2(x, y) x, y
#define CONCATENATE(x, y) x##y
#define NARG(...) NARG_(__VA_ARGS__, RSEQ_N())
#define NARG_(...) _EXPAND(ARG_N(__VA_ARGS__))
#define ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
              _11, _12, _13, _14, _15, _16, N, ...) N
#define RSEQ_N() 16, 15, 14, 13, 12, 11, \
                 10, 9, 8, 7, 6, 5, 4, 3, 2, 1

#define EXTEND(...) _EXPAND(EXTEND_(NARG(__VA_ARGS__), __VA_ARGS__))
#define EXTEND_(N, ...) _EXPAND(CONCATENATE(EXTEND_, N)(__VA_ARGS__))
#define EXTEND_1(x, ...) _EXPAND(std::make_pair((x), #x))
#define EXTEND_2(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_1(__VA_ARGS__))
#define EXTEND_3(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_2(__VA_ARGS__))
#define EXTEND_4(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_3(__VA_ARGS__))
#define EXTEND_5(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_4(__VA_ARGS__))
#define EXTEND_6(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_5(__VA_ARGS__))
#define EXTEND_7(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_6(__VA_ARGS__))
#define EXTEND_8(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_7(__VA_ARGS__))
#define EXTEND_9(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_8(__VA_ARGS__))
#define EXTEND_10(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_9(__VA_ARGS__))
#define EXTEND_11(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_10(__VA_ARGS__))
#define EXTEND_12(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_11(__VA_ARGS__))
#define EXTEND_13(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_12(__VA_ARGS__))
#define EXTEND_14(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_13(__VA_ARGS__))
#define EXTEND_15(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_14(__VA_ARGS__))
#define EXTEND_16(x, ...) _EXPAND2(EXTEND_1(x), EXTEND_15(__VA_ARGS__))


namespace {
  template<class T>
  void printInternal(const std::vector<T>& aVec, const char* const aObjName)
  {
    int index = 0;
    for (auto itr = aVec.begin(); itr != aVec.end(); itr++)
    {
      std::ostringstream ss;
      ss << aObjName << "[" << index << "] = " << *itr << "\n";
      printf("%s", ss.str().c_str());
      index++;
    }
  }

  template<>
  void printInternal(const std::vector<uint8_t>& aVec, const char* const aObjName)
  {
    auto hexString = bin2hex(aVec);
    printf("%s = %s, unsgined vector length = %zu", aObjName, hexString.c_str(), aVec.size());
  }

  template<>
  void printInternal(const std::vector<char>& aVec, const char* const aObjName)
  {
    auto hexString = bin2hex(aVec);
    printf("%s = %s, signed vector length = %zu", aObjName, hexString.c_str(), aVec.size());
  }

  template<class Key, class Value>
  void printInternal(const std::map<Key, Value> aMap, const char* const aObjName)
  {
    for (auto& pair : aMap)
    {
      std:: ostringstream ss;
      ss << aObjName << "[" << pair.first << ", " << pair.second << "]\n";
      printf("%s",ss.str().c_str());
    }
  }

  template<class Key, class Value>
  void printInternal(const std::unordered_map<Key, Value> aMap, const char* const aObjName)
  {
    for (auto& pair : aMap)
    {
      std::ostringstream ss;
      ss << aObjName << "[" << pair.first << ", " << pair.second << "]\n";
      printf("%s",ss.str().c_str());
    }
  }

  // For std string
  void printInternal(std::string aStdStr, const char* const aObjName)
  {
    printf("%s = %s", aObjName, aStdStr.c_str());
  }
  // For pointer
  void printInternal(const void* aPtr, const char* const aObjName)
  {
    printf("%s = %p", aObjName, aPtr);
  }
  // For c style string
  void printInternal(const char* aStr, const char* const aObjName)
  {
    printf("%s = %s", aObjName, aStr);
    printf("hex form %s = %s", aObjName, bin2hex(aStr, strlen(aStr)).c_str());
  }
  void printInternal(const unsigned char* aStr, const char* const aObjName)
  {
    printf("%s = %s", aObjName, aStr);
    printf("hex form %s = %s", aObjName, bin2hex(aStr, strlen(reinterpret_cast<const char*>(aStr))).c_str());
  }
  void printInternal(const signed char* aStr, const char* const aObjName)
  {
    printf("%s = %s", aObjName, aStr);
    printf("hex form %s = %s", aObjName, bin2hex(aStr, strlen(reinterpret_cast<const char*>(aStr))).c_str());
  }

  template<class Type>
  typename std::enable_if<!std::is_enum<Type>::value>::type
  printInternal(Type aVal, const char* const aObjName)
  {
    std::cout << aObjName << " = " << aVal;
  }

  template<class Type>
  typename std::enable_if<std::is_enum<Type>::value>::type
  printInternal(Type aEnum, const char* const aObjName)
  {
    std::cout << aObjName << " = " << static_cast<typename std::underlying_type<Type>::type>(aEnum);
  }

  void ezPrint() {
    std::cout << EZ_NONE"\n";
  }

  template<class Type>
  void ezPrint(
    const std::pair<Type, const char *> &arg) {
    printInternal(arg.first, arg.second);
    ezPrint();
  }

  template<class Type, class... Types>
  void ezPrint(
    const std::pair<Type, const char *> &arg,
    const std::pair<Types, const char *> &... args) {
    // first is the real object, second is the object variable name.
    printInternal(arg.first, arg.second);
    std::cout<<", ";
    ezPrint(args...);
  }

  template<class... Types>
  void PInternal(const char* aColor, const char* aFileName, const char* aFuncName, const int aLineNum, Types&&...  aArgs)
  {
    printf("[%s] %s%s::%s(%d) ", EZ_TAG, aColor, aFileName, aFuncName, aLineNum);
    ezPrint(std::forward<Types>(aArgs)...);
  }
} //namespace

#ifdef _MSC_VER
#define FUNC_NAME __FUNCSIG__
#else
#define FUNC_NAME __PRETTY_FUNCTION__
#endif

#define P(...) PInternal("", __FILE__, FUNC_NAME, __LINE__, EXTEND(__VA_ARGS__))
#define PR(...) PInternal(EZ_LIGHT_RED, __FILE__, FUNC_NAME, __LINE__, EXTEND(__VA_ARGS__))
#define PG(...) PInternal(EZ_LIGHT_GREEN, __FILE__, FUNC_NAME, __LINE__, EXTEND(__VA_ARGS__))
#define PB(...) PInternal(EZ_LIGHT_BLUE, __FILE__, FUNC_NAME, __LINE__, EXTEND(__VA_ARGS__))
#define PX(EZ_COLOR, ...) PInternal(EZ_COLOR, __FILE__, FUNC_NAME, __LINE__, EXTEND(__VA_ARGS__))

#define P0(...) PInternal("", __FILE__, FUNC_NAME, __LINE__)
#define PR0(...) PInternal(EZ_LIGHT_RED, __FILE__, FUNC_NAME, __LINE__)
#define PG0(...) PInternal(EZ_LIGHT_GREEN, __FILE__, FUNC_NAME, __LINE__)
#define PB0(...) PInternal(EZ_LIGHT_BLUE, __FILE__, FUNC_NAME, __LINE__)
#define PX0(EZ_COLOR, ...) PInternal(EZ_COLOR, __FILE__, FUNC_NAME, __LINE__)

#endif