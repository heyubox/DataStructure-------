#ifndef OSCGRAPH_H
#define OSCGRAPH_H

#include <QWidget>
#include <QMouseEvent>

class QStringList;
class QPen;


#define _MARGIN_ 1
#define _XPADDING_ 40   //distance from Button edge to X axis
#define _YPADDING_ 80   //distance from left edge to Y axis
#define _RIGHTEDGE_ (width()-_MARGIN_*2-_YPADDING_)      //the rightedge pos of color area in logic coordinate system
#define _TOPEDGE_ (height()-_MARGIN_*2-_XPADDING_)     //the topedge  pos of color area in logic coordinate system
#define _YLEN_ (_TOPEDGE_- 40)                    // y axis length in logic coordinate sys
#define _XLEN_ (_RIGHTEDGE_- 20)            // x axis length in logic coordinate sys
#define _X_SPAN_ ((_XLEN_-10) / 24)
#define _Y_SPAN_ ((_YLEN_-10) / 20)
#define _YOFS_ (height() - _XPADDING_ - _MARGIN_)
#define _XOFS_ (_YPADDING_ + _MARGIN_)

#define PI 3.14159

/*
 *
 *
 *         ypadding
 *           | |
 *         ______________________________
 * topedge_| _________________________  | }margin
 *         | | ^ylen                  | |
 *         | | |                      | |
 *         | | |______________>xlen   | |
 *         | |________________________| |} xpadding
 *         |____________________________|
 *         | |                        |
 *        margin                    rightedge
 *
 *
 *
 */




class OSCgraph : public QWidget
{
    Q_OBJECT

public:
    OSCgraph(volatile bool *dat, int ** ptr, QWidget *parent = nullptr);
    ~OSCgraph();
    void setTotal(int num);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


public slots:
    void setCycle(int ind);
    void setCycLen(int c);



public:
    int **outer;
    volatile bool *data;

private:

    int T, hT, aind, tind, ymaxs, cycle;
    int y_span, x_span;
    int y_top, x_top;
    int y_grid, x_grid;
    int y_hgrid, x_hgrid;
    int xofs, yofs;
    int iten_len;
    int totalNum;

    int *dptr;
    QStringList timeList, timeSpan, daySpan, dayList;

    int iten[7];
    int cycleList[6];

    QPen *pen1, *pen2, *pen3;
    QBrush *brush;
    QFont *font;

};
#endif // OSCGRAPH_H
