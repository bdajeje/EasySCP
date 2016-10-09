#include "choose_file.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>

#include "utils/file.hpp"
#include "utils/fonts.hpp"

namespace window {

ChooseFile::ChooseFile(std::shared_ptr<utils::Settings>& settings)
  : _settings {settings}
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  QPushButton* browse_button = new QPushButton(tr("Browse"));
  connect(browse_button, SIGNAL(clicked(bool)), this, SLOT(selectFile()));

  QLabel* text_1 = new QLabel(tr("Drag and Drop"));
  QLabel* text_2 = new QLabel(tr("a file to send"));
  QLabel* text_3 = new QLabel(tr("or"));

  text_1->setAlignment(Qt::AlignCenter);
  text_2->setAlignment(Qt::AlignCenter);
  text_3->setAlignment(Qt::AlignCenter);

  text_1->setFont(utils::font::Title);
  text_2->setFont(utils::font::Default);
  text_3->setFont(utils::font::Title);

  layout->addWidget(text_1);
  layout->addWidget(text_2);
  layout->addWidget(text_3);
  layout->addWidget(browse_button);

  setAcceptDrops(true);
}

void ChooseFile::selectFile()
{
  QUrl file_url = QFileDialog::getOpenFileUrl(this, tr("Select a file"), QString{_settings->getLastFileDir().c_str()});
  if(file_url.isEmpty())
    return;

  _settings->setLastFileDir(file_url.path());

  emit fileSelected(file_url.fileName());
}

void ChooseFile::dragEnterEvent(QDragEnterEvent *event)
{
  // Accept event
  event->acceptProposedAction();
}

void ChooseFile::dropEvent(QDropEvent *event)
{
  // Check there is at least one URL in the mimeData
  const QList<QUrl> urls = event->mimeData()->urls();
  if(urls.isEmpty())
    return;

  // Check given URL refers to a file
  const QUrl& url = urls.first();
  const QString& filepath = url.toLocalFile();
  if(!utils::files::exists(filepath.toStdString()))
    return;

  emit fileSelected(filepath);
}

}
