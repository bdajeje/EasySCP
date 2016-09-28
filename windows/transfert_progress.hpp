#ifndef TRANSFERTPROGRESS_HPP
#define TRANSFERTPROGRESS_HPP

#include <memory>

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
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

    void transfertFinished(int return_code);
    void transfertProgress(unsigned short percentage);
    void readProgressOutput(QString);
    void readProgressError(QString);

  private slots:

    void cancelPressed();
    void notifyCheckUpdate();

  signals:

    void finished();

  private:

    std::shared_ptr<utils::Settings> _settings;
    QSystemTrayIcon* _system_tray;
    QProgressBar* _progress_bar;
    QCheckBox* _notify_checkbox;
    QPushButton* _cancel_button;
    QLabel* _status_text;
    QString _filepath;
    QString _error_msg;
    QString _output_msg;    
    std::unique_ptr<worker::SCPWorker> _scp_worker;
    bool _finished;
    bool _force_stop {false};
};

}

#endif // TRANSFERTPROGRESS_HPP
