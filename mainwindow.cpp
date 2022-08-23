#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QSqlError"

#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_6->setHidden(true);
    ui->pushButtonSQLExecute->setEnabled(false);
    ui->pushButtonUseDB->setEnabled(false);
    ui->pushButtonSelectAllFromTable->setEnabled(false);
    ui->pushButtonCreateTable->setEnabled(false);
    ui->pushButtonInsertTestData->setEnabled(false);
    qDebug() <<  QSqlDatabase::drivers();

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}
//连接目标数据库
void MainWindow::on_pushButtonConnect_clicked()
{
    QString dbUrl = ui->lineEditDBAddr->text();
    QString dbUsername = ui->lineEditUserName->text();
    QString dbPassword = ui->lineEditUserPassword->text();
    QString dbDatabaseName = ui->lineEditDBName->text();

//    if(QSqlDatabase::database())

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbUrl);
    db.setPort(3306);//是mysql服务器的端口号
    db.setUserName(dbUsername);
    db.setPassword(dbPassword);
//    db.setDatabaseName(dbDatabaseName);

    if (!db.open()) {
        qDebug() << QString::fromLocal8Bit("fail");
//        QString error = db.lastError();
        ui->plainTextEditLog->appendPlainText("打开失败 请检查数据库各参数是否错误");

    }
    else{
        qDebug() << QString::fromLocal8Bit("打开成功");
        ui->plainTextEditLog->appendPlainText("打开成功 有如下数据库：");

        QSqlQuery query;
        query.exec("SHOW DATABASES");


        while (query.next()) {
            ui->plainTextEditLog->appendPlainText(query.value(0).toString());
        }

        ui->pushButtonSQLExecute->setEnabled(true);
        ui->pushButtonUseDB->setEnabled(true);
    }



}
//执行自定义sql
void MainWindow::on_pushButtonSQLExecute_clicked()
{
    QString sql = ui->textEditSQL->toPlainText();
    QSqlQuery query;
    if(query.exec(sql)){
        ui->plainTextEditLog->appendPlainText("sql语句运行成功,暂时无法查看返回值");


    }else{
        ui->plainTextEditLog->appendPlainText("sql语句:"+sql+"运行失败 :"+query.lastError().text());
//        ui->plainTextEditLog->appendPlainText()
    }


}
//创建目标表
void MainWindow::on_pushButtonCreateTable_clicked()
{
    QSqlQuery query;
    if(query.exec("CREATE TABLE BATCH_NO(b_no varchar(20) not null primary key);")){
        ui->plainTextEditLog->appendPlainText("创表成功");

    }else{
        ui->plainTextEditLog->appendPlainText("创建新表失败 :"+query.lastError().text());
    }
}
//使用目标db
void MainWindow::on_pushButtonUseDB_clicked()
{
    QString dbname = ui->lineEditDBName->text();
    QString sql = "use "+dbname;
    QSqlQuery query;
    if(query.exec(sql)){
        ui->plainTextEditLog->appendPlainText("选中db成功："+dbname );
        ui->pushButtonCreateTable->setEnabled(true);
        ui->pushButtonSelectAllFromTable->setEnabled(true);
        ui->pushButtonInsertTestData->setEnabled(true);
    }else{
        ui->plainTextEditLog->appendPlainText("sql语句:"+sql+"运行失败 :"+query.lastError().text());
    }

}
//查全表
void MainWindow::on_pushButtonSelectAllFromTable_clicked()
{
    QSqlQueryModel model;
    QString tablename = ui->lineEditTableName->text();
    model.setQuery("SELECT * FROM "+tablename);

    QSqlDatabase db = QSqlDatabase::database();
    QSqlRecord record = db.record(tablename);

    QString columnName;
    QString columnValue;
    QSqlField field;
    for (int i = 0; i < model.rowCount(); ++i) {
        for (int j = 0; j < record.count(); ++j) {
            field = record.field(j);
            columnName = field.name();
            columnValue = model.record(i).value(j).toString();
            ui->plainTextEditLog->appendPlainText(columnName+":" + columnValue);
        }
    }

    if(model.rowCount() == 0){
        ui->plainTextEditLog->appendPlainText("空表或无此表");
    }
}
//插入测试数据
void MainWindow::on_pushButtonInsertTestData_clicked()
{
    QSqlQueryModel model;
    QString tablename = ui->lineEditTableName->text();

    QString sql = QString("Insert into %1 value(%2) ").arg("BATCH_NO")
            .arg("\"Test114514Test-"+QString::number(rand()%99999) + " \" ");

    QSqlQuery query;
    if(query.exec(sql)){
        ui->plainTextEditLog->appendPlainText("插入测试数据成功");

    }else{
        ui->plainTextEditLog->appendPlainText("sql语句:"+sql+"运行失败 :"+query.lastError().text());
    }

}
