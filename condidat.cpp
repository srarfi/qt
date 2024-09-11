#include "condidat.h"
#include <QDate>
#include <QDateEdit>
#include <QDate>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtDebug>
#include <QSqlError>
#include <QMessageBox>
#include "offre.h"

//setters
void Condidat::setid(int n){id=n;}
void Condidat::setnom(QString n){nom=n;}
void Condidat::setprenom(QString n){prenom=n;}
void Condidat::setemail(QString n){email=n;}
void Condidat::settel(QString n){tel=n;}
void Condidat::setcin(QString n){cin=n;}



//getters
int Condidat::getid(){return id;}
QString Condidat::getnom(){return nom;}
QString Condidat::getprenom(){return prenom;}
QString Condidat::getemail(){return email;}
QString Condidat::gettel(){return tel;}
QString Condidat::getcin(){return cin;}


//constructer

Condidat::Condidat()
{
    id=0;
    nom = "";
    prenom = "";
    email = "";
    tel = "";
    cin="";
}

Condidat::Condidat(QString a,QString b , QString c,QString d ,QString e){
    this->nom=a;
    this->prenom=b;
    this->email=c;
    this->cin=d;
    this->tel=e;
}

Condidat::Condidat (int a,QString b , QString c,QString d ,QString e, QString f){
    this->id=a;
    this->nom=b;
    this->prenom=c;
    this->email=d;
    this->cin=e;
    this->tel=f;
}





//function
//controle de saisie

bool Condidat::test_champs_condidat() {
    // Vérifier que les champs ne sont pas vides
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || cin.isEmpty() || tel.isEmpty() || id == 0) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Tous les champs doivent être remplis.");
        return false;
    }

    // Vérifier la validité du CIN (8 caractères numériques)
    if (cin.length() != 8 || !cin.toInt()) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le CIN doit être un nombre à 8 chiffres.");
        return false;
    }

    // Vérifier l'email (format simple de vérification)
    QRegExp emailRegex("^\\S+@\\S+\\.\\S+$");
    if (!emailRegex.exactMatch(email)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "L'adresse email n'est pas valide.");
        return false;
    }

    // Vérifier le numéro de téléphone (ici, on suppose un format de 8 chiffres)
    if (tel.length() != 8 || !tel.toInt()) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le numéro de téléphone doit être un nombre à 8 chiffres.");
        return false;
    }

    // Si toutes les vérifications passent, retourner true
    return true;

}


bool Condidat::test_action(){

        // Check if 'name' is not empty
        if (nom.isEmpty()) {
            QMessageBox::critical(nullptr, "Erreur", "Faites la recherche avant de continuer.");
            return false;
        }
        return true;
}


bool Condidat::test_action_upd(){
    if (id ==0) {
        // Gérer le cas où la chaîne est vide
        QMessageBox::critical(nullptr, "Error", "ecrire le id por faire la recherche.");
        return false;
    }

        return true;
    }
// crud

// create

int Condidat::ajouter() {
    qDebug() << "Database Connection Status:" << QSqlDatabase::database().isOpen();
      // If ID_OFFRE_C exists, proceed to insert the candidat
      QSqlQuery query;
      query.prepare("INSERT INTO CANDIDATS (NOM, PRENOM, EMAIL, TELEPHONE, CIN) "
                    "VALUES (:nom, :prenom, :email, :telephone, :cin)");

      // Bind the values to the query
      query.bindValue(":nom", nom);
      query.bindValue(":prenom", prenom);
      query.bindValue(":email", email);
      query.bindValue(":telephone", tel);
      query.bindValue(":cin", cin);

      qDebug() << "Data extracted from UI fields:";
      qDebug() << "nom: " << nom;
      qDebug() << "prenom: " << prenom;
      qDebug() << "email: " << email;
      qDebug() << "telephone: " << tel;
      qDebug() << "cin: " << cin;


      // Execute the query to insert data into the database
      if (query.exec()) {
          qDebug() << "Candidat added successfully!";
          return 1;  // Candidat added successfully
      } else {
          qDebug() << "Error adding Candidat to the database:" << query.lastError().text();
          return 2;  // Error during insertion
      }
}


bool Condidat::chercher() {
    QString idd = QString::number(id);
    QSqlQuery query;
    query.prepare("SELECT * FROM CANDIDATS WHERE ID_CANDIDAT = :id");
    query.bindValue(":id", idd);

    if (query.exec() && query.next()) {
        // Fetch data from the query result and populate the Sponsor object
        nom = query.value("NOM").toString();
        prenom = query.value("PRENOM").toString();
        email = query.value("EMAIL").toString();
        tel = query.value("TELEPHONE").toString();
        cin = query.value("CIN").toString();
        test=id;
        return true; // Data found and populated successfully
    } else {
        qDebug() << "Error fetching CONDIDAT from the database:" << query.lastError().text();
        return false; // Sponsor with the specified ID not found
    }
}




//delate

bool Condidat::supprimer() {
    QSqlQuery query;
    QString idString = QString::number(id);
    query.prepare("DELETE FROM CANDIDATS WHERE ID_CANDIDAT = :id");
    query.bindValue(":id", idString);


    if (query.exec()) {
        qDebug() << "Offre with ID" << id << "deleted successfully.";
        return true;
    } else {
        qDebug() << "Error deleting Offre from the database:" << query.lastError().text();
        return false;
    }
}
//upd

int Condidat::modifier() {

    QString idd = QString::number(id);
    QSqlQuery query;
    query.prepare("UPDATE CANDIDATS SET NOM = :nom, PRENOM = :prenom, EMAIL = :mail, CIN = :cin, "
                  "TELEPHONE = :tel  WHERE ID_CANDIDAT = :id");

    query.bindValue(":id", idd);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":mail", email);
    query.bindValue(":cin", cin);
    query.bindValue(":tel", tel);


    if (query.exec()) {
        qDebug() << "Candidate with ID" << id << "updated successfully.";
        return 1; // Update successful
    } else {
        qDebug() << "Error updating candidate in the database:" << query.lastError().text();
        return 2; // Error during update
    }
}



