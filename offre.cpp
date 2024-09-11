#include "offre.h"
#include <QDate>
#include <QDateEdit>
#include <QDate>
#include <QDateEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>

Offre::Offre()
{
    id=0;
    titre = "";
    description = "";
    salaire = 0;
    date_pub = QDate(2000, 1, 1);  // Exemple : 1er janvier 2000
    date_exp = QDate(2000, 1, 1);    // Exemple : 1er janvier 2000
    id_condidat=0;

}
//setters
void Offre::setid(int n){id=n;}
void Offre::settitre(QString n){titre=n;}
void Offre::setsalaire(float n){salaire=n;}
void Offre::setdescription(QString n){description=n;}
void Offre::setdate_pub(QDate n){date_pub=n;}
void Offre::setdate_exp(QDate n){date_exp=n;}

//getters
int Offre::getid(){return id;}
QString Offre::gettitre(){return titre;}
float Offre::getsalaire(){return salaire;}
QDate Offre::getdate_exp(){return date_exp;}
QDate Offre::getdate_pub(){return date_pub;}
QString Offre::getdescription(){return description;}


//constructer
Offre::Offre (QString a,float b , QDate c,QDate d ,QString e,int f){
    this->titre=a;
    this->salaire=b;
    this->date_pub=c;
    this->date_exp=d;
    this->description=e;
    this->id_condidat=f;
}

Offre::Offre (int a,float b , QString c,QString d ,QDate e, QDate f,int g){
    this->id=a;
    this->salaire=b;
    this->titre=c;
    this->description=d;
    this->date_pub=e;
    this->date_exp=f;
    this->id_condidat=g;
}


// functions
//controle de saisie
bool Offre::test_champs_Offre() {
    if (titre.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le titre ne peut pas être vide.");
        return false;
    }
    if (!date_pub.isValid()) {
        QMessageBox::warning(nullptr, "Erreur", "La date de publication est invalide.");
        return false;
    }

    if (!date_exp.isValid()) {
        QMessageBox::warning(nullptr, "Erreur", "La date d'expiration est invalide.");
        return false;
    }

    if (date_exp <= date_pub) {
        QMessageBox::warning(nullptr, "Erreur", "La date d'expiration ne peut pas être antérieure à la date de publication.");
        return false;
    }

    QString salairee = QString::number(salaire);

    if (salairee.contains(QRegularExpression("[^0-9]"))) {
        QMessageBox::warning(nullptr, "Erreur", "Le salaire ne doit pas contenir de lettres ou caractères spéciaux.");
        return false;
    }

    if(salaire<=0){
        QMessageBox::warning(nullptr, "Erreur", "Le salaire nee doit pas etre null ou negative.");
        return false;
    }

    if (description.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La description ne peut pas être vide.");
        return false;
    }

    if (id_condidat <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID candidat ne doit pas être vide.");
        return false;
    }

    return true;
}



// recherche


bool Offre::chercher() {
    QString idd = QString::number(id);
    QSqlQuery query;
    query.prepare("SELECT * FROM OffRES_EMPLOI WHERE ID_Offre = :id");
    query.bindValue(":id", idd);

    if (query.exec() && query.next()) {
        // Fetch data from the query result and populate the Sponsor object
        titre = query.value("TITRE").toString();
        description = query.value("DESCRIPTION").toString();
        salaire = query.value("SALAIRE").toDouble();
        date_exp = query.value("DATE_EXPIRATION").toDate();
        date_pub = query.value("DATE_PUBLICATION").toDate();
        id_condidat = query.value("ID_CONDIDAT_O").toInt();
        return true; // Data found and populated successfully
    } else {
        qDebug() << "Error fetching sponsor from the database:" << query.lastError().text();
        return false; // Sponsor with the specified ID not found
    }
}

// nsitha hahahah
bool Offre::test_action(){
        // Check if 'name' is not empty
        if (titre.isEmpty()) {
             qDebug() << "titreeeeeeeeeeeeee: " << titre;
            QMessageBox::critical(nullptr, "Erreur", "Faites la recherche avant de continuer.");
            return false;
        }
        return true;
}


bool Offre::test_action_upd(){
    if (id ==0) {
        // Gérer le cas où la chaîne est vide
        QMessageBox::critical(nullptr, "Error", "ecrire le id pour faire la recherche.");
        return false;
    }

        return true;
    }


