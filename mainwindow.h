#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();

    void on_addCost_clicked();

    void on_costDelBtn_clicked();

    void on_showChartBtn_clicked();

private:
    Ui::MainWindow *ui;

    int totalIncome = 0;
    int totalCost = 0 ;
    int accountBalance =0;
    void saveData();
    void loadData();
    void updateBalance();
    QString formatNumber(int number);
    QString convertPersianToEnglish(QString input);
    QString currentDate();

    void buildChartForMonth(const QString& monthName);

};
#endif // MAINWINDOW_H
