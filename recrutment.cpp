#include "recrutment.h"
#include "ui_recrutment.h"
#include <QSqlError>
#include "condidat.h"
#include "offre.h"
#include <QtWidgets/QToolTip>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QTime>
#include <cstdlib>

using namespace QtCharts;

recrutment::recrutment(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::recrutment)
{
    ui->setupUi(this);
}

recrutment::~recrutment()
{
    delete ui;
}



// deplacment entre offre et condidat
void recrutment::on_recrutment_2_clicked()
{
     ui->stackedWidget_1->setCurrentIndex(0);
}

void recrutment::on_ajouter_v_4_clicked()
{
     ui->stackedWidget_1->setCurrentIndex(1);
}
// deplacment entre crud offre

void recrutment::on_add_rec_clicked()
{
    QStackedWidget* stackedWidget_1= ui->stackedWidget_2;
    stackedWidget_1->setCurrentIndex(0);
}



void recrutment::on_delete_rec_clicked()
{
    QStackedWidget* stackedWidget_1= ui->stackedWidget_2;
    stackedWidget_1->setCurrentIndex(2);
}

void recrutment::on_update_rec_clicked()
{
    QStackedWidget* stackedWidget_1= ui->stackedWidget_2;
    stackedWidget_1->setCurrentIndex(3);
}
//offre


void recrutment::on_read_rec_clicked()
{
    QStackedWidget* stackedWidget_1= ui->stackedWidget_2;
    stackedWidget_1->setCurrentIndex(1);
    ui->table->clearContents();  // Efface le contenu des cellules
    ui->table->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    QSqlQuery query;
    QList<Offre> listeoffre;
    query.prepare("SELECT * FROM OFFRES_EMPLOI ORDER BY ID_OFFRE");

    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table->setColumnCount(8);

        while (query.next()) {
            Offre offre ;

            // Fetch data from the query and populate the 'sponsor' instance
            offre.setid(query.value("ID_OFFRE").toInt());
            offre.settitre(query.value("TITRE").toString());
            offre.setsalaire(query.value("SALAIRE").toFloat());
            offre.setdescription(query.value("DESCRIPTION").toString());
            offre.setdate_pub(query.value("DATE_PUBLICATION").toDate());
            offre.setdate_exp(query.value("DATE_EXPIRATION").toDate());
            offre.setidcondidat(query.value("ID_CONDIDAT_O").toInt());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << offre.getid();
            qDebug() << "titre: " << offre.gettitre();
            qDebug() << "salaire: " << offre.getsalaire();
            qDebug() << "description: " << offre.getdescription();
            qDebug() << "date: " << offre.getdate_pub();
            qDebug() << " datee: " << offre.getdate_exp();
            qDebug() << " id offre condodat: " << offre.getidcondidat();

            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(offre.getid())));
            ui->table->setItem(i, 1, new QTableWidgetItem(offre.gettitre()));
            ui->table->setItem(i, 2, new QTableWidgetItem(offre.getdescription()));
            ui->table->setItem(i, 3, new QTableWidgetItem(offre.getdate_pub().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 4, new QTableWidgetItem(offre.getdate_exp().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 5, new QTableWidgetItem(QString::number(offre.getsalaire())));
            ui->table->setItem(i, 6, new QTableWidgetItem(QString::number(offre.getidcondidat())));

            listeoffre.append(offre);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }
}











