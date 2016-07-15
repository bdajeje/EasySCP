#include "scp_worker.hpp"

// \todo: remove those two lines
#include <thread>
#include <iostream>

#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>

namespace worker {

SCPWorker::SCPWorker(QObject* parent, const QString& filepath, const QString& full_target, const QString& password)
  : QThread(parent)
  , _filepath {filepath}
  , _full_target {full_target}
  , _password {password}
{}

void SCPWorker::run()
{
  namespace bp = ::boost::process;

  std::string exec = "echo nani";

  std::vector<std::string> args;
  args.push_back("--version");

  bp::context ctx;
  ctx.stdout_behavior = bp::capture_stream();

  bp::child child_process = bp::launch(exec, args, ctx);

  bp::pistream &is = child_process.get_stdout();
  std::string line;
  while(std::getline(is, line))
    std::cout << line << std::endl;

//  emit progress(10);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(20);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(30);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(40);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(60);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(80);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  emit progress(90);
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  emit finished();
}

}
