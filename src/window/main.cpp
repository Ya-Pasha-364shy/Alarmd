#include "dialog.h"
#include <iostream>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString qCnt = a.arguments().at(1);
    int cnt = qCnt.split(" ")[0].toInt();
    Dialog w(cnt);
    w.show();
    return a.exec();
}