// exportation en pdf
void recrutment::on_export_rec_clicked()
{
    // Créer le contenu HTML
    QString htmlContent = "<html>\n"
                          "<head>\n"
                          "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                          "</head>\n"
                          "<body bgcolor=#ffffff link=#5000A0>\n"
                          "<center> <H1>La Liste des offre des emplois </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n"
                          "<center>\n"
                          "<table border='1'>\n"
                          "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>"
                          "<th>ID</th>"
                          "<th>Titre d'offre</th>"
                          "<th>Description</th>"
                          "<th>Salaire</th>"
                          "<th>Date publication</th>"
                          "<th>Date expiration</th>"
                          "</tr></thead>\n";

    // Récupérer les données de la base de données et les ajouter au contenu HTML
    QSqlQuery query("SELECT * FROM OFFRES_EMPLOI ORDER BY ID_OFFRE");
    int rowNum = 1;
    while (query.next()) {
        htmlContent += "<tr>";
        htmlContent += "<td bkcolor=0>" + QString::number(rowNum++) + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("ID_OFFRE").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("TITRE").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("DESCRIPTION").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("SALAIRE").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("DATE_PUBLICATION").toDate().toString("dd/MM/yyyy") + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("DATE_EXPIRATION").toDate().toString("dd/MM/yyyy") + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("ID_CONDIDAT_O").toString() + "</td>";
        htmlContent += "</tr>\n";
    }
    htmlContent += "</table> </center>\n"
                   "</body>\n"
                   "</html>\n";

    // Demander à l'utilisateur le nom du fichier et l'emplacement pour enregistrer le PDF
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Sauvegarder en PDF", QDir::homePath(), "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return; // L'utilisateur a annulé la boîte de dialogue

    // Vérifier si l'utilisateur a fourni l'extension .pdf, sinon l'ajouter
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Créer le fichier PDF
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QPdfWriter pdfWriter(&file);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));
        pdfWriter.setResolution(300); // DPI

        // Écrire le contenu HTML dans le PDF
        QTextDocument doc;
        doc.setHtml(htmlContent);
        doc.print(&pdfWriter);

        file.close();
        qDebug() << "Le fichier PDF a été créé avec succès à l'emplacement:" << fileName;
    } else {
        qDebug() << "Erreur : Impossible de créer le fichier PDF";
    }

}

// crud
//add
void recrutment::on_ajouter_clicked()
{
    QString titre = ui->titre_add->text();
    float salaire = ui->salaire_add->text().toFloat();
    QDate date_pub = ui->date_pub_add->date();
    QDate date_exp = ui->date_exp_add->date();
    QString description = ui->description_add->toPlainText();
    int id_condidat = ui->id_candidat_offre_addd->text().toInt();

    Offre o(titre, salaire , date_pub, date_exp, description,id_condidat);

    bool valid=o.test_champs_Offre();
    if (valid){
    bool test = o.ajouter();
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("ok"),
            QObject::tr("Ajout effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
        else {
            QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                QObject::tr("Ajout non effectué. Failed.\nClick Cancel to exit."), QMessageBox::Cancel);
            }
        }



}


