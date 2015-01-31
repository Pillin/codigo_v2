#ifndef BARRA_H
#define BARRA_H
#include "ProgressBar.h"
#include "QVBoxLayout"
class Barra : public QWidget {
public:
    Barra(QWidget *parent = 0) : QWidget(parent), value(0) {
        QVBoxLayout *l=new QVBoxLayout(this);
        progress=new ProgressBar(this);
        progress->setValue(value);
        l->addWidget(progress);
        startTimer(100);
    }

protected:
    void timerEvent(QTimerEvent *){
        value=value<100 ? value+1 : 0;
        progress->setValue(value);
    }

private:
    ProgressBar *progress;
    int value;
};
#endif // BARRA_H
