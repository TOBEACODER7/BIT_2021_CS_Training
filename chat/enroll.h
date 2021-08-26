#ifndef ENROLL_H
#define ENROLL_H

#include <QWidget>

namespace Ui {
class Enroll;
}

class Enroll : public QWidget
{
    Q_OBJECT

public:
    explicit Enroll(QWidget *parent = nullptr);
    ~Enroll();

private slots:

    void on_putin_clicked();

private:
    Ui::Enroll *ui;
};

#endif // ENROLL_H
