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

  cout<<sizeof(long double);
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

  CTrie trie;

  getchar();
  return 0;
}

// int main()
// {
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