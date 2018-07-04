#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>

#define FUNC_NAME __PRETTY_FUNCTION__

#define PrintTID(...) \
  Utils::PrintTid(__FILE__, FUNC_NAME, __LINE__, ##__VA_ARGS__);
#define SleepFor(SEC) \
  Utils::Sleep(SEC, __FILE__, FUNC_NAME, __LINE__);
namespace Utils
{
    void PrintTid(const char* file,
                  const char* funcname,
                  int line,
                  const char* const msg = nullptr)
    {
      std::thread::id this_id = std::this_thread::get_id();
      std::stringstream stream;
      stream << "[" << file << "]" << funcname << "(" << line << ") TID " <<  this_id <<": " << msg << std::endl;
      std::cout << stream.str() << std::endl;
    }

    template<class... Args>
    void Sleep(int second, Args&&... args)
    {
      std::string msg = "start sleep for " + std::to_string(second) + " sec";
      PrintTid(std::forward<Args>(args)..., msg.c_str());
      std::this_thread::sleep_for(std::chrono::seconds(second));
      PrintTid(std::forward<Args>(args)..., "stop sleep");
    }
}