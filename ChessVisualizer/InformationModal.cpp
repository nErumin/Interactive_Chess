#include "InformationModal.h"
#include <string>

InformationModal::InformationModal(QWidget* parent)
    : QDialog{ parent },
      okButton{ this },
      infoIconLabel{ this },
      infoMessage{ this }
{
    setFixedSize(500, 150);
    setWindowTitle("Information");
    setWindowIcon(QIcon(":/resource/information.png"));

    okButton.setGeometry(410, 120, 80, 25);
    okButton.setText("OK");

    infoIconLabel.setGeometry(20, 15, 30, 30);
    infoMessage.setGeometry(65, 20, 300, 70);

    QPixmap iconPixmap{ ":/resource/information.png" };
    infoIconLabel.setPixmap(iconPixmap.scaled(infoIconLabel.width(), infoIconLabel.height(), Qt::KeepAspectRatio));

    infoMessage.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoMessage.setText("Empty");

    connect(&okButton, SIGNAL(clicked()), this, SLOT(close()));
}

void InformationModal::setMessageText(const std::string& text)
{
    infoMessage.setText(text.c_str());
}

void InformationModal::setModalTitle(const std::string& title)
{
    setWindowTitle(title.c_str());
}
