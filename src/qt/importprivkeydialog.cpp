#include "importprivkeydialog.h"
#include "ui_importprivkeydialog.h"
#include "init.h"
#include "base58.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QErrorMessage>
#include <QClipboard>

using namespace std;

ImportPrivKeyDialog::ImportPrivKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportPrivKeyDialog)
{
    ui->setupUi(this);

    // place focus on the private key edit line
    ui->privKeyEdit->setFocus();
}

ImportPrivKeyDialog::~ImportPrivKeyDialog()
{
    delete ui;
}

void ImportPrivKeyDialog::on_cancelButton_clicked()
{
     reject();
}

void ImportPrivKeyDialog::on_OkButton_clicked()
{


    QString privKey = ui->privKeyEdit->text();

    string strSecret = privKey.toStdString();
    string strLabel = "";

    strLabel = ui-> addressLabelEdit->text().toStdString();

    // Whether to perform rescan after import
    bool fRescan = true;

    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);

    if (!fGood) {
//        QErrorMessage msgBox;
        QMessageBox msgBox;
        msgBox.setText("The key is invalid. Please provide a valid key.");
//        msgBox.showMessage("The key is invalid. Please provide a valid key.");
        msgBox.exec();
    }

    //if (!fGood) throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid private key");

    if (fWalletUnlockStakingOnly)
        {
           QMessageBox msgBox;
           msgBox.setText("Wallet is unlocked for staking only.");
           msgBox.exec();
       }
        //    throw JSONRPCError(RPC_WALLET_UNLOCK_NEEDED, "Wallet is unlocked for staking only.");

    CKey key = vchSecret.GetKey();
    CPubKey pubkey = key.GetPubKey();
    CKeyID vchAddress = pubkey.GetID();
    {
        LOCK2(cs_main, pwalletMain->cs_wallet);

        pwalletMain->MarkDirty();
        pwalletMain->SetAddressBookName(vchAddress, strLabel);

        // Don't throw error in case a key is already there
        if (pwalletMain->HaveKey(vchAddress))
        {

          QMessageBox msgBox;
          msgBox.setText("This key already exists in the wallet.");
          msgBox.exec();
          accept();
          return;
        }

        pwalletMain->mapKeyMetadata[vchAddress].nCreateTime = 1;

        if (!pwalletMain->AddKeyPubKey(key, pubkey))
        {
           QMessageBox msgBox;
           msgBox.setText("Error adding key to the wallet.");
           msgBox.exec();
           reject();
           return;
       }
        //        throw JSONRPCError(RPC_WALLET_ERROR, "Error adding key to wallet");

        // whenever a key is imported, we need to scan the whole chain
        pwalletMain->nTimeFirstKey = 1; // 0 would be considered 'no value'

        if (fRescan) {
            pwalletMain->ScanForWalletTransactions(pindexGenesisBlock, true);
            pwalletMain->ReacceptWalletTransactions();
        }

    }

    //return Value::null;

    accept();

}

void ImportPrivKeyDialog::on_pasteButton_clicked()
{
    // get the clipboard
    QString clipText = QApplication::clipboard()->text(QClipboard::Clipboard);

    // paste the text in the private key edit
    ui->privKeyEdit->setText(clipText);

    // jump to enter the label
    ui->addressLabelEdit->setFocus();

}
