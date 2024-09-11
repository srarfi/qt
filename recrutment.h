#ifndef RECRUTMENT_H
#define RECRUTMENT_H

#include <QMainWindow>
#include "condidat.h"
#include "offre.h"


QT_BEGIN_NAMESPACE
namespace Ui { class recrutment; }
QT_END_NAMESPACE

class recrutment : public QMainWindow
{
    Q_OBJECT

public:
    recrutment(QWidget *parent = nullptr);
    ~recrutment();

private slots:
    void on_recrutment_2_clicked();

    void on_ajouter_v_4_clicked();

    void on_add_rec_clicked();

    void on_read_rec_clicked();

    void on_delete_rec_clicked();

    void on_update_rec_clicked();

    void on_export_rec_clicked();

    void on_ajouter_clicked();

    void on_chercher_supp__offre_clicked();

    void on_suprimer_offre_clicked();

    void on_id_read_3_textChanged();

    void on_chercher_offre_clicked();

    void on_mod_offre_clicked();

    void on_add_condidat_clicked();

    void on_afficher_condiat_clicked();

    void on_suprimer_condidat_clicked();

    void on_modifer_condidat_clicked();

    void on_exporter_condidat_clicked();

    void on_ajouter_con_clicked();

    void on_id_read_6_textChanged();

    void on_cherhcer_11_clicked();

    void on_suprimer_6_clicked();

    void on_cherhcer_12_clicked();

    void on_modifier_6_clicked();

    void on_add_rec_2_clicked();

    void on_update_rec_2_clicked();



    void on_add_rec_3_clicked();

    void on_add_rec_4_clicked();

private:
    Ui::recrutment *ui;
};
#endif // RECRUTMENT_H