int Offre::testExistID(int idCandidat) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CANDIDATS WHERE ID_CANDIDAT = :idCandidat");
    query.bindValue(":idCandidat", idCandidat);  // No need to convert to QString

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return 0;  // Indicates an error in query execution
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0 ? 1 : 0;  // Return 1 if ID exists, otherwise 0
    }

    return 0;  // Default case if no rows found
}

// crud
// create
bool Offre::ajouter() {
    qDebug() << "Database Connection Status:" << QSqlDatabase::database().isOpen();

    QString salairee = QString::number(salaire);
    QString date_debb = date_pub.toString("yyyy-MM-dd");
    QString date_finn = date_exp.toString("yyyy-MM-dd");

    if (testExistID(id_condidat) == 1) {
        QSqlQuery query;
        query.prepare("INSERT INTO OffRES_EMPLOI (TITRE, DATE_PUBLICATION, DATE_EXPIRATION, SALAIRE, DESCRIPTION, ID_CONDIDAT_O) "
                      "VALUES (:titre, TO_DATE(:date, 'YYYY-MM-DD'), TO_DATE(:datee, 'YYYY-MM-DD'), :salaire, :description, :id_c)");

        query.bindValue(":titre", titre);
        query.bindValue(":date", date_debb);
        query.bindValue(":datee", date_finn);
        query.bindValue(":description", description);
        query.bindValue(":salaire", salairee);
        query.bindValue(":id_c", id_condidat);  // Use id_condidat as an integer

        qDebug() << "Data extracted from UI fields:";
        qDebug() << "titre: " << titre;
        qDebug() << "salaire: " << salairee;
        qDebug() << "description: " << description;
        qDebug() << "date: " << date_debb;
        qDebug() << "datee: " << date_finn;
        qDebug() << "id_c: " << id_condidat;

        // Execute the query to insert data into the database
        if (query.exec()) {
            qDebug() << "Offre added successfully!";
            return true;
        } else {
            qDebug() << "Error adding Offre to the database:" << query.lastError().text();
            return false;
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "L'ID candidat n'existe pas.");
    }

    return false;  // Ensure function always returns a value
}



//delate


bool Offre::supprimer() {
    QSqlQuery query;

    query.prepare("DELETE FROM OffreS_EMPLOI WHERE ID_Offre = :id");
    query.bindValue(":id", id);
    QString idString = QString::number(id);
    if (query.exec()) {
        qDebug() << "Offre with ID" << id << "deleted successfully.";
        return true;
    } else {
        qDebug() << "Error deleting Offre from the database:" << query.lastError().text();
        return false;
         }

}

//update
bool Offre::modifier() {
QSqlQuery query;
QString idd = QString::number(id);
QString id_c = QString::number(id_condidat);
QString salairee = QString::number(salaire);
QString date_debb = date_pub.toString("yyyy-MM-dd");
QString date_finn = date_exp.toString("yyyy-MM-dd");

if (testExistID(id_condidat) == 1) {
    query.prepare("UPDATE OffreS_EMPLOI SET TITRE = :titre, SALAIRE = :salaire, DESCRIPTION = :description, "
                  "DATE_PUBLICATION = TO_DATE(:date_debut, 'YYYY-MM-DD'), "
                  "DATE_EXPIRATION = TO_DATE(:date_fin, 'YYYY-MM-DD'), "
                  "ID_CONDIDAT_O = :id_c "
                  "WHERE ID_Offre = :id");

    query.bindValue(":id", idd);
    query.bindValue(":titre", titre);
    query.bindValue(":salaire", salairee);
    query.bindValue(":description", description);
    query.bindValue(":date_debut", date_debb);
    query.bindValue(":date_fin", date_finn);
    query.bindValue(":id_c", id_c);

    if (query.exec()) {
        qDebug() << "Offre with ID" << id << "updated successfully.";
        return true;
    } else {
        qDebug() << "Error updating Offre in the database:" << query.lastError().text();
        return false;
    }
} else {
    QMessageBox::critical(nullptr, "Error", "L'ID candidat n'existe pas.");
}

return false;  // Ensure function always returns a value
}





