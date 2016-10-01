#include "scp_worker.hpp"

namespace worker {

SCPWorker::SCPWorker(QObject* parent, const QString& filepath, const QString& full_target, const QString& password, uint limit)
  : QThread(parent)
  , _filepath {filepath}
  , _full_target {full_target}
  , _password {password}  
  , _limit {limit}
{}

void SCPWorker::run()
{
  const QString program = "sshpass";
  QStringList arguments;
  arguments<< "-p" << _password << "/usr/bin/scp";

  // Set limit if needed
  if(_limit > 0)
    arguments << "-l" << QString(_limit);

  arguments << _filepath << _full_target;

  _process.reset(new QProcess);

  connect(_process.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
  connect(_process.get(), SIGNAL(readyReadStandardError()), this, SLOT(readProcessError()));
  connect(_process.get(), SIGNAL(finished(int)), this, SIGNAL(finished(int)));

  _process->start(program, arguments);
  _process->waitForFinished();
}

void SCPWorker::stop()
{
  _process->terminate();
}

void SCPWorker::readProcessOutput()
{
  QString read = _process->readAllStandardOutput();
  emit output(read);
}

void SCPWorker::readProcessError()
{
  QString read = _process->readAllStandardError();
  emit error(read);
}

}
