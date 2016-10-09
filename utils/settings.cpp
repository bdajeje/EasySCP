#include "settings.hpp"

#include <boost/algorithm/string.hpp>

#include "utils/file.hpp"

namespace utils {

const std::string Settings::key_notify_me {"notify_me"};
const std::string Settings::key_target_username {"target_username"};
const std::string Settings::key_target_address {"target_address"};
const std::string Settings::key_target_path {"target_destination"};
const std::string Settings::key_tranfert_limit {"tranfert_limit"};
const std::string Settings::key_last_file_dir {"last_file_dir"};

const std::array<std::string, 6> Settings::SettingKeys {
  Settings::key_notify_me,
  Settings::key_target_username,
  Settings::key_target_address,
  Settings::key_target_path,
  Settings::key_tranfert_limit,
  Settings::key_last_file_dir
};

Settings::Settings(const QString& filepath)
  : _filepath {filepath}
{
  readSettings(filepath);
}

void Settings::readSettings(const QString& filepath)
{
  // Read settings file
  std::string content;
  if(!utils::files::read(filepath.toStdString(), content))
    return;

  // Split settings content by line
  std::vector<std::string> lines;
  boost::algorithm::split(lines, content, boost::is_any_of("\n"));

  // Read every setting line and update setting variables
  for(const std::string& line : lines)
  {
    // Split line in two parts, key and value, seperated by '='
    const size_t delimiter_offset = line.find("=");
    if(delimiter_offset == std::string::npos)
      continue;

    // Is this a known setting key?
    const std::string setting_key = line.substr(0, delimiter_offset);
    const auto last = std::end(SettingKeys);
    if(std::find(std::begin(SettingKeys), last, setting_key) == last)
      continue;

    // Register found setting
    const std::string setting_value = line.substr(delimiter_offset + 1);
    _settings[setting_key] = setting_value;
  }
}

bool Settings::save() const
{
  std::string content;
  for(const std::string& key : SettingKeys)
    content += key + "=" + _settings.at(key) + "\n";

  return utils::files::create(_filepath.toStdString(), content, true);
}

}
