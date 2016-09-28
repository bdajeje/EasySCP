#include "transfert_progress.hpp"

#include <QVBoxLayout>
#include <QMessageBox>

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
  _status_text = new QLabel("Tranfering");
  _status_text->setFont(utils::font::Title);
  _status_text->setAlignment(Qt::AlignCenter);
  layout->addWidget(_status_text);

  _progress_bar = new QProgressBar;
  _progress_bar->setMinimum(0);
  // Because tranfert progression feature is not yet implemented
  // set maximum and minimum both equal to 0 so the progress bar
  // shows a nice 'in progress' status
    _progress_bar->setMaximum(0);
//  _progress_bar->setMaximum(100);
  _progress_bar->setValue(0);
  layout->addWidget(_progress_bar);

  _notify_checkbox = new QCheckBox("Notify me when transfert is finished");
  layout->addWidget(_notify_checkbox);
  _notify_checkbox->setChecked(settings->getNotifyMe());
  connect(_notify_checkbox, SIGNAL(clicked(bool)), SLOT(notifyCheckUpdate()));

  _cancel_button = new QPushButton("Cancel");
  layout->addWidget(_cancel_button);

  connect(_cancel_button, SIGNAL(pressed()), SLOT(cancelPressed()));
}

TransfertProgress::~TransfertProgress()
{
  if(_scp_worker)
  {
    _scp_worker->stop();
    _scp_worker->quit();
    _scp_worker->wait();
  }
}

void TransfertProgress::notifyCheckUpdate()
{
  // Save settings
  _settings->setNotifyMe( _notify_checkbox->isChecked() );
  _settings->save();
}

void TransfertProgress::start(const QString& filepath, const QString& target_username, const QString& target_address, const QString& target_destination, const QString& password)
{
  _finished = false;
  _progress_bar->setValue(0);
  _filepath = filepath;  

  _scp_worker.reset( new worker::SCPWorker(this, filepath, utils::scp::fullAddress(target_username, target_address, target_destination), password) );
  connect(_scp_worker.get(), SIGNAL(output(QString)), this, SLOT(readProgressOutput(QString)));
  connect(_scp_worker.get(), SIGNAL(error(QString)), this, SLOT(readProgressError(QString)));
  connect(_scp_worker.get(), SIGNAL(finished(int)), this, SLOT(transfertFinished(int)));

  _scp_worker->start();
}

void TransfertProgress::readProgressOutput(QString output)
{
  _output_msg += output;
}

void TransfertProgress::readProgressError(QString error)
{
  _error_msg += error;
}

void TransfertProgress::transfertProgress(unsigned short percentage)
{
  _progress_bar->setValue(percentage);
}

void TransfertProgress::transfertFinished(int return_code)
{
  _finished = true;

  if(_force_stop)
    return;

  // \todo: delete this line:
  // Because tranfert progression feature is not yet implemented
  // we need to set a real max value
  _progress_bar->setMaximum(100);

  _progress_bar->setValue(100);

  if(return_code != 0)
  {
    QMessageBox::warning(this, "Transfert error", "Error: " + _error_msg);
    return;
  }

  // Show system tray message if needed
  if( _notify_checkbox->isChecked() )
    _system_tray->showMessage("Transfert completed", utils::files::extractName(_filepath.toStdString()).c_str(), QSystemTrayIcon::Information, 2000);

  _status_text->setText("Transfert complete.");
  _cancel_button->setText("Return");
}

void TransfertProgress::cancelPressed()
{
  if(!_finished)
  {
    _force_stop = true;
    _scp_worker->stop();
  }

  emit finished();
}

}
