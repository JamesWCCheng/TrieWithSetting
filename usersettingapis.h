#ifndef USERSETTINGAPIS_H
#define USERSETTINGAPIS_H

#include <string>
#include <memory>
#include <map>
template<class SettingType>
class ISettingChanged
{
 public:
  virtual void OnSettingChanged(SettingType aSettingType) = 0;
};

// simply allowed one callback being registered.
template<class SettingType>
class NotifierBase
{
public:
  void Register(std::shared_ptr<ISettingChanged<SettingType>> aCallback)
  {
    iCallback = std::move(aCallback);
  }
  void UnRegister()
  {
    iCallback = nullptr;
  }
  void Notify(SettingType aSettingType)
  {
    if (iCallback)
    {
      iCallback->OnSettingChanged(aSettingType);
    }
  }
private:
  std::shared_ptr<ISettingChanged<SettingType>> iCallback;
};
// Current design setting architecture.
enum Type1Setting
{
  BOOL
};
class UserType1Setting : public NotifierBase<Type1Setting>
{
public:
  bool GetType1Setting()
  {
    return iSetting;
  }
  void SetType1Setting(bool aSetting)
  {
    iSetting = aSetting;
    Notify(BOOL);
  }
  bool iSetting;
};

enum Type2Setting
{
  INT
};
class UserType2Setting : public NotifierBase<Type2Setting>
{
public:
  int GetType2Setting()
  {
    return iSetting;
  }
  void SetType2Setting(int aSetting)
  {
    iSetting = aSetting;
    Notify(INT);
  }
  int iSetting;
};

enum Type3Setting
{
  STRING, INTEGER
};
class UserType3Setting : public NotifierBase<Type3Setting>
{
public:
  void SetType3StringSetting(const std::string& aSetting)
  {
    iStringSetting = aSetting;
    Notify(STRING);
  }
  std::string GetType3StringSetting()
  {
    return iStringSetting;
  }
  void SetType3IntSetting(int aSetting)
  {
    iIntSetting = aSetting;
    Notify(INTEGER);
  }
  int GetType3IntSetting()
  {
    return iIntSetting;
  }
private:
  int iIntSetting;
  std::string iStringSetting;
};

class AllSettings
{
public:
  static AllSettings* getinstance()
  {
    static AllSettings s;
    return &s;
  }
  UserType1Setting& getType1()
  {
    return s1;
  }
  UserType2Setting& getType2()
  {
    return s2;
  }
  UserType3Setting& getType3()
  {
    return s3;
  }

private:
  UserType1Setting s1;
  UserType2Setting s2;
  UserType3Setting s3;
  AllSettings() = default;
};
#endif // USERSETTINGAPIS_H