//chercher delate
//111111
void recrutment::on_chercher_supp__offre_clicked()
{

    int id = ui->id_sup_offre->text().toInt();
    QString titre = ui->titre_offre_sup->text();
    Offre o(id,id);
    bool test =o.chercher();
    o.settitre(titre);
    bool test2= o.test_action_upd();
    if (test2){
    if ( test) {

        qDebug() << "Data extracted from UI fields:";
        qDebug() << "id: " << o.getid();
        qDebug() << "titre: " << o.gettitre();
        qDebug() << "salaire: " << o.getsalaire();
        qDebug() << "Date Debut: " << o.getdate_pub();
        qDebug() << "Date Fin: " << o.getdate_exp();
        qDebug() << "Description: " << o.getdescription();
        qDebug() << "id condidat: " << o.getidcondidat();

    ui->titre_offre_sup->setText(o.gettitre());
    ui->salaire_sup_offre->setText(o.getdescription());
    ui->description_sup_offre->setPlainText(o.getdescription());
    ui->date_pub_offre->setText(o.getdate_pub().toString("yyyy-MM-dd"));
    ui->date_exp_supp_offre->setText(o.getdate_exp().toString("yyyy-MM-dd"));
    ui->salaire_sup_offre_2->setText(QString::number(o.getidcondidat()));



}
    else {
        QMessageBox::critical(nullptr, QObject::tr("not found"),
        QObject::tr("sponsor not found . Failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->id_sup_offre->clear();
    }
    }
}
//delate
//2222222222222222
void recrutment::on_suprimer_offre_clicked()
{
    int id = ui->id_sup_offre->text().toInt();
    Offre o(id);
    o.chercher();
    if (o.supprimer()) {
            // Deletion was successful
            qDebug() << "Sponsor with ID" << id << "deleted successfully.";
            ui->titre_offre_sup->clear();
            ui->salaire_sup_offre->clear();
            ui->date_pub_offre->clear();
            ui->date_exp_supp_offre->clear();
            ui->description_sup_offre->clear();
            ui->id_sup_offre->clear();
            ui->salaire_sup_offre_2->clear();
        } else {
            // Handle the case where deletion failed
            qDebug() << "Error deleting sponsor with ID" << id;
            // You may want to show an error message or log the error
        }
}








//rehcherche buy id
void recrutment::on_id_read_3_textChanged()
{

    ui->table->clearContents();  // Efface le contenu des cellules
    ui->table->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    int id = ui->id_read_3->text().toInt();
    QString idd = QString::number(id);
    QSqlQuery query;
    QList<Offre> listeSponsors;
    query.prepare("SELECT * FROM OFFRES_EMPLOI WHERE ID_OFFRE LIKE :id");
    query.bindValue(":id", "%" + idd + "%");


    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table->setColumnCount(8);

        while (query.next()) {
            Offre offre ;

            // Fetch data from the query and populate the 'sponsor' instance
            offre.setid(query.value("ID_OFFRE").toInt());
            offre.settitre(query.value("TITRE").toString());
            offre.setsalaire(query.value("SALAIRE").toFloat());
            offre.setdescription(query.value("DESCRIPTION").toString());
            offre.setdate_pub(query.value("DATE_PUBLICATION").toDate());
            offre.setdate_exp(query.value("DATE_EXPIRATION").toDate());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << offre.getid();
            qDebug() << "titre: " << offre.gettitre();
            qDebug() << "salaire: " << offre.getsalaire();
            qDebug() << "description: " << offre.getdescription();
            qDebug() << "date: " << offre.getdate_pub();
            qDebug() << " datee: " << offre.getdate_exp();

            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(offre.getid())));
            ui->table->setItem(i, 1, new QTableWidgetItem(offre.gettitre()));
            ui->table->setItem(i, 2, new QTableWidgetItem(offre.getdescription()));
            ui->table->setItem(i, 3, new QTableWidgetItem(offre.getdate_pub().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 4, new QTableWidgetItem(offre.getdate_exp().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 5, new QTableWidgetItem(QString::number(offre.getsalaire())));

            listeSponsors.append(offre);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }
}

//recherche upd

void recrutment::on_chercher_offre_clicked()
{
    int id = ui->id_mod_offre->text().toInt();
    Offre o(id,id);
    bool test =o.chercher();
    bool test2= o.test_action_upd();
    if (test2){
    if ( test) {

        qDebug() << "Data extracted from UI fields:";
        qDebug() << "id: " << o.getid();
        qDebug() << "titre: " << o.gettitre();
        qDebug() << "salaire: " << o.getsalaire();
        qDebug() << "Date Debut: " << o.getdate_pub();
        qDebug() << "Date Fin: " << o.getdate_exp();
        qDebug() << "Description: " << o.getdescription();
        qDebug() << "id_c: " << o.getidcondidat();

        ui->titre_mod_offre->setText(o.gettitre());
        ui->salaire_offre_mod->setText(QString::number(o.getsalaire()));
        ui->desc_offre_mod->setPlainText(o.getdescription());
        ui->date_pub_offre_mod->setText(o.getdate_pub().toString("yyyy-MM-dd"));
        ui->date_exp_offre_mod->setText(o.getdate_exp().toString("yyyy-MM-dd"));
        ui->id_offre_candidat_add->setText(QString::number(o.getidcondidat()));

}
    else {
        QMessageBox::critical(nullptr, QObject::tr("not found"),
        QObject::tr("sponsor not found . Failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->id_mod_offre->clear();
    }
    }
}
//upd
void recrutment::on_mod_offre_clicked()
{
    int id = ui->id_mod_offre->text().toInt();
    int test_id=id;
    QString titre = ui->titre_mod_offre->text();
    QString description = ui->desc_offre_mod->toPlainText();
    float salaire = ui->salaire_offre_mod->text().toFloat();
    QString date_pub = ui->date_pub_offre_mod->text();
    QString date_exp = ui->date_exp_offre_mod->text();
    int id_c = ui->id_offre_candidat_add->text().toInt();

    QDate date_puub = QDate::fromString(date_pub, "yyyy-MM-dd");
    QDate date_exxp = QDate::fromString(date_exp, "yyyy-MM-dd");

    Offre o(id ,salaire, titre, description, date_puub, date_exxp,id_c);
    bool test =o.test_champs_Offre();
        if(id==test_id){
            qDebug() << " ID" << id;
            qDebug() << "test_ID" << test_id;
            if (test ){
                o.modifier();
            }
        }
        else{
            QMessageBox::critical(nullptr, QObject::tr("Error"),
            QObject::tr("vous naves pas le droit de modifierle id.\nClick Cancel to exit."), QMessageBox::Cancel);
            ui->id_mod_offre->setText(QString::number(test));
        }



}




//condidat







//deplacment entre crud condidat

void recrutment::on_add_condidat_clicked()
{

    QStackedWidget* stackedWidget_1= ui->stackedWidget_3;
    stackedWidget_1->setCurrentIndex(0);
}

void recrutment::on_afficher_condiat_clicked()
{

    QStackedWidget* stackedWidget_1= ui->stackedWidget_3;
    stackedWidget_1->setCurrentIndex(1);
    ui->table_2->clearContents();  // Efface le contenu des cellules
    ui->table_2->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    QSqlQuery query;
    QList<Condidat> listeSponsors;
    query.prepare("SELECT * FROM CANDIDATS ORDER BY ID_CANDIDAT ");

    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table_2->setColumnCount(8);

        while (query.next()) {
            Condidat c;

            // Fetch data from the query and populate the 'sponsor' instance
            c.setid(query.value("ID_CANDIDAT").toInt());
            c.setnom(query.value("NOM").toString());
            c.setprenom(query.value("PRENOM").toString());
            c.setemail(query.value("EMAIL").toString());
            c.settel(query.value("TELEPHONE").toString());
            c.setcin(query.value("CIN").toString());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << c.getid();
            qDebug() << "nom: " << c.getnom();
            qDebug() << "prenom: " << c.getprenom();
            qDebug() << "email: " << c.getemail();
            qDebug() << "cin: " << c.getcin();
            qDebug() << " tel: " << c.gettel();


            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table_2->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table_2->setItem(i, 0, new QTableWidgetItem(QString::number(c.getid())));
            ui->table_2->setItem(i, 1, new QTableWidgetItem(c.getnom()));
            ui->table_2->setItem(i, 2, new QTableWidgetItem(c.getprenom()));
            ui->table_2->setItem(i, 3, new QTableWidgetItem(c.getemail()));
            ui->table_2->setItem(i, 4, new QTableWidgetItem(c.gettel()));
            ui->table_2->setItem(i, 5, new QTableWidgetItem(c.getcin()));

            listeSponsors.append(c);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }
}

void recrutment::on_suprimer_condidat_clicked()
{

    QStackedWidget* stackedWidget_1= ui->stackedWidget_3;
    stackedWidget_1->setCurrentIndex(2);
}

void recrutment::on_modifer_condidat_clicked()
{

    QStackedWidget* stackedWidget_1= ui->stackedWidget_3;
    stackedWidget_1->setCurrentIndex(3);
}
void recrutment::on_exporter_condidat_clicked()
{
    // Créer le contenu HTML
    QString htmlContent = "<html>\n"
                          "<head>\n"
                          "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                          "</head>\n"
                          "<body bgcolor=#ffffff link=#5000A0>\n"
                          "<center> <H1>La Liste des Candidats</H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n"
                          "<center>\n"
                          "<table border='1'>\n"
                          "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>"
                          "<th>ID Candidat</th>"
                          "<th>Nom</th>"
                          "<th>Prénom</th>"
                          "<th>Email</th>"
                          "<th>CIN</th>"
                          "<th>Téléphone</th>"
                          "</tr></thead>\n";

    // Récupérer les données de la base de données et les ajouter au contenu HTML
    QSqlQuery query("SELECT * FROM CANDIDATS ORDER BY ID_CANDIDAT");
    int rowNum = 1;
    while (query.next()) {
        htmlContent += "<tr>";
        htmlContent += "<td bkcolor=0>" + QString::number(rowNum++) + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("ID_CANDIDAT").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("NOM").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("PRENOM").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("EMAIL").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("CIN").toString() + "</td>";
        htmlContent += "<td bkcolor=0>" + query.value("TELEPHONE").toString() + "</td>";
        htmlContent += "</tr>\n";
    }
    htmlContent += "</table> </center>\n"
                   "</body>\n"
                   "</html>\n";

    // Demander à l'utilisateur le nom du fichier et l'emplacement pour enregistrer le PDF
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Sauvegarder en PDF", QDir::homePath(), "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return; // L'utilisateur a annulé la boîte de dialogue

    // Vérifier si l'utilisateur a fourni l'extension .pdf, sinon l'ajouter
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Créer le fichier PDF
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QPdfWriter pdfWriter(&file);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));
        pdfWriter.setResolution(300); // DPI

        // Écrire le contenu HTML dans le PDF
        QTextDocument doc;
        doc.setHtml(htmlContent);
        doc.print(&pdfWriter);

        file.close();
        qDebug() << "Le fichier PDF a été créé avec succès à l'emplacement:" << fileName;
    } else {
        qDebug() << "Erreur : Impossible de créer le fichier PDF";
    }
}


// crud condidat




void recrutment::on_ajouter_con_clicked()
    {
        QString nom = ui->nom_condidat_add->text();
        QString prenom = ui->prenom_condidat_add->text();
        QString email = ui->email_dindiat_add->text();
        QString cin = ui->cin_condidat_add->text();
        QString tel = ui->tel_condidat_add->text();
        Condidat c(nom, prenom , email, cin , tel );
        bool valid=c.test_champs_condidat();
        if (valid){
        int result = c.ajouter();

        if (result == 1) {  // Successful addition
            QMessageBox::information(nullptr, QObject::tr("Succès"),
                QObject::tr("Ajout effectué avec succès.\nClick Cancel to exit."), QMessageBox::Cancel);
        } else {  // Error during insertion
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Une erreur s'est produite lors de l'ajout.\nAjout non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
        }

            }


}

void recrutment::on_id_read_6_textChanged()
{
    // Clear the table before populating
    ui->table_2->clearContents();
    ui->table_2->setRowCount(0);   // Reset row count to zero

    int i = 0; // Initialize row counter
    int id = ui->id_read_6->text().toInt(); // Get ID from the text field
    QString idd = QString::number(id);
    QSqlQuery query;
    QList<Condidat> listeSponsors;  // Corrected the class name

    // Prepare the query to search for candidates matching the ID
    query.prepare("SELECT * FROM CANDIDATS WHERE ID_CANDIDAT LIKE :id");
    query.bindValue(":id", "%" + idd + "%");

    if (query.exec()) {
        // Ensure the table has the correct number of columns
        ui->table_2->setColumnCount(7); // 7 columns since you're showing 7 fields

        while (query.next()) {
            Condidat c;

            // Fetch data from the query and populate the 'Condidat' instance
            c.setid(query.value("ID_CANDIDAT").toInt());
            c.setnom(query.value("NOM").toString());
            c.setprenom(query.value("PRENOM").toString());
            c.setemail(query.value("EMAIL").toString());
            c.settel(query.value("TELEPHONE").toString());
            c.setcin(query.value("CIN").toString());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << c.getid();
            qDebug() << "nom: " << c.getnom();
            qDebug() << "prenom: " << c.getprenom();
            qDebug() << "email: " << c.getemail();
            qDebug() << "cin: " << c.getcin();
            qDebug() << "tel: " << c.gettel();

            // Add a new row for each candidate in the QTableWidget
            ui->table_2->insertRow(i);

            // Populate cells in the row with the candidate data
            ui->table_2->setItem(i, 0, new QTableWidgetItem(QString::number(c.getid())));
            ui->table_2->setItem(i, 1, new QTableWidgetItem(c.getnom()));
            ui->table_2->setItem(i, 2, new QTableWidgetItem(c.getprenom()));
            ui->table_2->setItem(i, 3, new QTableWidgetItem(c.getemail()));
            ui->table_2->setItem(i, 4, new QTableWidgetItem(c.gettel()));
            ui->table_2->setItem(i, 5, new QTableWidgetItem(c.getcin()));// Correct column for ID_OFFRE

            listeSponsors.append(c); // Add candidate to the list
            i++; // Increment row counter
        }
    } else {
        // Print an error if query execution failed
        qDebug() << "Error fetching all candidates from the database:" << query.lastError().text();
    }
}
//33333
void recrutment::on_cherhcer_11_clicked()
{
    int id = ui->id_del_con->text().toInt();
    Condidat c(id,id);
    bool test =c.chercher();
    bool test2= c.test_action_upd();
    if (test2){
    if ( test) {

        qDebug() << "Data extracted from UI fields:";
        qDebug() << "id: " << c.getid();
        qDebug() << "nom: " << c.getnom();
        qDebug() << "prenom: " << c.getprenom();
        qDebug() << "email: " << c.getemail();
        qDebug() << "cin: " << c.getcin();
        qDebug() << "tel: " << c.gettel();



    ui->nom_del_con->setText(c.getnom());
    ui->prenom_del_con->setText(c.getprenom());
    ui->mail_del_con->setText(c.getemail());
    ui->cin_del_con->setText(c.getcin());
    ui->tel_del_con->setText(c.gettel());
}

    else {
        QMessageBox::critical(nullptr, QObject::tr("not found"),
        QObject::tr("sponsor not found . Failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->id_sup_offre->clear();
    }
}
}
//delate

//4444
void recrutment::on_suprimer_6_clicked()
{   
    int id = ui->id_del_con->text().toInt();
    Condidat c(id);
    c.chercher();

        if (c.supprimer()) {
            // Deletion was successful
            qDebug() << "condidat with ID" << id << "deleted successfully.";
            ui->id_del_con->clear();
            ui->nom_del_con->clear();
            ui->prenom_del_con->clear();
            ui->mail_del_con->clear();
            ui->tel_del_con->clear();
            ui->cin_del_con->clear();
        } else {
            // Handle the case where deletion failed
            qDebug() << "Error deleting sponsor with ID" << id;
            // You may want to show an error message or log the error
        }
    //
}

void recrutment::on_cherhcer_12_clicked()
{

    int id = ui->lineEdit_id_upd_6->text().toInt();
    Condidat c(id,id);
    bool test =c.chercher();
    bool test2= c.test_action_upd();
    if (test2){
    if ( test) {

        qDebug() << "Data extracted from UI fields:";
        qDebug() << "id: " << c.getid();
        qDebug() << "nom: " << c.getnom();
        qDebug() << "prenom: " << c.getprenom();
        qDebug() << "email: " << c.getemail();
        qDebug() << "cin: " << c.getcin();
        qDebug() << "tel: " << c.gettel();


    ui->lineEdit_nom_upd_6->setText(c.getnom());
    ui->lineEdit_prenom_upd->setText(c.getprenom());
    ui->lineEdit_mail_upd_6->setText(c.getemail());
    ui->lineEdit_cin_upd_6->setText(c.getcin());
    ui->lineEdit_tel_upd_6->setText(c.gettel());
    //ui->line
}

    else {
        QMessageBox::critical(nullptr, QObject::tr("not found"),
        QObject::tr("sponsor not found . Failed.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->id_sup_offre->clear();
    }
}

}
void recrutment::on_modifier_6_clicked()
{
    int id = ui->lineEdit_id_upd_6->text().toInt();
    QString nom = ui->lineEdit_nom_upd_6->text();
    QString prenom = ui->lineEdit_prenom_upd->text();
    QString mail = ui->lineEdit_mail_upd_6->text();
    QString cin = ui->lineEdit_cin_upd_6->text();
    QString tel = ui->lineEdit_tel_upd_6->text();


    Condidat c(id, nom, prenom, mail, cin, tel);

    qDebug() << "Data extracted from UI fields:";
    qDebug() << "id: " << c.getid();
    qDebug() << "nom: " << c.getnom();
    qDebug() << "prenom: " << c.getprenom();
    qDebug() << "email: " << c.getemail();
    qDebug() << "cin: " << c.getcin();
    qDebug() << "tel: " << c.gettel();

    if (c.test_champs_condidat()) {
        int result = c.modifier();

        // Check the result of the modifier() function
        if (result == 1) {
            QMessageBox::information(nullptr, QObject::tr("Succès"),
                QObject::tr("Modification effectuée avec succès.\nClick Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                QObject::tr("Une erreur s'est produite lors de la modification.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur de validation"),
            QObject::tr("Les champs ne sont pas valides.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}


void recrutment::on_add_rec_2_clicked()
{
    ui->table->clearContents();  // Efface le contenu des cellules
    ui->table->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    QSqlQuery query;
    QList<Offre> listeSponsors;
    query.prepare("SELECT * FROM OFFRES_EMPLOI ORDER BY SALAIRE");

    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table->setColumnCount(8);

        while (query.next()) {
            Offre offre ;

            // Fetch data from the query and populate the 'sponsor' instance
            offre.setid(query.value("ID_OFFRE").toInt());
            offre.settitre(query.value("TITRE").toString());
            offre.setsalaire(query.value("SALAIRE").toFloat());
            offre.setdescription(query.value("DESCRIPTION").toString());
            offre.setdate_pub(query.value("DATE_PUBLICATION").toDate());
            offre.setdate_exp(query.value("DATE_EXPIRATION").toDate());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << offre.getid();
            qDebug() << "titre: " << offre.gettitre();
            qDebug() << "salaire: " << offre.getsalaire();
            qDebug() << "description: " << offre.getdescription();
            qDebug() << "date: " << offre.getdate_pub();
            qDebug() << " datee: " << offre.getdate_exp();

            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(offre.getid())));
            ui->table->setItem(i, 1, new QTableWidgetItem(offre.gettitre()));
            ui->table->setItem(i, 2, new QTableWidgetItem(offre.getdescription()));
            ui->table->setItem(i, 3, new QTableWidgetItem(offre.getdate_pub().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 4, new QTableWidgetItem(offre.getdate_exp().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 5, new QTableWidgetItem(QString::number(offre.getsalaire())));
            listeSponsors.append(offre);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }

}

void recrutment::on_update_rec_2_clicked(){
    QSqlQuery query;
        query.prepare("SELECT TO_CHAR(DATE_PUBLICATION, 'YYYY-MM') AS Mois, COUNT(*) AS NombreOffres "
                      "FROM OFFRES_EMPLOI GROUP BY TO_CHAR(DATE_PUBLICATION, 'YYYY-MM') "
                      "ORDER BY TO_CHAR(DATE_PUBLICATION, 'YYYY-MM')");

        if (query.exec()) {
            QChart *chart = new QChart();
            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);
            chart->setTitle("Nombre d'offres par mois");

            QBarSeries *series = new QBarSeries();
            QStringList categories;

            // Seed the random number generator for color
            qsrand(QTime::currentTime().msec());

            while (query.next()) {
                QString mois = query.value("Mois").toString();
                int nombreOffres = query.value("NombreOffres").toInt();

                QBarSet *barSet = new QBarSet(mois);

                // Generate a random color for each bar set
                QColor color = QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256);
                barSet->setColor(color);

                *barSet << nombreOffres;
                series->append(barSet);

                categories << mois;
            }

            // Configure x-axis
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            axisX->setTitleText("Mois");
            chart->addAxis(axisX, Qt::AlignBottom);

            // Configure y-axis
            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("Nombre d'offres");
            axisY->setLabelFormat("%d");
            axisY->setTickCount(10); // Adjust number of ticks
            chart->addAxis(axisY, Qt::AlignLeft);

            series->attachAxis(axisX);
            series->attachAxis(axisY);

            chart->addSeries(series);
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->setBackgroundBrush(QBrush(QColor(240, 240, 240)));

            // Tooltips setup
            for (auto barSet : series->barSets()) {
                connect(barSet, &QBarSet::hovered, [=](bool status, int index) {
                    if (status) {
                        QToolTip::showText(QCursor::pos(),
                                           QString("%1: %2 offres")
                                               .arg(barSet->label())
                                               .arg(barSet->at(index)));
                    } else {
                        QToolTip::hideText();
                    }
                });
            }

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setRubberBand(QChartView::HorizontalRubberBand);

            QMainWindow *window = new QMainWindow();
            window->setCentralWidget(chartView);
            window->resize(900, 600);
            window->show();
        } else {
            qDebug() << "Erreur lors de la récupération des statistiques depuis la base de données:" << query.lastError().text();
        }
    }



void recrutment::on_add_rec_3_clicked()
{

    ui->table->clearContents();  // Efface le contenu des cellules
    ui->table->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    QSqlQuery query;
    QList<Offre> listeSponsors;
    query.prepare("SELECT * FROM OFFRES_EMPLOI ORDER BY DATE_EXPIRATION");

    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table->setColumnCount(8);

        while (query.next()) {
            Offre offre ;

            // Fetch data from the query and populate the 'sponsor' instance
            offre.setid(query.value("ID_OFFRE").toInt());
            offre.settitre(query.value("TITRE").toString());
            offre.setsalaire(query.value("SALAIRE").toFloat());
            offre.setdescription(query.value("DESCRIPTION").toString());
            offre.setdate_pub(query.value("DATE_PUBLICATION").toDate());
            offre.setdate_exp(query.value("DATE_EXPIRATION").toDate());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << offre.getid();
            qDebug() << "titre: " << offre.gettitre();
            qDebug() << "salaire: " << offre.getsalaire();
            qDebug() << "description: " << offre.getdescription();
            qDebug() << "date: " << offre.getdate_pub();
            qDebug() << " datee: " << offre.getdate_exp();

            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(offre.getid())));
            ui->table->setItem(i, 1, new QTableWidgetItem(offre.gettitre()));
            ui->table->setItem(i, 2, new QTableWidgetItem(offre.getdescription()));
            ui->table->setItem(i, 3, new QTableWidgetItem(offre.getdate_pub().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 4, new QTableWidgetItem(offre.getdate_exp().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 5, new QTableWidgetItem(QString::number(offre.getsalaire())));
            listeSponsors.append(offre);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }
}

void recrutment::on_add_rec_4_clicked()
{

    ui->table->clearContents();  // Efface le contenu des cellules
    ui->table->setRowCount(0);   // Réinitialise le nombre de lignes à zéro
    int i = 0;
    QSqlQuery query;
    QList<Offre> listeSponsors;
    query.prepare("SELECT * FROM OFFRES_EMPLOI ORDER BY DATE_PUBLICATION");

    if (query.exec()) {
        // Assurez-vous que le nombre de colonnes est suffisant pour toutes les données
        ui->table->setColumnCount(8);

        while (query.next()) {
            Offre offre ;

            // Fetch data from the query and populate the 'sponsor' instance
            offre.setid(query.value("ID_OFFRE").toInt());
            offre.settitre(query.value("TITRE").toString());
            offre.setsalaire(query.value("SALAIRE").toFloat());
            offre.setdescription(query.value("DESCRIPTION").toString());
            offre.setdate_pub(query.value("DATE_PUBLICATION").toDate());
            offre.setdate_exp(query.value("DATE_EXPIRATION").toDate());

            qDebug() << "Data extracted from UI fields:";
            qDebug() << "id: " << offre.getid();
            qDebug() << "titre: " << offre.gettitre();
            qDebug() << "salaire: " << offre.getsalaire();
            qDebug() << "description: " << offre.getdescription();
            qDebug() << "date: " << offre.getdate_pub();
            qDebug() << " datee: " << offre.getdate_exp();

            // Ajoutez une nouvelle ligne pour chaque sponsor dans le QTableWidget
            ui->table->insertRow(i);

            // Populate les cellules de la ligne avec les données de 'sponsor'
            ui->table->setItem(i, 0, new QTableWidgetItem(QString::number(offre.getid())));
            ui->table->setItem(i, 1, new QTableWidgetItem(offre.gettitre()));
            ui->table->setItem(i, 2, new QTableWidgetItem(offre.getdescription()));
            ui->table->setItem(i, 3, new QTableWidgetItem(offre.getdate_pub().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 4, new QTableWidgetItem(offre.getdate_exp().toString("yyyy-MM-dd")));
            ui->table->setItem(i, 5, new QTableWidgetItem(QString::number(offre.getsalaire())));
            listeSponsors.append(offre);
            i++;
        }
    } else {
        qDebug() << "Error fetching all sponsors from the database:" << query.lastError().text();
    }
}
