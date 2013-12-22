#ifndef FCOMMANDER_H
#define FCOMMANDER_H

#include <QObject>

class Fcommander : public QObject
{
    Q_OBJECT
public:
    explicit Fcommander(QObject *parent = 0);

signals:

public slots:

private:
    void sync();
    void login();
};

#endif // FCOMMANDER_H
