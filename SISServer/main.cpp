#include <QtCore/QCoreApplication>

#include "sisserverlibrary.h"

int main(int argc, char *argv[])
{
    Server server;
    QCoreApplication a(argc, argv);

    return a.exec();
}
