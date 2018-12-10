#include "customerwindow.h"
#include "ui_customerwindow.h"
#include "customer.h"
#include "queue.h"
#include <QDir>
#include <QFile>
#include <QTextStream>

extern int type;
extern QString nameStr;
extern Queue maleQueue , femaleQueue;
extern QFile maleQueueFile;
extern QFile femaleQueueFile;
extern BookList bookList;
extern QFile bookListFile;

CustomerWindow::CustomerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerWindow)
{
    ui->setupUi(this);
    ui->back_btn->setIcon(QIcon("../pro1/pics/back.png"));

    if(bookListFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&bookListFile);
        int i = 0;
        in.setCodec("UTF-8");
        while(!in.atEnd()){

            QString line = in.readLine();
//            qDebug(line.toUtf8().data());

            QStringList bookInfo = line.split(';');
//            for(int k = 0;k < 4;k++)
//                qDebug(bookInfo.at(k).toStdString().c_str());

            Book *newBook = new Book(bookInfo.at(0) , bookInfo.at(1) ,  bookInfo.at(2).toInt() , bookInfo.at(3).toInt());

            bookList.addEnd(newBook);


            ui->books_tableWidget->insertRow(i);
            QTableWidgetItem *item;
                for(int j = 0;j < 4;j++){
//                    qDebug(bookInfo.at(j).toUtf8().data());
                    item = new QTableWidgetItem(bookInfo.at(j));

                    ui->books_tableWidget->setItem(i ,j , item);
                }
                item = new QTableWidgetItem("+");
                ui->books_tableWidget->setItem(i , 4 , item);

        i++;
        }
        bookList.display();

        bookListFile.close();
    }
    QHeaderView* header = ui->books_tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setStretchLastSection(true);


}

CustomerWindow::~CustomerWindow()
{
    delete ui;
}

void CustomerWindow::on_back_btn_clicked()
{
    this->close();
    CustomerEnterWindow *cstEnterWindow = new CustomerEnterWindow;
    cstEnterWindow->showNormal();
}

void CustomerWindow::on_finish_btn_clicked()
{


    Customer *customer = new Customer(nameStr , type);
    if(type == 1){
        maleQueue.push_rear(customer);

        if(maleQueueFile.open(QIODevice::Append | QIODevice::Text)){
            QTextStream out(&maleQueueFile);
            out<<nameStr<<";"<<QString::number(type)<<";";
        }

    }
    else if(type == 0){
        femaleQueue.push_rear(customer);
    }


}

void CustomerWindow::on_addBookToStack_btn_clicked()
{
    if(ui->books_tableWidget->rowCount()){
        bookList.remove(bookList.getBookAt(ui->books_tableWidget->currentRow()));
         qDebug("remove");
         bookList.display();
         ui->books_tableWidget->removeRow(ui->books_tableWidget->currentRow());
         if(bookListFile.open(QIODevice::WriteOnly | QIODevice::Text)){
             QTextStream out(&bookListFile);
             out.setCodec("UTF-8");
             for(int i = 0;i < bookList.getBooksNumber();i++){
                 out<<bookList.getBookAt(i)->getTitle()<<";"<<bookList.getBookAt(i)->getAuthor()<<";"<<bookList.getBookAt(i)->getPublishYear()<<";"<<bookList.getBookAt(i)->getPrice()<<"\n";
             }

             bookListFile.close();
         }
    }
}
