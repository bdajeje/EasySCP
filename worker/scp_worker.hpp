#ifndef SCPWORKER_HPP
#define SCPWORKER_HPP

#include <QThread>

namespace worker {

class SCPWorker final : public QThread
{
  Q_OBJECT

  public:

    SCPWorker(QObject* parent, const QString& filepath, const QString& full_target, const QString& password);

    void run() Q_DECL_OVERRIDE;

  signals:

    void finished();
    void progress(unsigned short);

  private:

    QString _filepath;
    QString _full_target;
    QString _password;
};

}

#endif // SCPWORKER_HPP
