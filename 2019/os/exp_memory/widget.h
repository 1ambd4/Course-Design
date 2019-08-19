#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <set>
#include <algorithm>
#include <QListWidgetItem>
#include <iterator>

#include "job.h"
#include "memory.h"
#include "waitallocate.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget();

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void updateJobTable();

    void updateMemoryTable();

    void updateProcessTable();

    void insertMemoryTable(int start, int len);

    void insertMemoryNo(int start, int len);

    void eraseMemoryNo(int start, int len);

private slots:
    void on_pushButton_clicked();

    void on_btnMax_clicked();

    void on_btnMin_clicked();

    void on_btnShowAbout_clicked();

    void on_rbJobSelected_toggled(bool checked);

    void on_rbMemorySelected_toggled(bool checked);

    void on_puInsert_clicked();

    void on_lwJobNo_itemSelectionChanged();

    void on_lwJobStartAddress_itemSelectionChanged();

    void on_lwJobLength_itemSelectionChanged();

    void on_lwMemoryStartAddress_itemSelectionChanged();

    void on_lwMemoryLength_itemSelectionChanged();

    void on_lwMemoryStatus_itemSelectionChanged();

    void on_rbProgressRequire_toggled(bool checked);

    void on_lwProcessNo_itemSelectionChanged();

    void on_lwProcessTime_itemSelectionChanged();

    void on_lwJobNo_itemClicked(QListWidgetItem *item);

    void on_lwProcessNo_itemClicked(QListWidgetItem *item);

    void on_btnReset_clicked();

private:
    Ui::Widget *ui;
    QPoint z;

    std::vector<Job> job_table;
    std::set<int> job_no;
    std::vector<Memory> memory_table;
    std::set<int> memory_no;
    std::vector<WaitAllocate> wait_table;

    // 标识当前选中插入作业还是插入内存分区
    int selected = 1;
};

#endif // WIDGET_H
