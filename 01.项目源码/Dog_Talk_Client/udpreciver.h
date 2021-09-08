#include <QUdpSocket>
#include <QThread>
#include <QFile>
class UdpReciver : public QObject
{
    Q_OBJECT
public:
    UdpReciver(QObject *parent = NULL);
    ~UdpReciver();
    void init_port(QString tmphost, int tmport);
signals:
    void deliverInfo(QString);
public slots:
    void readDatagrams(); //listen UDP data
private:
    QUdpSocket *m_udpSocket;
    QString m_localhost;
    int m_port;
    QByteArray m_data;
    QThread *m_thread;
};
