#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "jalali.h"
#include <QIntValidator>
#include <QFile>
#include <QTextStream>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->costCategoryComboBox->addItems({
        "تفریح", "خورد و خوراک", "رفت‌وآمد", "قبوض", "پزشکی", "لباس", "سایر"
    });

    ui->monthComboBox->addItems({
        "فروردین", "اردیبهشت", "خرداد", "تیر", "مرداد", "شهریور",
        "مهر", "آبان", "آذر", "دی", "بهمن", "اسفند"
    });



    this->setLayoutDirection(Qt::RightToLeft);
    ui->monayInput->setValidator(new QIntValidator(1,1000000000, this));
    loadData();
}


MainWindow::~MainWindow()
{
    saveData();
    delete ui;
}

void MainWindow::updateBalance()
{
    accountBalance = totalIncome - totalCost;
    QString balanceStr = QLocale(QLocale::Persian).toString(accountBalance);
    ui->balanceLabel->setText(balanceStr);
}

QString MainWindow::formatNumber(int number)
{
    return QLocale(QLocale::Persian).toString(number);
}

QString MainWindow::convertPersianToEnglish(QString input) {
    QString persianDigits[] = {"۰","۱","۲","۳","۴","۵","۶","۷","۸","۹"};
    for (int i = 0; i < 10; ++i) {
        input.replace(persianDigits[i], QString::number(i));
    }
    return input;
}

QString MainWindow::currentDate(){

    QDate today = QDate::currentDate();
    QDate jDate = Jalali::gregorianToJalali(today);
    QStringList persianMonths = {
        "فروردین", "اردیبهشت", "خرداد", "تیر", "مرداد", "شهریور",
        "مهر", "آبان", "آذر", "دی", "بهمن", "اسفند"
    };
    QString dateStr = QString("%1 %2 %3").arg(jDate.day()).arg(persianMonths[jDate.month()-1]).arg(jDate.year());
    return dateStr;

}

void MainWindow::on_addBtn_clicked()
{
    QString income = ui->monayInput->text();
    QString resurce = ui->resInput->text();

    //check for null fild
    if(income.isEmpty() || resurce.isEmpty()){
        QMessageBox::warning(this, "خطا", "لطفاً مبلغ و منبع را وارد کنید.");
        return;

    }




    QString formattedIncome = formatNumber(income.toInt());

    QString items =currentDate()+" - "+resurce+" - "+formattedIncome +" تومان " ;
    ui->listWidget->addItem(items);

    int addAmount = income.toLongLong();
    totalIncome += addAmount;

    updateBalance();

    ui->monayInput->clear();
    ui->resInput->clear();

}


void MainWindow::on_delBtn_clicked()
{
    QListWidgetItem* selectedItem = ui->listWidget->currentItem();

    if(!selectedItem){
        QMessageBox::warning(this,"خطا","هیچ موردی انتخاب نشده است");
        return;
    }

    QString textItem = selectedItem->text();
    QStringList parts = textItem.split(" - ");

    if (parts.size() < 3) {
        QMessageBox::warning(this, "خطا", "فرمت آیتم نادرست است");
        return;
    }

    QString amountStr = parts[2];
    amountStr.replace(" تومان", "");
    amountStr.replace("٬", "");
    amountStr = convertPersianToEnglish(amountStr);

    qint64 amount = amountStr.toLongLong();
    //
    totalIncome -= amount;
    updateBalance();

    delete selectedItem;
}

void MainWindow::saveData()
{
    QFile file("data.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){

        QTextStream out(&file);

        //save incoms
        for(int row = 0 ; row < ui->listWidget->count() ; row++){
            QString line = ui->listWidget->item(row)->text();
            out <<"INCOME:"<< line<<"\n";
        }

        //save costs
        for(int row = 0 ; row < ui->costList->count() ; row ++){
            QString line = ui->costList->item(row)->text();
            out<<"COST:"<<line<<"\n";
        }

        //save totals
        out << "TOTAL_INCOME:" <<totalIncome << "\n";
        out <<"TOTAL_COST:" <<totalCost << "\n";

        file.close();
    }
}

