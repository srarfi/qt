#ifndef OFFRE_H
#define OFFRE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QDateEdit>
#include <QApplication>
#include <QtWidgets>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QApplication>
#include <QtWidgets>
#include <QProcess>
#include <QDateTime>


class Condidat
{
private:
    int id,test;
    QString tel,email,cin,nom,prenom;

public:
    void setid(int n);
    void setidtest(int n){this->test=n;};
    void setnom(QString n);
    void setprenom(QString n);
    void setemail(QString n);
    void setcin(QString n);
    void settel(QString n);

    int getid();
    int getidtst(){return test;};
    QString getnom();
    QString getprenom();
    QString getemail();
    QString gettel();
    QString getcin();


    Condidat();
    Condidat (QString,QString, QString,QString,QString);
    Condidat (int,QString,QString,QString,QString,QString);
    Condidat (int a){id=a;};
    Condidat (int a,int b){id=a;test=b;};

    int ajouter();
    bool chercher();
    QSqlQueryModel * afficher ();
    bool supprimer();
    int modifier();
    bool test_champs_condidat();
    bool test_action();
    bool test_action_upd();


};


#endif // OFFRE_H
