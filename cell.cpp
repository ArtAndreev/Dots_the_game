#include "cell.h"
#include <QApplication>
static const int CHIP_RADIUS = 25;
static const int interval = 12;

Cell::Cell(QWidget *parent) :
    QWidget(parent)
{
    blocked = false;
    destroing = false;
    pointer_chip = new Chip();
    in_combination = false;
    in_quadr = false;
    this->setFixedSize(CHIP_RADIUS + 2,CHIP_RADIUS + 2);
    current_radius = CHIP_RADIUS;
}

Cell::Cell(QWidget *parent, Chip chip, bool blocked)
    : QWidget(parent)
{
    this->blocked = blocked;
    destroing = false;
    pointer_chip = new Chip(chip);
    in_combination = false;
    in_quadr = false;
    this->setFixedSize(CHIP_RADIUS + 2,CHIP_RADIUS + 2);
    current_radius = CHIP_RADIUS;
}

Chip* Cell::get_chip()
{
    return pointer_chip;
}

void Cell::animated_random_chip()
{
     get_small();
     QTimer::singleShot(interval*14, this, &Cell::random_chip);
     QTimer::singleShot(interval*14, this, &Cell::get_big);//,skj 14
     return;
}

//new
//O -> Y, B py
//Y -> P, R //ok
//B -> Y, P //ok
//R -> O, B by
//P -> O, R br

//yy, oo, bb, rr, pp

//Y = 0, O = 1, R = 2, B = 3, P = 4

//0 -> 1, 2
//1 -> 2, 3
//2 -> 3, 4
//3 -> 4, 0    3+1 = 4 3+2 = 5
//4 -> 0, 1    4+1 = 5 4+2 = 6


bool Cell::is_blocked()
{
    return blocked;
}

bool Cell::is_in_combination()
{
    return in_combination;
}

bool Cell::is_in_quadr()
{
    return in_quadr;
}

void Cell::set_quadr_flag(bool value)
{
    in_quadr = value;
}

void Cell::paintEvent(QPaintEvent *event)
{

        Q_UNUSED(event);
        //qDebug("paintEvent");
        QPainter painter(this);
        update_chip_model(&painter);

        painter.setRenderHint(QPainter::Antialiasing, true);

        drawChip(&painter);

        if (in_combination)
        {
            activate_graphics(&painter);
        }
        else
        {
            deactivate_graphics(&painter);
        }
}

Cell::~Cell()
{
    delete pointer_chip;
}

void Cell::activate()
{
    //qDebug("activate");
    if (blocked) return;
    in_combination = true;
    update();
}

void Cell::deactivate()
{
    //qDebug("deactivate");
    if (blocked) return;
    in_combination = false;
    update();
}

void Cell::activate_graphics(QPainter *painter)
{
    //qDebug("activate_graphics");
    if (blocked) return;
    painter->setPen(QPen(Qt::white, 3, Qt::SolidLine, Qt::FlatCap));
    drawChip(painter);
}

void Cell::deactivate_graphics(QPainter *painter)
{
    if (blocked) return;
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    drawChip(painter);
}

void Cell::update_chip_model(QPainter *painter)
{
    //qDebug("update_chip_model");
    if (blocked) return;
    switch(pointer_chip->color)
    {
    case 0:
        //dark blue
        painter->setBrush(QBrush(QColor(QRgb(Chip_colors::Dark_blue)), Qt::SolidPattern));
        break;
    case 1:
        //yellow
        painter->setBrush(QBrush(QColor(QRgb(Chip_colors::Yellow)), Qt::SolidPattern));
        break;
    case 2:
        //purple
        painter->setBrush(QBrush(QColor(QRgb(Chip_colors::Purple)), Qt::SolidPattern));
        break;
    case 3:
        //red
        painter->setBrush(QBrush(QColor(QRgb(Chip_colors::Red)), Qt::SolidPattern));
        break;
    case 4:
        //green
        painter->setBrush(QBrush(QColor(QRgb(Chip_colors::Green)), Qt::SolidPattern));
        break;
    default:
        painter->setBrush(QBrush(QColor(QRgb(0x000000)), Qt::SolidPattern));
        QMessageBox::critical(this, "", "ошибка в выбооре цвета");
    }
}

void Cell::drawTriangle(QPainter *painter)
{
        QVector<QPoint> points;
        points.append(QPoint(1 + CHIP_RADIUS/2, 1 + 2 + (CHIP_RADIUS - current_radius)/2));
        points.append(QPoint(1 + (CHIP_RADIUS - current_radius)/2, CHIP_RADIUS - (CHIP_RADIUS - current_radius)/2));
        points.append(QPoint(CHIP_RADIUS - (CHIP_RADIUS - current_radius)/2, CHIP_RADIUS - (CHIP_RADIUS - current_radius)/2));
        QPolygon triangle(points);
        painter->drawPolygon(points);
        points.clear();
}

void Cell::drawChip(QPainter *painter)
{
    if (blocked) return;
    switch (pointer_chip->shape)
    {
    case 0:
        painter->drawEllipse(1 + (CHIP_RADIUS - current_radius)/2, 1 + (CHIP_RADIUS - current_radius)/2, current_radius,current_radius);
        break;
    case 1:
        drawTriangle(painter);
        break;
    default:
        qDebug() << "shape error";
        break;
    }
}

void Cell::update_by_radius(int radius)
{
    if (blocked) return;
    current_radius = radius;
    repaint();
}

void Cell::smaller()
{
    if (blocked) return;
    --current_radius;
    repaint();
}

void Cell::bigger()
{
    if (blocked) return;
    ++current_radius;
    repaint();
}

void Cell::set_small()
{
    if (blocked) return;
    current_radius = 5;
    repaint();
}

void Cell::set_big()
{
    if (blocked) return;
    current_radius = CHIP_RADIUS;
    repaint();
}

void Cell::get_small()
{
    if (blocked) return;
    QTimer *tmr = new QTimer();
    connect(tmr, &QTimer::timeout, this, &Cell::smaller);
    tmr->setInterval(interval);
    tmr->start();
    QTimer::singleShot(interval*14, tmr, &QTimer::stop);
    QTimer::singleShot(interval*14, this, &Cell::set_small);

}

void Cell::get_big()
{
    if (blocked) return;
    QTimer *tmr = new QTimer();
    connect(tmr, &QTimer::timeout, this, &Cell::bigger);
    tmr->setInterval(interval);
    tmr->start();
    QTimer::singleShot(interval*14, tmr, &QTimer::stop);
    QTimer::singleShot(interval*14, this, &Cell::set_big);
}

void Cell::random_chip()
{
    if (blocked) return;
    int old_color = pointer_chip->color;
    do
    {
        delete pointer_chip;
        pointer_chip  = new Chip();
    } while (pointer_chip->color == old_color);
}

void Cell::block(bool blocked)
{
    this->blocked = blocked;
    this->repaint();
}

void Cell::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //qDebug("mousePressEvent");
    emit pressSignal();
    QWidget::mousePressEvent(event);
    //update();

}

void Cell::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //qDebug("mouseReleaseEvent");
    emit releaseSignal();
}

void Cell::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    //qDebug("enterEvent");
    emit enterSignal();
}


void Cell::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    //qDebug("leaveEvent");
    emit leaveSignal();
}

void Cell::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug("moveEvent");
    QPoint temp = mapToGlobal(event->pos());
    if (blocked)
    {
        temp = QPoint(100000, 100000);
    }
    emit moveSignal(temp);
}


