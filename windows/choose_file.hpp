#ifndef CHOOSEFILE_HPP
#define CHOOSEFILE_HPP

#include <QWidget>
#include <QPushButton>

namespace window {

class ChooseFile final : public QWidget
{
  Q_OBJECT

  public:

    ChooseFile();

  private:

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

  private slots:

    void selectFile();

  signals:

    void fileSelected(QString filepath);
};

}

#endif // CHOOSEFILE_HPP
