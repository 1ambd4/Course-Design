#include <QMouseEvent>
#include <QFile>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QCryptographicHash>
#include <algorithm>
#include <QMessageBox>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <iterator>

#include "widget.h"
#include "ui_widget.h"

// TODO
// debug version tag
#define LTTZZ

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background:transparent");

    ui->setupUi(this);

    // 设置为横向模式
    ui->lwProcessNo->setFlow(QListWidget::LeftToRight);
    ui->lwProcessTime->setFlow(QListWidget::LeftToRight);

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

void Widget::updateJobTable()
{
    ui->lwJobNo->clear();
    ui->lwJobStartAddress->clear();
    ui->lwJobLength->clear();

    for (int i = 0; i < job_table.size(); ++i)
    {
        ui->lwJobNo->addItem(QString::number(job_table[i].no));
        ui->lwJobStartAddress->addItem(QString::number(job_table[i].start));
        ui->lwJobLength->addItem(QString::number(job_table[i].len));
    }
}

void Widget::updateMemoryTable()
{
    ui->lwMemoryStartAddress->clear();
    ui->lwMemoryLength->clear();
    ui->lwMemoryStatus->clear();

    for (int i = 0; i < memory_table.size(); ++i)
    {
        ui->lwMemoryStartAddress->addItem(QString::number(memory_table[i].start));
        ui->lwMemoryLength->addItem(QString::number(memory_table[i].len));
        ui->lwMemoryStatus->addItem("F");
    }
}

void Widget::updateProcessTable()
{
    ui->lwProcessNo->clear();
    ui->lwProcessTime->clear();

    for (int i = 0; i < wait_table.size(); ++i)
    {
        ui->lwProcessNo->addItem(QString::number(wait_table[i].no));
        ui->lwProcessTime->addItem(QString::number(wait_table[i].need));
    }
}

// NOTE
void Widget::insertMemoryTable(int start, int len)
{
    int cnt = 0;

    for (cnt = 0; cnt < memory_table.size(); ++cnt)
    {
        if (memory_table[cnt].start > start)
        {
            break;
        }
    }
    qDebug() << "cnt = " << cnt;


    if (memory_table.empty())   // 空的就直接插
    {
        qDebug() << "empty so this is head and tail";
        memory_table.push_back(Memory(start, len, "F"));
    }
    else
    {
        if (cnt <= 0)       // 前面不需要检查
        {
            qDebug() << "head";
            if (start + len == memory_table[0].start)
            {
                memory_table[0].start = start;
                memory_table[0].len += len;
            }
            else
            {
                memory_table.push_back(Memory(start, len, "F"));
            }
        }
        else if (cnt >= memory_table.size())        // 后面不需要检查
        {
            qDebug() << "tail";
            if (memory_table[cnt-1].start + memory_table[cnt-1].len == start)
            {
                memory_table[cnt-1].len += len;
            }
            else
            {
                memory_table.push_back(Memory(start, len, "F"));
            }
        }
        else                // 前后都要检查
        {
            if (memory_table[cnt-1].start + memory_table[cnt-1].len == start && start + len == memory_table[cnt].start)       // 三块可以合并成一整个
            {
                memory_table[cnt-1].len = memory_table[cnt-1].len + len + memory_table[cnt].len;
                memory_table.erase(memory_table.begin() + cnt);
                updateMemoryTable();
            }
            else
            {
                bool ok = true;
                if (memory_table[cnt-1].start + memory_table[cnt-1].len == start)
                {
                    ok = false;
                    memory_table[cnt-1].len += len;
                }
                if (start + len == memory_table[cnt].start)
                {
                    ok = false;
                    memory_table[cnt].start = start;
                    memory_table[cnt].len += len;
                }
                if (ok)
                {
                    memory_table.push_back(Memory(start, len, "F"));
                }
            }
//            bool ok = true;
//            if (memory_table[cnt-1].start + memory_table[cnt-1].len == start)
//            {
//                ok = false;
//                memory_table[cnt-1].len += len;
//            }
//            if (start + len == memory_table[cnt].start)
//            {
//                ok = false;
//                memory_table[cnt].start = start;
//                memory_table[cnt].len += len;
//            }
//            if (ok)
//            {
//                memory_table.push_back(Memory(start, len, "F"));
//            }
        }
    }
}

