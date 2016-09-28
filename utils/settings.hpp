#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <map>

#include <QString>

namespace utils {

class Settings final
{
  public:

    Settings(const QString& filepath);

    bool getNotifyMe() const { return _settings.at(key_notify_me) == "yes";  }
    const std::string& getTargetUserName() const { return _settings.at(key_target_username); }
    const std::string& getTargetAddress() const { return _settings.at(key_target_address); }
    const std::string& getTargetPath() const { return _settings.at(key_target_path); }

    void setNotifyMe(bool value) { _settings[key_notify_me] = value ? "yes" : "no"; }
    void setTargetUserName(QString value) { _settings[key_target_username] = value.toStdString(); }
    void setTargetAddress(QString value) { _settings[key_target_address] = value.toStdString(); }
    void setTargetPath(QString value) { _settings[key_target_path] = value.toStdString(); }
    void setTransfertLimit(int value) { _settings[key_tranfert_limit] = std::to_string(value); }

    bool save() const;

  private:

    void readSettings(const QString& filepath);

  private:

    const QString _filepath;
    std::map<std::string, std::string> _settings;

    static const std::string key_notify_me;
    static const std::string key_target_username;
    static const std::string key_target_address;
    static const std::string key_target_path;
    static const std::string key_tranfert_limit;
    static const std::array<std::string, 5> SettingKeys;
};

}

#endif // SETTINGS_HPP
