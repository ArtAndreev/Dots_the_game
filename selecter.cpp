#include "selecter.h"


Selecter::Selecter(QWidget *)
{
    createSelecterWindow();
    setAttribute(Qt::WA_QuitOnClose);
}

Selecter::~Selecter()
{
    buttons.clear();
}

void Selecter::createSelecterWindow()
{
    grid = new QGridLayout();
    buttons.append(new QPushButton("line"));
    buttons.append(new QPushButton("random"));
    buttons.append(new QPushButton("color"));
    connect(buttons[0], &QPushButton::clicked, this, &Selecter::line_clicked);
    connect(buttons[1], &QPushButton::clicked, this, &Selecter::random_clicked);
    connect(buttons[2], &QPushButton::clicked, this, &Selecter::color_clicked);

    for (size_t i = 0; i < buttons.count(); i++)
    {
        buttons[i]->setFixedSize(100, 200);
        grid->addWidget(buttons[i], 0, i);
    }
    this->setLayout(grid);
}

void Selecter::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Selecter::line_clicked()
{
    emit selected(0);
    this->setEnabled(0);
    this->setVisible(0);
}

void Selecter::random_clicked()
{
    emit selected(1);
    this->setEnabled(0);
    this->setVisible(0);


}

void Selecter::color_clicked()
{
    emit selected(2);
    this->setEnabled(0);
    this->setVisible(0);

}