void Widget::insertMemoryNo(int start, int len)
{
    for (int i = start; i < start + len; ++i)
    {
        memory_no.insert(i);
    }
}

void Widget::eraseMemoryNo(int start, int len)
{
    for (int i = start; i < start + len; ++i)
    {
        memory_no.erase(i);
    }
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

void Widget::on_btnShowAbout_clicked()
{
    QMessageBox::information(nullptr, "About", "This program is written by Dirong Chen.");
}

void Widget::on_rbJobSelected_toggled(bool checked)
{
    selected = 1;
    ui->lbOne->setText("作业号");
    ui->sbOne->setValue(0);
    ui->lbSecond->setText("起始地址");
    ui->sbSecond->setValue(0);
    ui->lbThird->show();
    ui->sbThird->show();
    ui->lbThird->setText("长度");
    ui->sbThird->setValue(0);
}

void Widget::on_rbMemorySelected_toggled(bool checked)
{
    selected = 2;
    ui->lbOne->setText("起始地址");
    ui->sbOne->setValue(0);
    ui->lbSecond->setText("长度");
    ui->sbSecond->setValue(0);
    ui->lbThird->hide();
    ui->sbThird->hide();
}

void Widget::on_rbProgressRequire_toggled(bool checked)
{
    selected = 3;
    ui->lbOne->setText("作业号");
    ui->sbOne->setValue(0);
    ui->lbSecond->setText("需要量");
    ui->sbSecond->setValue(0);
    ui->lbThird->hide();
    ui->sbThird->hide();
}

void Widget::on_puInsert_clicked()
{
    // 当前为插入作业表
    if (selected == 1)
    {
        // TODO
        // 合法性检测

        // 获取参数
        int no = ui->sbOne->value();
        int start = ui->sbSecond->value();
        int len = ui->sbThird->value();

        // 数据合法性初步检测
        if (std::find(job_no.begin(), job_no.end(), no) != job_no.end())
        {
            QMessageBox::critical(nullptr, "error information", "Job no cant't be duplicate.");
            return ;
        }

        if (len <= 0)
        {
            QMessageBox::critical(nullptr, "error information", "Job length should not less than zero.");
            return ;
        }

        for (int i = start; i < start + len; ++i)
        {
            if (std::find(memory_no.begin(), memory_no.end(), i) != memory_no.end())
            {
                QMessageBox::critical(nullptr, "error information", "Memory start address cant't be duplicate.");
                return ;
            }
        }

        job_no.insert(no);

        // TODO
        job_table.push_back(Job(no, start, len));

        insertMemoryNo(start, len);

        // 每次更新表格内容前先排一下序
        std::sort(job_table.begin(), job_table.end(), [](const Job& obja, const Job& objb) {
            return obja.no < objb.no;
        });

        updateJobTable();

#ifdef LTTZZ
        for (int i = 0; i < job_table.size(); ++i)
        {
           // qDebug() << job_table[i].no;
        }
#endif
    }
    // 当前为插入内存表
    else if (selected == 2)
    {
        // TODO
        // 合法性检测

        int start = ui->sbOne->value();
        int len = ui->sbSecond->value();

        // TODO
        // start 差错性检测

//        if (std::find(memory_no.begin(), memory_no.end(), start) != memory_no.end())
//        {
//            QMessageBox::critical(nullptr, "error information", "Memory start address cant't be duplicate.");
//            return ;
//        }
        for (int i = start; i < start + len; ++i)
        {
            if (std::find(memory_no.begin(), memory_no.end(), i) != memory_no.end())
            {
                QMessageBox::critical(nullptr, "error information", "Memory start address cant't be duplicate.");
                return ;
            }
        }

        if (len <= 0)
        {
            QMessageBox::critical(nullptr, "error information", "Memory length should not less than zero.");
            return ;
        }

        // TODO
        // 这边合并会导致一些意外的bug吧, 那我这边no直接insert一个区间?
        //memory_no.insert(start);
        insertMemoryNo(start, len);

        // 插入的时候,需要分三种情况,上连下不连就和上面合并,下连上不连就和下面合并,都不连就直接插.
        // 可以在插入的时候直接判断,也可以不管连不连续,直接插入,插完了再考虑合并.


        // NOTE

        insertMemoryTable(start, len);
//        int cnt = 0;

//        for (cnt = 0; cnt < memory_table.size(); ++cnt)
//        {
//            if (memory_table[cnt].start > start)
//            {
//                break;
//            }
//        }
//        qDebug() << "cnt = " << cnt;


//        if (memory_table.empty())   // 空的就直接插
//        {
//            qDebug() << "empty so this is head and tail";
//            memory_table.push_back(Memory(start, len, "F"));
//        }
//        else
//        {
//            if (cnt <= 0)       // 前面不需要检查
//            {
//                qDebug() << "head";
//                if (start + len == memory_table[0].start)
//                {
//                    memory_table[0].start = start;
//                    memory_table[0].len += len;
//                }
//                else
//                {
//                    memory_table.push_back(Memory(start, len, "F"));
//                }
//            }
//            else if (cnt >= memory_table.size())        // 后面不需要检查
//            {
//                qDebug() << "tail";
//                if (memory_table[cnt-1].start + memory_table[cnt-1].len == start)
//                {
//                    memory_table[cnt-1].len += len;
//                }
//                else
//                {
//                    memory_table.push_back(Memory(start, len, "F"));
//                }
//            }
//            else                // 前后都要检查
//            {
//                bool ok = true;
//                if (memory_table[cnt-1].start + memory_table[cnt-1].len == start)
//                {
//                    ok = false;
//                    memory_table[cnt-1].len += len;
//                }
//                if (start + len == memory_table[cnt].start)
//                {
//                    ok = false;
//                    memory_table[cnt].start = start;
//                    memory_table[cnt].len += len;
//                }
//                if (ok)
//                {
//                    memory_table.push_back(Memory(start, len, "F"));
//                }
//            }
//        }

        // NOTE


        // 每次更新表格内容前先排一下序
        std::sort(memory_table.begin(), memory_table.end(), [](const Memory& obja, const Memory& objb) {
            return obja.start < objb.start;
        });

        updateMemoryTable();

#ifdef LTTZZ
        for (int i = 0; i < memory_table.size(); ++i)
        {
            qDebug() << memory_table[i].start;
        }
    }
#endif
    // 当前为插入等待分配作业表
    else if (selected == 3)
    {
        // TODO
        // 合法性检测


        int no = ui->sbOne->value();
        int need = ui->sbSecond->value();

        if (std::find(job_no.begin(), job_no.end(), no) != job_no.end())
        {
            QMessageBox::critical(nullptr, "error information", "Job no cant't be duplicate.");
            return ;
        }

        if (need <= 0)
        {
            QMessageBox::critical(nullptr, "error information", "Job need time should not less than zero.");
            return ;
        }

        job_no.insert(no);

        wait_table.push_back(WaitAllocate(no, need));

        std::sort(wait_table.begin(), wait_table.end(), [](const WaitAllocate& obja, const WaitAllocate& objb) {
            return obja.no < objb.no;
        });

        updateProcessTable();

#ifdef LTTZZ
        for (int i = 0; i < wait_table.size(); ++i)
        {
            qDebug() << wait_table[i].no;
        }
#endif
    }
}

void Widget::on_lwJobNo_itemSelectionChanged()
{
    int id = ui->lwJobNo->currentRow();
    ui->lwJobStartAddress->setCurrentRow(id);
    ui->lwJobLength->setCurrentRow(id);
}

void Widget::on_lwJobStartAddress_itemSelectionChanged()
{
    int id = ui->lwJobStartAddress->currentRow();
    ui->lwJobNo->setCurrentRow(id);
    ui->lwJobLength->setCurrentRow(id);
}

void Widget::on_lwJobLength_itemSelectionChanged()
{
    int id = ui->lwJobLength->currentRow();
    ui->lwJobNo->setCurrentRow(id);
    ui->lwJobStartAddress->setCurrentRow(id);
}

void Widget::on_lwMemoryStartAddress_itemSelectionChanged()
{
    int id = ui->lwMemoryStartAddress->currentRow();
    ui->lwMemoryLength->setCurrentRow(id);
    ui->lwMemoryStatus->setCurrentRow(id);
}

void Widget::on_lwMemoryLength_itemSelectionChanged()
{
    int id = ui->lwMemoryLength->currentRow();
    ui->lwMemoryStartAddress->setCurrentRow(id);
    ui->lwMemoryStatus->setCurrentRow(id);
}

void Widget::on_lwMemoryStatus_itemSelectionChanged()
{
    int id = ui->lwMemoryStatus->currentRow();
    ui->lwMemoryStartAddress->setCurrentRow(id);
    ui->lwMemoryLength->setCurrentRow(id);
}


void Widget::on_lwProcessNo_itemSelectionChanged()
{
    int id = ui->lwProcessNo->currentRow();
    ui->lwProcessTime->setCurrentRow(id);
}

void Widget::on_lwProcessTime_itemSelectionChanged()
{
    int id = ui->lwProcessTime->currentRow();
    ui->lwProcessNo->setCurrentRow(id);
}

void Widget::on_lwJobNo_itemClicked(QListWidgetItem *item)
{
    int id = ui->lwJobNo->currentRow();

    // TODO
    // 点击条目释放内存，修改空闲分区表

    // TODO
    // 添加，此处仅做添加处理，实际需要合并,当然显示之前需要再sort一下

    insertMemoryTable(job_table[id].start, job_table[id].len);

    // memory_table.push_back(Memory(job_table[id].start, job_table[id].len));

    // memory_no.insert(job_table[id].start);
    insertMemoryNo(job_table[id].start, job_table[id].len);

    std::sort(memory_table.begin(), memory_table.end(), [](const Memory& obja, const Memory& objb) {
        return obja.start < objb.start;
    });

    std::set<int>::iterator it = std::find(job_no.begin(), job_no.end(), job_table[id].no);
    job_no.erase(it);

    // 这句话要写在上边的删除job_no后面,否则会导致删除出错
    job_table.erase(job_table.begin() + id);

    updateMemoryTable();

    updateJobTable();
}

// TODO
// 最先适应分配内存部分
// 那就是找,由于现在内存一定不会是连续的,也就是说不需要考虑分区拼接问题,只考虑分配即可.
void Widget::on_lwProcessNo_itemClicked(QListWidgetItem *item)
{
    int id = ui->lwProcessNo->currentRow();

    // TODO
    // 分配函数还未完成
    int need = wait_table[id].need;
    bool ok = false;

    int cnt = 0;
    for (cnt = 0; cnt < memory_table.size(); ++cnt)
    {
        if (need <= memory_table[cnt].len)
        {
            ok = true;
            break;
        }
    }

    qDebug() << cnt;

    if (ok)         // 可以分配
    {
        qDebug() << "can allocate memory.";

        eraseMemoryNo(memory_table[cnt].start, memory_table[cnt].len);


        job_table.push_back(Job(wait_table[id].no, memory_table[cnt].start, need));

        memory_table[cnt].start += need;
        memory_table[cnt].len -= need;

        // TODO
        // erase memory no

        if (memory_table[cnt].len == 0)
        {
            memory_table.erase(memory_table.begin() + cnt);
        }

        // 这句话要写在上边的删除job_no后面,否则会导致删除出错
        wait_table.erase(wait_table.begin() + id);

        updateMemoryTable();

        updateJobTable();

        updateProcessTable();
    }
    else            // 无法完成分配,需要手动释放内存
    {
        QMessageBox::critical(nullptr, "title", "Can not allocate, please release the process first.");
        return ;
    }

    // std::set<int>::iterator it = std::find(job_no.begin(), job_no.end(), wait_table[id].no);
    // TODO
    // 弱鸡rease,应该改成erase区间
    // job_no.erase(it);
}

void Widget::on_btnReset_clicked()
{
    job_table.clear();
    job_no.clear();
    memory_table.clear();
    memory_no.clear();
    wait_table.clear();

    updateJobTable();
    updateMemoryTable();
    updateProcessTable();
}
