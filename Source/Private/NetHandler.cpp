#include "Private/NetHandler.h"
#include "Net.h"

using namespace sp;

NetHandler::NetHandler()
{
    moveToThread(Net::thread());
}

void NetHandler::setReply(QNetworkReply *reply)
{
    _reply = reply;
}
