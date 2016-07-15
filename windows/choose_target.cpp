#include "choose_target.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QDialog>

#include "utils/fonts.hpp"

namespace window {

ChooseTarget::ChooseTarget(std::shared_ptr<utils::Settings>& settings)
  : _settings {settings}
{
  _target_user = new QLineEdit;
  _target_user->setPlaceholderText("User name");
  _target_user->setText(settings->getTargetUserName().c_str());

  _target_address = new QLineEdit;
  _target_address->setPlaceholderText("IP or Computer name");
  _target_address->setText(settings->getTargetAddress().c_str());

  _target_path = new QLineEdit;
  _target_path->setPlaceholderText("Path");
  _target_path->setText(settings->getTargetPath().c_str());

  _password_field = new QLineEdit;
  _password_field->setEchoMode(QLineEdit::Password);
  _password_field->setPlaceholderText("Enter password");

  QPushButton* previous_button = new QPushButton("Previous");
  _start_button = new QPushButton("Transfert");

  QHBoxLayout* button_layout = new QHBoxLayout;
  button_layout->addWidget(previous_button);
  button_layout->addWidget(_start_button);

  QLabel* _intro_text = new QLabel("Choose a target");
  _intro_text->setAlignment(Qt::AlignCenter);
  _intro_text->setFont(utils::font::Title);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(_intro_text);
  layout->addWidget(_target_user);
  layout->addWidget(_target_address);
  layout->addWidget(_target_path);
  layout->addWidget(_password_field);
  layout->addLayout(button_layout);

  connect(previous_button, SIGNAL(pressed()),            this, SIGNAL(previousPage()));
  connect(_start_button,   SIGNAL(pressed()),            this, SLOT(startTransfertPressed()));
  connect(_target_user,    SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));
  connect(_target_address, SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));
  connect(_password_field, SIGNAL(textChanged(QString)), this, SLOT(inputUpdated()));

  inputUpdated();
}

void ChooseTarget::inputUpdated()
{
  const bool all_field_filled = !getTargetAddress().isEmpty() && !getTargetUserName().isEmpty() && !getTargetPassword().isEmpty();
  _start_button->setDisabled(!all_field_filled);
}

void ChooseTarget::startTransfertPressed()
{
  // Register input values so next time user uses the app, those values will be displayed by default
  _settings->setTargetUserName( getTargetUserName() );
  _settings->setTargetAddress( getTargetAddress() );
  _settings->setTargetPath( getTargetDestination() );
  _settings->save();

  // Start transfert
  emit startTransfert();
}

}
