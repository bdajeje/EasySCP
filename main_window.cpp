#include "main_window.hpp"

#include <QApplication>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(const QIcon& icon, QWidget *parent)
  : QMainWindow{parent}
{
  // First validate requirements
  hasCommand("scp");
  hasCommand("sshpass");

  // Central widget - will contains all other views
  _central_widget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout(_central_widget);

  // Read settings
  _settings = std::make_shared<utils::Settings>("./resources/settings");

  // Create system tray icon
  _system_tray_icon = new QSystemTrayIcon(icon, this);
  _system_tray_icon->show();

  // Create views
  _w_choose_file        = new window::ChooseFile;
  _w_choose_target      = new window::ChooseTarget{_settings};
  _w_transfert_progress = new window::TransfertProgress{_settings, _system_tray_icon};

  layout->addWidget(_w_choose_file);
  layout->addWidget(_w_choose_target);
  layout->addWidget(_w_transfert_progress);

  _w_choose_target->hide();
  _w_transfert_progress->hide();

  connect(_w_choose_file, SIGNAL(fileSelected(QString)), this, SLOT(filepathSelected(QString)));
  connect(_w_choose_target, SIGNAL(previousPage()), this, SLOT(showChooseFile()));
  connect(_w_choose_target, SIGNAL(startTransfert()), this, SLOT(startTransfert()));
  connect(_w_transfert_progress, SIGNAL(finished()), this, SLOT(showChooseFile()));

  setCentralWidget(_central_widget);
}

MainWindow::~MainWindow()
{
  delete _w_choose_file;
  delete _w_choose_target;
  delete _w_transfert_progress;
}

void MainWindow::hasCommand(const QString& cmd)
{
  const QString program = "whereis";
  QStringList arguments;
  arguments << cmd;

  QProcess process;
  process.start(program, arguments);
  process.waitForFinished();

  QString output = process.readAllStandardOutput();
  // whereis produces output like: [cmd_name]: [cmd_paths]
  // So if there is not cmd_paths it means the program is not installed
  if(output.length() <= cmd.length() + 2)
  {
    QMessageBox::warning(this, tr("Error"), tr("This program needs '%1'' to run, please install it before using EasySCP.").arg(cmd));
    exit(EXIT_FAILURE);
  }
}

void MainWindow::filepathSelected(QString filepath)
{
  _selected_filepath = filepath;

  _w_choose_file->hide();
  _w_choose_target->show();
}

void MainWindow::showChooseFile()
{
  _w_choose_target->hide();
  _w_transfert_progress->hide();
  _w_choose_file->show();
}

void MainWindow::startTransfert()
{
  _w_choose_target->hide();
  _w_transfert_progress->show();
  _w_transfert_progress->start(_selected_filepath,
                               _w_choose_target->getTargetUserName(),
                               _w_choose_target->getTargetAddress(),
                               _w_choose_target->getTargetDestination(),
                               _w_choose_target->getTargetPassword()
                               );
}
