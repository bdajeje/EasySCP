#include "scp.hpp"

namespace utils{
namespace scp {

QString fullAddress(const QString& username, const QString& address, const QString& path)
{
  return username + "@" + address + ":" + path;
}

}
}
