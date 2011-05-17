#include <QtCore/QCoreApplication>

#include "sisserverlibrary.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SisServer server;

    return a.exec();
}
