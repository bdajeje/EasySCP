#ifndef CHOOSETARGET_HPP
#define CHOOSETARGET_HPP

#include <memory>
#include <vector>

#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>

#include "utils/settings.hpp"

namespace window {

struct LastTarget {

    LastTarget(const QString& target_user, const QString& target_address, const QString& target_path, int speed_limit)
      : _target_user {target_user}
      , _target_address {target_address}
      , _target_path {target_path}
      , _speed_limit {speed_limit}
    {}

    QString _target_user;
    QString _target_address;
    QString _target_path;
    int _speed_limit;
};

class ChooseTarget final : public QWidget
{
  Q_OBJECT

  public:

    explicit ChooseTarget(std::shared_ptr<utils::Settings>& settings);

    QString getTargetUserName() const { return _target_user->text(); }
    QString getTargetAddress() const { return _target_address->text(); }
    QString getTargetDestination() const { return _target_path->text(); }
    QString getTargetPassword() const { return _password_field->text(); }

  private:

    bool isValidInputs() const;
    void loadLastTargets();
    void saveToLastTargets(const QString& target_user, const QString& target_address, const QString& target_path, int speed_limit);
    void addLastTarget(const QString& target_user, const QString& target_address, const QString& target_path, int speed_limit);
    void saveLastTargets();

  signals:

    void previousPage();
    void startTransfert();    

  private slots:

    void inputUpdated();
    void startTransfertPressed();
    void setValueFromLastTarget(QString entry_name);
    void removeCurrentLasttarget();

  private:

    QComboBox* _last_targets_drop_down;
    QPushButton* _remove_last_target;
    QLineEdit* _target_user;
    QLineEdit* _target_address;
    QLineEdit* _target_path;
    QLineEdit* _password_field;
    QSpinBox* _transfert_limit;
    std::vector<QLineEdit*> _fields;
    QPushButton* _start_button;
    std::shared_ptr<utils::Settings> _settings;
    std::map<QString, LastTarget> _last_targets;
};

}

#endif // CHOOSETARGET_HPP
