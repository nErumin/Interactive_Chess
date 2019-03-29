#ifndef INFORMATIONMODAL_H
#define INFORMATIONMODAL_H

#include <QDialog>
#include <QPushButton>
#include <string>
#include <QWidget>
#include <QLabel>

class InformationModal : public QDialog
{
public:
    InformationModal(QWidget* parent);
private:
    QPushButton okButton;
    QLabel infoIconLabel;
    QLabel infoMessage;
};

#endif // INFORMATIONMODAL_H
