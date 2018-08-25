#include <QTimer>

#include "testping.h"

#include "../../ping.h"

TestPing::TestPing()
{
    Ping ping;
    ping.onPing();

    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), &ping, SLOT(onPing()));
    timer->start(10000);
}
