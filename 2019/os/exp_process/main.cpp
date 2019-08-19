#include <QApplication>
#include <QTextCodec>

#include "widget.h"
#include "process.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置编码方式
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Widget w;
    w.show();

    return a.exec();
}
