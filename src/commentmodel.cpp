#include "commentmodel.h"

#include <QtCore/QDateTime>

#include "utils.h"

CommentModel::CommentModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[FullnameRole] = "fullname";
    roles[AuthorRole] = "author";
    roles[BodyRole] = "body";
    roles[ScoreRole] = "score";
    roles[LikesRole] = "likes";
    roles[CreatedRole] = "created";
    roles[DepthRole] = "depth";
    roles[IsScoreHiddenRole] = "isScoreHidden";
    setRoleNames(roles);
}

int CommentModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_commentList.count();
}

QVariant CommentModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT_X(index.row() < m_commentList.count(), Q_FUNC_INFO, "index out of range");

    const CommentObject comment = m_commentList.at(index.row());

    switch (role) {
    case FullnameRole: return comment.fullname();
    case AuthorRole: {
        QString author = comment.author();
        if (comment.isSubmitter())
            author += " [S]";
        switch (comment.distinguished()) {
        case CommentObject::DistinguishedByModerator: author += " [M]"; break;
        case CommentObject::DistinguishedByAdmin: author += " [A]"; break;
        case CommentObject::DistinguishedBySpecial: author += " [?]"; break;
        default: break;
        }
        return author;
    }
    case BodyRole: return comment.body();
    case ScoreRole: return comment.score();
    case LikesRole: return comment.likes();
    case CreatedRole: {
        QString createdTimeDiff = Utils::getTimeDiff(comment.created());
        // TODO: show the edited time diff in UI
        if (comment.edited().isValid())
            createdTimeDiff.append("*");
        return createdTimeDiff;
    }
    case DepthRole: return comment.depth();
    case IsScoreHiddenRole: return comment.isScoreHidden();
    default:
        qCritical("CommentModel::data(): Invalid role");
        return QVariant();
    }
}

int CommentModel::count() const
{
    return m_commentList.count();
}

QString CommentModel::lastFullName() const
{
    Q_ASSERT(!m_commentList.isEmpty());
    return m_commentList.last().fullname();
}

void CommentModel::append(const QList<CommentObject> &commentList)
{
    if (commentList.isEmpty())
        return;

    beginInsertRows(QModelIndex(), m_commentList.count(), m_commentList.count() + commentList.count() - 1);
    m_commentList.append(commentList);
    endInsertRows();
}

void CommentModel::clear()
{
    if (m_commentList.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, m_commentList.count() - 1);
    m_commentList.clear();
    endRemoveRows();
}

void CommentModel::changeVote(const QString &fullname, VoteManager::VoteType voteType)
{
    for (int i = 0; i < m_commentList.count(); ++i) {
        CommentObject comment = m_commentList.at(i);

        if (comment.fullname() == fullname) {
            int oldLikes = comment.likes();
            switch (voteType) {
            case VoteManager::Upvote:
                comment.setLikes(1); break;
            case VoteManager::Downvote:
                comment.setLikes(-1); break;
            case VoteManager::Unvote:
                comment.setLikes(0); break;
            }
            comment.setScore(comment.score() + (comment.likes() - oldLikes));
            emit dataChanged(index(i), index(i));
            break;
        }
    }
}

int CommentModel::getParentIndex(int index) const
{
    int parentDepth = m_commentList.at(index).depth() - 1;
    for (int i = index; i >= 0; --i) {
        if (m_commentList.at(i).depth() == parentDepth)
            return i;
    }

    qWarning("CommentModel::getParentIndex(): Cannot find parent index");
    return index;
}
