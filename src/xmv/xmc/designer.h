#ifndef DESIGNER_H
#define DESIGNER_H

#include <QObject>
#include <QProcess>

static const QString designer_exec_name = "xmd";

class Designer : public QObject
{
    Q_OBJECT
public:
    explicit Designer(QObject *parent = 0);
    ~Designer();

    void state();

signals:

public slots:
    void handleProcessChange(QProcess::ProcessState state);

private:
    QProcess m_process;
};

#endif // DESIGNER_H
