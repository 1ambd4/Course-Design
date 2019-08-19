#include <QMouseEvent>
#include <QFile>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QCryptographicHash>
#include <algorithm>
#include <QMessageBox>

#include "widget.h"
#include "ui_widget.h"
#include "process.h"
#include "compare.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background:transparent");
    ui->setupUi(this);

    // 去除边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    // shadow margin effect
    QGraphicsDropShadowEffect *shadow_main = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect *shadow_noma = new QGraphicsDropShadowEffect();
    shadow_main->setBlurRadius(10);
    shadow_main->setColor(Qt::black);
    shadow_main->setOffset(0);
    shadow_noma->setBlurRadius(10);
    shadow_noma->setColor(Qt::black);
    shadow_noma->setOffset(0);
    ui->shadowWeight->setGraphicsEffect(shadow_main);
    ui->pushButton->setGraphicsEffect(shadow_noma);

   // ui->lwProgress
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (z == QPoint())
    {
        return ;
    }

    QPoint y = event->globalPos();
    QPoint x = y - z;
    this->move(x);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    // both all vector
    QPoint y = event->globalPos();
    QPoint x = this->geometry().topLeft();
    this->z = y - x;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    this->z = QPoint();
}

void Widget::on_pushButton_clicked()
{
    window()->close();
}

void Widget::on_btnMax_clicked()
{
    if (this->isMaximized())
    {
        ui->vlMain->setMargin(9);
        this->showNormal();
    }
    else
    {
        ui->vlMain->setMargin(0);
        this->showMaximized();
    }
}

void Widget::on_btnMin_clicked()
{
    this->showMinimized();
}

void Widget::on_btnInsertProgress_clicked()
{
    QString name = ui->leInProgressName->text();
    int time = ui->sbInProgressTime->value();
    int priv = ui->sbInProgressPriv->value();

    if (std::find(process_table.begin(), process_table.end(), name.toStdString()) == process_table.end())
    {
        // 输入是否合法，即进程标识非空且所需时间不为0
        if (!name.isEmpty() && time != 0)
        {
            process_table.insert(name.toStdString());
            pq.push(Process(name.toStdString(), time, priv));

            std::priority_queue<Process, std::vector<Process>, Compare> tmp = pq;

            int cnt = 0;

            while (!tmp.empty())
            {
                qDebug() << "______" << QString::fromStdString(tmp.top().name) << " " << tmp.top().time;
                tmp.pop();
            }

            tmp = pq;

            while (!tmp.empty())
            {
                if (QString::fromStdString(tmp.top().name) != name)
                {
                    cnt++;
                }
                else
                {
                    ui->lwProgressName->insertItem(cnt, name);
                    ui->lwProgressTime->insertItem(cnt, QString::number(time));
                    ui->lwProgressPriv->insertItem(cnt, QString::number(priv));
                }
                // qDebug() << QString::fromStdString(tmp.top().name) << " " << tmp.top().time;
                tmp.pop();
            }

            if (!pq.empty())
            {
                qDebug() << "so stupid you!";
            }
        }
        else
        {
            QMessageBox::critical(nullptr, "error information", "The time required for the process needs to be greater than\nzero and process identifier can't be duplicated.");
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "error information", "Duplicate process identifier.");
        qDebug() << "duplicate name";
    }

}

void Widget::on_lwProgressName_itemSelectionChanged()
{
    ui->lwProgressTime->setCurrentRow(ui->lwProgressName->currentRow());
    ui->lwProgressPriv->setCurrentRow(ui->lwProgressName->currentRow());
}

void Widget::on_lwProgressTime_itemSelectionChanged()
{
    ui->lwProgressName->setCurrentRow(ui->lwProgressTime->currentRow());
    ui->lwProgressPriv->setCurrentRow(ui->lwProgressTime->currentRow());
}


void Widget::on_lwProgressPriv_itemSelectionChanged()
{
    ui->lwProgressName->setCurrentRow(ui->lwProgressPriv->currentRow());
    ui->lwProgressTime->setCurrentRow(ui->lwProgressPriv->currentRow());
}

void Widget::on_btnStartRun_clicked()
{
    if (!pq.empty())
    {
        Process _top =  pq.top();
        pq.pop();
        if (_top.time-1 > 0)
        {
            pq.push(Process(_top.name, _top.time-1, _top.priv-1));
        }
        else
        {
            process_table.erase(std::find(process_table.begin(), process_table.end(), _top.name));
        }

        std::priority_queue<Process, std::vector<Process>, Compare> tmp = pq;

        ui->labelShowSelect->setText("进程" + QString::fromStdString(_top.name) + "被调度");

        ui->lwProgressName->clear();
        ui->lwProgressTime->clear();
        ui->lwProgressPriv->clear();

        while (!tmp.empty())
        {
            _top = tmp.top();

            ui->lwProgressName->addItem(QString::fromStdString(_top.name));
            ui->lwProgressTime->addItem(QString::number(_top.time));
            ui->lwProgressPriv->addItem(QString::number(_top.priv));

            tmp.pop();
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "error information", "The progress table is empty now.\nPlease insert first.");
    }
}

void Widget::on_btnShowAbout_clicked()
{
    QMessageBox::information(nullptr, "About", "This program is written by Dirong Chen.");
}

void Widget::on_btnReset_clicked()
{
    ui->lwProgressName->clear();
    ui->lwProgressTime->clear();
    ui->lwProgressPriv->clear();
    while (!pq.empty())
    {
        pq.pop();
    }
    process_table.clear();
}
