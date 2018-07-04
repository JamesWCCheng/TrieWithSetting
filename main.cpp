#include "LRUCache.h"
#include "Trie.h"
#include "usersettingapis.h"
#include "utils.h"
#include "simplelogger.h"

#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

using namespace std;

class ISettingHandler
{
public:
  virtual ~ISettingHandler() {}
  using SettingChangedCallbackType = function<void(/*setting*/)>;
  //weak_ptr<>
  virtual void Set() = 0;
  virtual void Get() = 0;
  virtual void Listen(SettingChangedCallbackType) = 0;
  virtual void UnListen() = 0;
};

class CUserType1SettingHandler
    : public ISettingHandler,
      public ISettingChanged<Type1Setting>,
      public enable_shared_from_this<CUserType1SettingHandler>
{
public:
  void Set(/*Object*/) override {}
  /*Object*/ void Get() override {}
  void Listen(SettingChangedCallbackType aCallback) override
  {
    AllSettings::getinstance()->getType1().Register(shared_from_this());
    iCallback = std::move(aCallback);
  }

  void UnListen(/*need callback*/) override
  {
    AllSettings::getinstance()->getType1().UnRegister();
    iCallback = nullptr;
  }

  void OnSettingChanged(Type1Setting aSettingType) override
  {
    // cout << "CUserType1SettingHandler::" << __func__ << endl;
    // Got the changing evnet, but needs to query the value then callback.
    iCallback();
  }

private:
  SettingChangedCallbackType iCallback;
};

class SettingManager
{
public:
  SettingManager()
  {
    // initial Setting Trie Node and insert to SettingTrie.
  }

private:
  vector<shared_ptr<ISettingHandler>> iHandlers;
};

int main()
{
  auto type1handler = make_shared<CUserType1SettingHandler>();
  type1handler->Listen([]() { cout << "Got Callback" << endl; });

  // simulate setting change
  auto f = std::async(std::launch::async, [] {
    bool s = false;
    while (true)
    {
      s = !s;
      AllSettings::getinstance()->getType1().SetType1Setting(s);
      SleepFor(2);
      PR0();
    }
  });

  getchar();
  return 0;
}

// int main()
// {
//   // Test LRU
//   CLRUCache<int, int> q;
//   q.Put(1, 5566);
//   q.Put(2, 7788);
//   std::cout << q.Get(1).first << q.Get(2).first << std::endl;
//   auto qq = q;
//   std::cout << qq.Get(1).first << qq.Get(2).first << std::endl;
//   auto qqq = std::move(q);
//   std::cout << qqq.Get(1).first << qqq.Get(2).first << std::endl;
//   CLRUCache<int, int> q2;
//   CLRUCache<int, int> q3;
//   q2 = qq;
//   q3 = std::move(qq);
//   std::cout << q2.Get(1).first << q2.Get(2).first << std::endl;
//   std::cout << q3.Get(1).first << q3.Get(2).first << std::endl;

//   // Test Trie
//   string input[] =
//       {
//           "A.B.C",
//           "A.C.D.E",
//           "AA.BB.CC",
//           "B.B.Q",
//           "BB",
//       };
//   CTrie trie;
//   cout << "== Dictionary ==" << endl;
//   for (auto s : input)
//   {
//     trie.Insert(s);
//     cout << s << endl;
//   }
//   cout << "=============" << endl;
//   cout << "List All" << endl;
//   const auto &all = trie.Suggest("");
//   for (auto &s : all)
//   {
//     cout << s << endl;
//   }
//   cout << "=============" << endl;

//   cout << "A suggests?" << endl;
//   const auto &result = trie.Suggest("A");
//   for (auto &s : result)
//   {
//     cout << s << endl;
//   }
//   cout << "=============" << endl;
//   cout << "BB suggests?" << endl;
//   const auto &result2 = trie.Suggest("BB");
//   for (auto &s : result2)
//   {
//     cout << s << endl;
//   }
//   cout << "=============" << endl;
//   cout << "B suggests?" << endl;
//   const auto &result3 = trie.Suggest("B");
//   for (auto &s : result3)
//   {
//     cout << s << endl;
//   }
//   cout << "=============" << endl;
//   cout << " suggests?" << endl;
//   const auto &result4 = trie.Suggest("");
//   for (auto &s : result4)
//   {
//     cout << s << endl;
//   }
//   cout << "=============" << endl;
//   cout << "B suggests?" << endl;
//   const auto &result5 = trie.Suggest("B");
//   for (auto &s : result5)
//   {
//     cout << s << endl;
//   }

//   cout << trie.Search("") << endl << trie.Search("A.B.C");
//   getchar();
//   return 0;
// }