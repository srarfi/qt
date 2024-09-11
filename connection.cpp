#include "connection.h"
#include "offre.h"
Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("recrutment");
db.setUserName("srarfi");//inserer nom de l'utilisateur
db.setPassword("ridha");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;



    return  test;
}
