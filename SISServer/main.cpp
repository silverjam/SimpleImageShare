#include <QtCore/QCoreApplication>

#include "sisserverlibrary.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SisServer server(0);

    return a.exec();
}
