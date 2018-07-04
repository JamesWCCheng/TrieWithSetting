#ifndef TRIE_H
#define TRIE_H

#include "usersettingapis.h"
#include <deque>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>

struct Exception
{
  std::stack<int> a;
  std::string message;
  Exception(std::string msg) : message(std::move(msg)) {}
};

class Value
{
public:
  template <class T>
  static Value create(T value) { return Value(); }

  Value() = default;

  Value(bool value) : value_(std::make_shared<BooleanValue>(value)) {}

  Value(int value) : value_(std::make_shared<IntegerValue>(value)) {}

  Value(std::string value)
      : value_(std::make_shared<StringValue>(std::move(value))) {}

  bool toBoolean() const { return value_->toBoolean(); }

  int toInteger() const { return value_->toInteger(); }

  template <class Enum>
  Enum toEnum() const
  {
    return Enum(value_->toInteger());
  }

  std::string toString() const { return value_->toString(); }

private:
  class BaseValue
  {
  public:
    virtual bool toBoolean() const { throw Exception("Not boolean"); }

    virtual int toInteger() const { throw Exception("Not integer"); }

    template <class Enum>
    Enum toEnum() const { return Enum(toInteger()); }

    virtual std::string toString() const { throw Exception("Not string"); }
  }; // class BaseValue

  class BooleanValue : public BaseValue
  {
  public:
    BooleanValue(bool value) : value_(value) {}

    bool toBoolean() const override { return value_; }

  private:
    bool value_;
  }; // class BooleanValue

  class IntegerValue : public BaseValue
  {
  public:
    IntegerValue(int value) : value_(value) {}

    int toInteger() const override { return value_; }

  private:
    int value_;
  }; // class IntegerValue

  class StringValue : public BaseValue
  {
  public:
    StringValue(std::string value) : value_(std::move(value)) {}

    std::string toString() const override { return value_; }

  private:
    std::string value_;
  }; // class StringValue

  std::shared_ptr<BaseValue> value_;
}; // class Value

class Settings
{
public:
  template <class T>
  void set(const std::string &key, T value)
  {
    settings_[key] = typename Cast<T>::type(std::move(value));
  }

  Value get(const std::string &key) const
  {
    auto itr = settings_.find(key);
    if (itr == settings_.end())
    {
      throw Exception("No key " + key);
    }
    return itr->second;
  }

private:
  template <class T, bool = std::is_same<T, bool>::value,
            bool = std::is_integral<T>::value || std::is_enum<T>::value,
            bool = std::is_same<T, std::string>::value ||
                   std::is_same<T, char *>::value ||
                   std::is_same<T, const char *>::value>
  struct Cast;

  template <class T>
  struct Cast<T, true, true, false>
  {
    typedef bool type;
  };

  template <class T>
  struct Cast<T, false, true, false>
  {
    typedef int type;
  };

  template <class T>
  struct Cast<T, false, false, true>
  {
    typedef std::string type;
  };

  std::unordered_map<std::string, Value> settings_;
}; // class Settings

struct CTrieNode
{
  using NodeValueType = char;
  CTrieNode() : iIsWord(false) {}

  bool iIsWord;
  std::unordered_map<NodeValueType, std::unique_ptr<CTrieNode>> iChildren;
};

struct CTrie
{
  CTrie()
      : // iRoot(std::make_unique<CTrieNode>())
        iRoot(new CTrieNode())
  {
  }

  void Insert(const std::string &aPrefix)
  {
    auto current = iRoot.get();
    for (char c : aPrefix)
    {
      // std::pair<iterator,bool>, be true when the insertion took place.
      auto check = current->iChildren.emplace(c, nullptr);
      if (check.second)
      {
        current->iChildren[c].reset(new CTrieNode());
      }
      current = current->iChildren[c].get();
    }
    current->iIsWord = true;
  }

  void Insert(const std::vector<std::string> &aPrefixVec)
  {
    for (const auto &pre : aPrefixVec)
    {
      Insert(pre);
    }
  }

  bool Search(const std::string &aKey)
  {
    auto current = iRoot.get();
    for (char c : aKey)
    {
      if (current == nullptr ||
          current->iChildren.find(c) == current->iChildren.end())
      {
        return false;
      }
      current = current->iChildren[c].get();
    }
    return current->iIsWord;
  }

  std::vector<std::string> Suggest(const std::string &aPrefix)
  {
    std::queue<std::pair<std::string, CTrieNode *>> q;
    auto current = iRoot.get();
    if (aPrefix.empty())
    {
      // push all the starting character as the root of BFS.
      for (auto &trieNode : current->iChildren)
      {
        q.push({ std::string(1, trieNode.first), trieNode.second.get()});
      }
    }
    else
    {
      // traverse to the last valid word starting from aPrefix.
      std::string validPrefix;
      for (char c : aPrefix)
      {
        if (current->iChildren.find(c) == current->iChildren.end())
        {
          break;
        }
        validPrefix.push_back(c);
        current = current->iChildren[c].get();
      }
      std::cout << "valid prefix = " << validPrefix << std::endl;
      // Cannot find any words starting with aPrefix.
      if (validPrefix.empty())
      {
        return std::vector<std::string>{};
      }

      q.push({validPrefix, current});
    }
    std::vector<std::string> result;
    // BFS
    while (!q.empty())
    {
      auto p = q.front();
      q.pop();
      if (p.second->iIsWord)
      {
        result.push_back(p.first);
      }

      for (auto &trieNode : p.second->iChildren)
      {
        q.push({p.first + trieNode.first, trieNode.second.get()});
      }
    }

    return result;
  }

private:
  std::unique_ptr<CTrieNode> iRoot;
};

#endif