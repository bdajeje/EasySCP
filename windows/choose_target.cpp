#include "choose_target.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDialog>

#include <boost/algorithm/string.hpp>

#include "utils/fonts.hpp"
#include "utils/file.hpp"

namespace window {

static const char* LAST_TARGET_FILE = "./resources/last_targets";

ChooseTarget::ChooseTarget(std::shared_ptr<utils::Settings>& settings)
  : _settings {settings}
{
  _last_targets_drop_down = new QComboBox;

  _target_user = new QLineEdit;
  _target_user->setPlaceholderText(tr("User name"));
  _target_user->setText(settings->getTargetUserName().c_str());

  _target_address = new QLineEdit;
  _target_address->setPlaceholderText(tr("IP or Computer name"));
  _target_address->setText(settings->getTargetAddress().c_str());

  _target_path = new QLineEdit;
  _target_path->setPlaceholderText(tr("Path"));
  _target_path->setText(settings->getTargetPath().c_str());

  _password_field = new QLineEdit;
  _password_field->setEchoMode(QLineEdit::Password);
  _password_field->setPlaceholderText(tr("Enter password"));

  _transfert_limit = new QSpinBox;
  _transfert_limit->setPrefix(tr("Speed limit Kb/s: "));
  _transfert_limit->setMaximum(999999999);

  QPushButton* previous_button = new QPushButton(tr("Previous"));
  _start_button = new QPushButton(tr("Transfert"));

  QHBoxLayout* button_layout = new QHBoxLayout;
  button_layout->addWidget(previous_button);
  button_layout->addWidget(_start_button);

  QLabel* _intro_text = new QLabel(tr("Receiver information"));
  _intro_text->setAlignment(Qt::AlignCenter);
  _intro_text->setFont(utils::font::Title);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(_intro_text);
  layout->addWidget(_last_targets_drop_down);
  layout->addWidget(_target_user);
  layout->addWidget(_target_address);
  layout->addWidget(_target_path);
  layout->addWidget(_password_field);
  layout->addWidget(_transfert_limit);
  layout->addLayout(button_layout);

  _fields.reserve(5);
  _fields.emplace_back(_target_user);
  _fields.emplace_back(_target_address);
  _fields.emplace_back(_target_path);
  _fields.emplace_back(_password_field);

  connect(previous_button, SIGNAL(pressed()),            this, SIGNAL(previousPage()));
  connect(_start_button,   SIGNAL(pressed()),            this, SLOT(startTransfertPressed()));
  connect(_target_user,    SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));
  connect(_target_address, SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));
  connect(_password_field, SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));

  connect(_target_user,    SIGNAL(returnPressed()),  this, SLOT(startTransfertPressed()));
  connect(_target_address, SIGNAL(returnPressed()),  this, SLOT(startTransfertPressed()));
  connect(_target_path,    SIGNAL(returnPressed()),  this, SLOT(startTransfertPressed()));
  connect(_password_field, SIGNAL(returnPressed()),  this, SLOT(startTransfertPressed()));
  connect(_last_targets_drop_down, SIGNAL(currentIndexChanged(QString)), this, SLOT(setValueFromLastTarget(QString)));

  inputUpdated();
  loadLastTargets();
}

void ChooseTarget::loadLastTargets()
{
  _last_targets_drop_down->addItem("");

  std::string content;
  if(!utils::files::read(LAST_TARGET_FILE, content))
    return;

  std::vector<std::string> lines;
  boost::algorithm::split(lines, content, boost::is_any_of("\n"));

  std::vector<std::string> parts;
  parts.reserve(_fields.size() + 1);
  for(const std::string& line : lines)
  {
    parts.clear();
    boost::algorithm::split(parts, line, boost::is_any_of("|"));

    if(parts.size() < 4 || parts[0].empty())
      continue;

    const std::string& key = parts[0];
    int speed_limit {0};
    try {
      speed_limit = std::stoi(parts[3]);
    }
    catch(...)
    {}

    addLastTarget(key.c_str(), QString(parts[1].c_str()), QString(parts[2].c_str()), speed_limit);
  }
}

void ChooseTarget::setValueFromLastTarget(QString entry_name)
{
  auto last_target_it = _last_targets.find(entry_name);
  if(last_target_it == _last_targets.end())
    return;

  const LastTarget& last_target = last_target_it->second;
  _target_user->setText(last_target._target_user);
  _target_address->setText(last_target._target_address);
  _target_path->setText(last_target._target_path);
  _transfert_limit->setValue(last_target._speed_limit);

  // Focus for empty fields (better UX)
  for(QLineEdit* line_edit : _fields)
  {
    if(line_edit->text().isEmpty())
    {
      line_edit->setFocus();
      break;
    }
  }
}

bool ChooseTarget::isValidInputs() const
{
  return !getTargetAddress().isEmpty() && !getTargetUserName().isEmpty() && !getTargetPassword().isEmpty();
}

void ChooseTarget::inputUpdated()
{
  _start_button->setDisabled(!isValidInputs());
}

void ChooseTarget::startTransfertPressed()
{
  if(!isValidInputs())
    return;

  const QString user_name = getTargetUserName();
  const QString address = getTargetAddress();
  const QString destination = getTargetDestination();
  const int limit = _transfert_limit->value();

  // Register input values so next time user uses the app, those values will be displayed by default
  _settings->setTargetUserName( user_name );
  _settings->setTargetAddress( address );
  _settings->setTargetPath( destination );
  _settings->setTransfertLimit( limit );
  _settings->save();

  saveToLastTargets(user_name, address, destination, limit);

  // Start transfert
  emit startTransfert();
}

void ChooseTarget::addLastTarget(const QString& target_user, const QString& target_address, const QString& target_path, int speed_limit)
{
  _last_targets.emplace(std::make_pair(target_user, LastTarget(target_user, target_address, target_path, speed_limit)));
  _last_targets_drop_down->addItem(target_user);
}

void ChooseTarget::saveToLastTargets(const QString& target_user, const QString& target_address, const QString& target_path, int speed_limit)
{
  // Check if already in last targets
  auto it = std::find_if(_last_targets.begin(), _last_targets.end(), [&](const std::pair<QString, LastTarget>& last_target_pair) {
    const LastTarget& last_target = last_target_pair.second;
    return last_target._target_user    == target_user &&
           last_target._target_address == target_address &&
           last_target._target_path    == target_path &&
           last_target._speed_limit    == speed_limit;
  });

  if(it != _last_targets.end())
    return;

  addLastTarget(target_user, target_address, target_path, speed_limit);

  std::string content;
  if(!utils::files::read(LAST_TARGET_FILE, content))
    return;

  content += target_user.toStdString()    + "|"
           + target_address.toStdString() + "|"
           + target_path.toStdString()    + "|"
           + std::to_string(speed_limit) + "\n";
  utils::files::create(LAST_TARGET_FILE, content, true);
}

}
