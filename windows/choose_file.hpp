#ifndef CHOOSEFILE_HPP
#define CHOOSEFILE_HPP

#include <memory>

#include <QWidget>
#include <QPushButton>

#include "utils/settings.hpp"

namespace window {

class ChooseFile final : public QWidget
{
  Q_OBJECT

  public:

    ChooseFile(std::shared_ptr<utils::Settings>& settings);

  private:

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

  private slots:

    void selectFile();

  signals:

    void fileSelected(QString filepath);

  private:

    std::shared_ptr<utils::Settings> _settings;
};

}

#endif // CHOOSEFILE_HPP
