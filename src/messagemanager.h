#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "abstractmanager.h"

class MessageManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit MessageManager(QObject *parent = 0);

    Q_INVOKABLE void reply(const QString &fullname, const QString &rawText);
    Q_INVOKABLE void markRead(const QString &fullname);
    Q_INVOKABLE void markUnread(const QString &fullname);

signals:
    void replySuccess();
    void markReadStatusSuccess(const QString &fullname, bool isUnread);
    void error(const QString &errorString);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QString m_fullname;
    enum Action { Reply, MarkRead, MarkUnread };
    Action m_action;

    APIRequest *m_request;
    void abortActiveReply();
};

#endif // MESSAGEMANAGER_H
