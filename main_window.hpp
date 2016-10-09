#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "utils/settings.hpp"
#include "windows/choose_file.hpp"
#include "windows/choose_target.hpp"
#include "windows/transfert_progress.hpp"

class MainWindow final : public QMainWindow
{
  Q_OBJECT

  public:

    MainWindow(const QIcon& icon, QWidget *parent = 0);
    ~MainWindow();

    QSystemTrayIcon* systemTray() { return _system_tray_icon; }

  private slots:

    void filepathSelected(QString filepath);
    void showChooseFile();
    void startTransfert();

  private:

    std::shared_ptr<utils::Settings> _settings;
    QString _selected_filepath;
    QSystemTrayIcon* _system_tray_icon;

    QWidget* _central_widget;
    window::ChooseFile* _w_choose_file;
    window::ChooseTarget* _w_choose_target;
    window::TransfertProgress* _w_transfert_progress;
};

#endif // MAIN_WINDOW_HPP
