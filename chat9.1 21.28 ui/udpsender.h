# include <QUdpSocket>
class UdpSender : public QObject
{
    Q_OBJECT
public:
    UdpSender();
    ~UdpSender();
    void initSender(QString desHost, int port);
    void send_file(QByteArray msg);
public slots:
    void recMsg(QByteArray msgInfo);
private:
    QUdpSocket *m_Socket = nullptr;
    QString m_address;
    int m_port;
};
