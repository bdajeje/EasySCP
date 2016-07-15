#ifndef CHOOSETARGET_HPP
#define CHOOSETARGET_HPP

#include <memory>

#include <QLineEdit>
#include <QWidget>
#include <QPushButton>

#include "utils/settings.hpp"

namespace window {

class ChooseTarget final : public QWidget
{
  Q_OBJECT

  public:

    explicit ChooseTarget(std::shared_ptr<utils::Settings>& settings);

    QString getTargetUserName() const { return _target_user->text(); }
    QString getTargetAddress() const { return _target_address->text(); }
    QString getTargetDestination() const { return _target_path->text(); }
    QString getTargetPassword() const { return _password_field->text(); }

  signals:

    void previousPage();
    void startTransfert();

  private slots:

    void inputUpdated();
    void startTransfertPressed();

  private:

    QLineEdit* _target_user;
    QLineEdit* _target_address;
    QLineEdit* _target_path;
    QLineEdit* _password_field;
    QPushButton* _start_button;
    std::shared_ptr<utils::Settings> _settings;
};

}

#endif // CHOOSETARGET_HPP
