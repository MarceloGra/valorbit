#ifndef IMPORTPRIVKEYDIALOG_H
#define IMPORTPRIVKEYDIALOG_H

#include <QDialog>

namespace Ui {
  class ImportPrivKeyDialog;
}

class ImportPrivKeyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportPrivKeyDialog(QWidget *parent = 0);
    ~ImportPrivKeyDialog();

private slots:
    void on_cancelButton_clicked();
    void on_OkButton_clicked();
    void on_pasteButton_clicked();

private:
    Ui::ImportPrivKeyDialog *ui;
};

#endif // IMPORTPRIVKEYDIALOG_H
