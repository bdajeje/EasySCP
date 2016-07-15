#ifndef TRANSFERTPROGRESS_HPP
#define TRANSFERTPROGRESS_HPP

#include <memory>

#include <QMainWindow>
#include <QProgressBar>
#include <QWidget>
#include <QCheckBox>
#include <QSystemTrayIcon>

#include "utils/settings.hpp"
#include "worker/scp_worker.hpp"

namespace window {

class TransfertProgress final : public QWidget
{
  Q_OBJECT

  public:

    explicit TransfertProgress(std::shared_ptr<utils::Settings>& settings, QSystemTrayIcon* system_tray);
    ~TransfertProgress();

    void start(const QString& filepath, const QString& target_username, const QString& target_address, const QString& target_destination, const QString& password);

  public slots:

    void transfertFinished();
    void transfertProgress(unsigned short percentage);

  private:

    std::shared_ptr<utils::Settings> _settings;
    QSystemTrayIcon* _system_tray;
    QProgressBar* _progress_bar;
    QCheckBox* _notify_checkbox;
    QString _filepath;

    std::unique_ptr<worker::SCPWorker> _scp_worker;
};

}

#endif // TRANSFERTPROGRESS_HPP
