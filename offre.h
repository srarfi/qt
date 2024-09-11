#ifndef offre_H
#define offre_H
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


class Offre
{
private:
    int id,test,id_condidat;
    QString titre,description;
    float salaire;
    QDate date_pub,date_exp;
public:
    void setid(int n);
    void setidtest(int n){this->test=n;};
    void setidcondidat(int n){this->id_condidat=n;};
    void settitre(QString n);
    void setdescription(QString n);
    void setsalaire(float n);
    void setdate_pub(QDate n);
    void setdate_exp(QDate n);

    int getid();
    int getidtst(){return test;};
    int getidcondidat(){return id_condidat;};
    QString gettitre();
    float getsalaire();
    QDate getdate_pub();
    QDate getdate_exp();
    QString getdescription();


    Offre();
    Offre (QString,float, QDate,QDate,QString,int);
    Offre (int,float,QString,QString,QDate,QDate,int);
    Offre (int a){id=a;};
    Offre (int a,int b){id=a;test=b;};


    bool ajouter();
    bool chercher();
    QSqlQueryModel * afficher ();
    bool supprimer();
    bool modifier();
    bool test_champs_Offre();
    bool test_action();
    bool test_action_upd();
    int testExistID(int);

};


#endif // offre_H
