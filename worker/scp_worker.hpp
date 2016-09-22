#ifndef SCPWORKER_HPP
#define SCPWORKER_HPP

#include <QProcess>
#include <QThread>

namespace worker {

class SCPWorker final : public QThread
{
  Q_OBJECT

  public:

    SCPWorker(QObject* parent, const QString& filepath, const QString& full_target, const QString& password);
	~SCPWorker();

    void run() Q_DECL_OVERRIDE;

  private slots:

	void readProcessOutput();
	void startProcess();

  signals:

	void finished(int);
    void progress(unsigned short);

  private:

    QString _filepath;
    QString _full_target;
    QString _password;
	QProcess* _process {nullptr};
};

}

#endif // SCPWORKER_HPP
