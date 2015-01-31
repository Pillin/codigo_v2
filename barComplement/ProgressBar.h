#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include "QProgressBar"

class ProgressBar : public QProgressBar {
public:
    ProgressBar(QWidget *parent = 0) : QProgressBar(parent) {
        style="QProgressBar {"
                "text-align: center;"
                "}"
              "QProgressBar::chunk {"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2);"
                "text-align: center;"
                "margin: 2px 1px 1p 2px;"
              "}";
        setRange(0,100);
    }

public slots:
    void setValue(int value){
        QString c,ac;
        switch(value/20){
            case 0:
                c="#ff0000";
                break;
            case 1:
                c="#00ff00";
                break;
            case 2:
                c="#0000ff";
                break;
            case 3:
                c="#ffff00";
                break;
            case 4:
                c="#ff00ff";
                break;
            case 5:
                c="#000000";
                break;
        }
        ac=adjust(c,80);
        setStyleSheet(style.arg(ac).arg(c));
        QProgressBar::setValue(value);
    }

private:
    QString adjust(QString color, int factor){
        QColor *c=new QColor(color);
        c->setHsl(c->hslHue(),
                  c->hslSaturation(),
                  qMin(c->lightness()+factor, 255));
        return c->name();
    }

    QString style;
};




#endif // PROGRESSBAR_H
