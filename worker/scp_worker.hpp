#ifndef SCPWORKER_HPP
#define SCPWORKER_HPP

#include <memory>

#include <QProcess>
#include <QThread>

namespace worker {

class SCPWorker final : public QThread
{
  Q_OBJECT

  public:

    SCPWorker(QObject* parent, const QString& scp_path, const QString& sshpass_path,
              const QString& filepath, const QString& full_target, const QString& password, uint limit = 0);

    void run() Q_DECL_OVERRIDE;
    void stop();

  private slots:

    void readProcessOutput();
    void readProcessError();

  signals:

    void finished(int);
    void progress(unsigned short);
    void output(QString);
    void error(QString);

  private:

    QString _filepath;
    QString _full_target;
    QString _password;
    uint _limit;
    QString _scp_path;
    QString _sshpass_path;
    std::unique_ptr<QProcess> _process;
};

}

#endif // SCPWORKER_HPP
