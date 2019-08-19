#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <set>

#include "compare.h"
#include "process.h"

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

private slots:
    void on_pushButton_clicked();

    void on_btnMax_clicked();

    void on_btnMin_clicked();

    void on_btnInsertProgress_clicked();

    void on_lwProgressTime_itemSelectionChanged();

    void on_lwProgressName_itemSelectionChanged();

    void on_lwProgressPriv_itemSelectionChanged();

    void on_btnStartRun_clicked();

    void on_btnShowAbout_clicked();

    void on_btnReset_clicked();

private:
    Ui::Widget *ui;
    QPoint z;
    std::priority_queue<Process, std::vector<Process>, Compare> pq;
    std::set<std::string> process_table;
};

#endif // WIDGET_H