void MainWindow::loadData()
{
    QFile file("data.txt");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){

        QTextStream in(&file);

        while(!in.atEnd()){
            QString line = in.readLine();

            if(line.startsWith("INCOME:")){
                QString content = line.section("INCOME:",1);
                ui->listWidget->addItem(content);
            }

            else if (line.startsWith("COST:")){
                QString content = line.section("COST:",1);
                ui->costList->addItem(content);
            }
            else if (line.startsWith("TOTAL_INCOME:")){
                QString value = line.section(":",1);
                totalIncome = value.toInt();
                QString formattedTotal = formatNumber(totalIncome);
                ui->labelTotal->setText(formattedTotal);
            }
            else if (line.startsWith("TOTAL_COST:")){
                QString value = line.section(":",1);
                totalCost = value.toInt();
                QString formattedTotal = formatNumber(totalCost);
                ui->costAmount->setText(formattedTotal);
            }
        }
        file.close();
    }
    updateBalance();
}




void MainWindow::on_addCost_clicked()
{
    //get info from input
    QString category = ui->costCategoryComboBox->currentText();
    QString costInput = ui->costAmountInput->text();
    QString costDes = ui->costDescInput->text();
    //
    QString formattedCost = formatNumber(costInput.toInt());

    // qDebug()<<"catguray : " << category<< " cost amountt : " << formattedCost<< "recurce  : " << costDes ;

    if(costDes.isEmpty()){
        costDes = category;
    }
    if(costInput.isEmpty()){
        QMessageBox::warning(this, "خطا","مقدار هزینه خالی است");
        return;
    }

    // currentDate();



    QString item = QString("%1 -  %2 - %3 -  %4 تومان").arg(currentDate()).arg(category).arg(costDes).arg(formattedCost);
    //add item to list


    ui->costList->addItem(item);
    totalCost += costInput.toInt();
    QString totalCostStr = formatNumber(totalCost);
    ui->costAmount->setText(totalCostStr);

    //Update account balance

    accountBalance -= costInput.toInt();
    updateBalance();

    ui->costAmountInput->clear();
    ui->costDescInput->clear();
    // qDebug()<< "Account Balance " << accountBalance << "total income : " <<totalIncome;
}


void MainWindow::on_costDelBtn_clicked()
{
    QListWidgetItem* item = ui->costList->currentItem();
    //check selected item
    if(!item){
        QMessageBox::warning(this , "خطا", "موردی انتخاب نشده است");
        return;
    }

    //get item amount
    QStringList itemStr = item->text().split("-");

    QString costAmount = itemStr[2].trimmed();
    costAmount.replace("تومان" ,"");
    costAmount.replace("٬","");
    // qDebug()<<"costAmount : "<< costAmount;
    costAmount = convertPersianToEnglish(costAmount);

    totalCost -= costAmount.toInt();

    QString totalCostStr = formatNumber(totalCost);
    ui->costAmount->setText(totalCostStr);

    //update costAmount and balance

    accountBalance += costAmount.toInt();
    updateBalance();



    delete item;
}

void MainWindow::buildChartForMonth(const QString& monthName)
{
    QMap<QString, qint64> categoryTotals;


    for (int i = 0; i < ui->costList->count(); ++i) {
        QString itemText = ui->costList->item(i)->text();
        QStringList parts = itemText.split(" - ");
        if (parts.size() < 4) continue;

        QString datePart = parts[0].trimmed();
        QString category = parts[1].trimmed();
        QString amountStr = parts[3].replace("تومان", "").replace("٬", "").trimmed();

        if (!datePart.contains(monthName)) continue;

        amountStr = convertPersianToEnglish(amountStr);
        qint64 amount = amountStr.toLongLong();

        categoryTotals[category] += amount;
    }


    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("هزینه‌ها");

    QStringList categories;
    for (auto it = categoryTotals.begin(); it != categoryTotals.end(); ++it) {
        *barSet << it.value();
        categories << it.key();
    }

    series->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("نمودار هزینه‌ها در " + monthName);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QLayout *oldLayout = ui->chartContainer->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
    layout->addWidget(chartView);
}

void MainWindow::on_showChartBtn_clicked()
{
    QString selectedMonth = ui->monthComboBox->currentText();
    buildChartForMonth(selectedMonth);
}

