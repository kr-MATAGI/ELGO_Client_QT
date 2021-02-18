#include <QApplication>
#include <QCoreApplication>
#include "TEST/testform.h"


#include "pthread.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    QApplication a(argc, argv);

    testForm tf;
    tf.show();

    return a.exec();
}
