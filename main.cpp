#include "recrutment.h"
#include <QtSql>
#include <QApplication>
#include <iostream>
#include <QMessageBox>
#include "connection.h"
#include "offre.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
       recrutment r;
       Connection c;
       bool test=c.createconnect();
       if(test)
       {r.show();
           QMessageBox::information(nullptr, QObject::tr("database is open"),
                       QObject::tr("connection successful.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);



   }
       else
           QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                       QObject::tr("connection failed.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);



       return a.exec();
   }
