#include "transfert_progress.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "main_window.hpp"
#include "utils/file.hpp"
#include "utils/fonts.hpp"
#include "utils/scp.hpp"

namespace window {

TransfertProgress::TransfertProgress(std::shared_ptr<utils::Settings>& settings, QSystemTrayIcon* system_tray)
  : _settings{settings}
  , _system_tray{system_tray}
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  QLabel* status_text = new QLabel("Status");
  status_text->setFont(utils::font::Title);
  status_text->setAlignment(Qt::AlignCenter);
  layout->addWidget(status_text);

  _progress_bar = new QProgressBar;
  _progress_bar->setMinimum(0);
  _progress_bar->setMaximum(100);
  _progress_bar->setValue(0);
  layout->addWidget(_progress_bar);

  _notify_checkbox = new QCheckBox("Notify me when transfert is finished");
  layout->addWidget(_notify_checkbox);
  _notify_checkbox->setChecked(settings->getNotifyMe());

  QPushButton* cancel_button = new QPushButton("Cancel");
  layout->addWidget(cancel_button);
}

TransfertProgress::~TransfertProgress()
{
  if(_scp_worker)
  {
    _scp_worker->quit();
    _scp_worker->wait();
  }
}

void TransfertProgress::start(const QString& filepath, const QString& target_username, const QString& target_address, const QString& target_destination, const QString& password)
{
  _filepath = filepath;

  // First save settings
  _settings->setNotifyMe( _notify_checkbox->isChecked() );
  _settings->save();

  // Start scp worker thread
  _scp_worker.reset( new worker::SCPWorker(this, filepath, utils::scp::fullAddress(target_username, target_address, target_destination), password) );
  connect(_scp_worker.get(), SIGNAL(progress(unsigned short)), this, SLOT(transfertProgress(unsigned short)));
  connect(_scp_worker.get(), SIGNAL(finished()), this, SLOT(transfertFinished()));
  _scp_worker->start();
}

void TransfertProgress::transfertProgress(unsigned short percentage)
{
  _progress_bar->setValue(percentage);
}

void TransfertProgress::transfertFinished()
{
  _progress_bar->setValue(100);

  // Show system tray message if needed
  if( _notify_checkbox->isChecked() )
    _system_tray->showMessage("Transfert completed", utils::files::extractName(_filepath.toStdString()).c_str(), QSystemTrayIcon::Information, 2000);
}

}
