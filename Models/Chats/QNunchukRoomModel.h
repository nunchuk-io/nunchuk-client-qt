#ifndef QNUNCHUKROOM_H
#define QNUNCHUKROOM_H

#include <QObject>
#include <QRegularExpression>
#include <QtConcurrent>
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <events/encryptionevent.h>
#include <events/redactionevent.h>
#include <events/roomavatarevent.h>
#include <events/roomcreateevent.h>
#include <events/roommemberevent.h>
#include <events/roommessageevent.h>
#include <events/roomcanonicalaliasevent.h>
#include <events/simplestateevents.h>
#include "QConversationModel.h"
#include "QNunchukRoomUsers.h"
#include "QRoomCustomEvent.h"
#include "WalletModel.h"
#include "Chats/QRoomWallet.h"
#include "bridgeifaces.h"
#include "DracoDefines.h"
#include "QRoomTransaction.h"

#define PAGINATION_NUMBER 100

using namespace Quotient;
typedef QSharedPointer<Quotient::Room> QuotientRoomPtr;
typedef QSharedPointer<Quotient::Connection> QuotientConnectionPtr;

namespace utils {
static const QRegularExpression removeReplyRegex{
    "> <.*?>.*?\\n\\n", QRegularExpression::DotMatchesEverythingOption};
static const QRegularExpression removeRichReplyRegex{
    "<mx-reply>.*?</mx-reply>", QRegularExpression::DotMatchesEverythingOption};
static const QRegularExpression codePillRegExp{
    "<pre><code[^>]*>(.*?)</code></pre>", QRegularExpression::DotMatchesEverythingOption};
static const QRegularExpression userPillRegExp{
    "<a href=\"https://matrix.to/#/@.*?:.*?\">(.*?)</a>",
    QRegularExpression::DotMatchesEverythingOption};
static const QRegularExpression strikethroughRegExp{
    "<del>(.*?)</del>", QRegularExpression::DotMatchesEverythingOption};
}  // namespace utils

class QNunchukRoomEnum: public QObject
{
    Q_OBJECT
    Q_ENUMS(JoinState)
public:
    enum JoinState {
        Joined = 0,
        Invited,
        Leaved
    };
};

class QNunchukRoom: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString              roomid         READ id              CONSTANT)
    Q_PROPERTY(int                  userCount      READ userCount       NOTIFY userCountChanged)
    Q_PROPERTY(QStringList          userNames      READ userNames       NOTIFY userNamesChanged)
    Q_PROPERTY(QString              roomName       READ roomName        WRITE  setRoomName   NOTIFY roomNameChanged)
    Q_PROPERTY(QConversationModel*  conversation   READ conversation    NOTIFY conversationChanged)
    Q_PROPERTY(QString              lastTimestamp  READ lasttimestamp   NOTIFY lasttimestampChanged)
    Q_PROPERTY(QString              lastMessage    READ lastMessage     NOTIFY lastMessageChanged)
    Q_PROPERTY(QNunchukRoomUsers*   users          READ users           NOTIFY usersChanged)
    Q_PROPERTY(QString              typingNames    READ typingNames     NOTIFY typingNamesChanged)
    Q_PROPERTY(bool                 allHisLoaded   READ allHisLoaded    NOTIFY allHisLoadedChanged)
    Q_PROPERTY(QRoomWallet*         roomWallet     READ roomWallet      NOTIFY roomWalletChanged)
    Q_PROPERTY(QRoomTransactionModel* pendingTxs   READ pendingTxs      NOTIFY pendingTxsChanged)
    Q_PROPERTY(QRoomTransactionModel* txs          READ txs             NOTIFY txsChanged)
    Q_PROPERTY(int                  unreadCount    READ unreadCount     NOTIFY unreadCountChanged)
    Q_PROPERTY(QRoomTransaction* pinTransaction    READ pinTransaction  NOTIFY pinTransactionChanged)
    Q_PROPERTY(QString roomAvatar                  READ roomAvatar      NOTIFY usersChanged)
public:
    QNunchukRoom(Room* r);
    ~QNunchukRoom();
    QMap<QString, nunchuk::AddressType> addr_map
    {
        {"ANY"          , nunchuk::AddressType::ANY},
        {"LEGACY"       , nunchuk::AddressType::LEGACY},
        {"NESTED_SEGWIT", nunchuk::AddressType::NESTED_SEGWIT},
        {"NATIVE_SEGWIT", nunchuk::AddressType::NATIVE_SEGWIT},
        {"TAPROOT"      , nunchuk::AddressType::TAPROOT}
    };
    bool isServerNoticeRoom() const;
    bool isNunchukSyncRoom() const;
    QString localUserName() const;
    QString id() const;
    QStringList aliases() const;
    QString status() const;
    int userCount() const;
    QStringList userNames();
    QString roomAvatar();
    QString roomName();
    void setRoomName(const QString& name);
    Quotient::Room *room() const;
    JoinState roomJoinState();
    int unreadCount() const;
    QString lasttimestamp() const;
    time_t lasttimestamp_timet() const;
    void setLasttimestamp(const Conversation &cons);
    QNunchukRoomUsers *users() const;
    QNunchukRoomUsersPtr usersPtr() const;
    bool HasContact(const QString &id);
    void setUsers(const QNunchukRoomUsersPtr &data);
    QConversationModel *conversation() const;
    QConversationModelPtr conversationPtr() const;
    void setConversation(const QConversationModelPtr &data);
    QString typingNames();
    void setTypingNames(const QString names);
    QRoomWallet* roomWallet() const;
    QRoomWalletPtr roomWalletPtr() const;
    void setRoomWallet(const QRoomWalletPtr &roomWallet);
    QString lastMessage() const;
    void setLastMessage(const Conversation &cons);
    bool allHisLoaded() const;
    QRoomTransaction *pinTransaction();
    void setPinTransaction(QRoomTransactionPtr tran);
    nunchuk::Wallet createWalletFromJson(const QJsonObject &json);
    bool isEncrypted();

    Q_INVOKABLE void sendMessage(const QString& message);
    Q_INVOKABLE void sendReaction(const QString& react);
    Q_INVOKABLE void inviteToRoom(const QString& memberId);
    Q_INVOKABLE void kickMember(const QString &memberId);
    Q_INVOKABLE void banMember(const QString& userId);
    Q_INVOKABLE void getMoreContents(const int limit = PAGINATION_NUMBER);
    Q_INVOKABLE void markAllMessagesAsRead();
    Q_INVOKABLE void markFiveMessagesAsRead();
    Q_INVOKABLE void joinAndLeaveWallet();
    Q_INVOKABLE bool leaveWallet(const QString& xfp);
    Q_INVOKABLE bool getXpub(const QString& id);
    Q_INVOKABLE void finalizeWallet();
    Q_INVOKABLE void cancelWallet();

    void setTags(const QString& newtag);
    void setDisplayed(bool displayed);
    void downloadHistorical();
    void connectRoomSignals();
    void connectRoomServiceSignals();
    bool checkIsLocalUser(const QString userID);
    QList<DracoUser> getNunchukMembers() const;
    QString getNunchukMemberChatId(const QString &input);
    QString getNunchukMemberName(const QString &input);
    QString getNunchukMemberEmail(const QString& input) ;
    void setNunchukMembers(const QList<DracoUser> &nunchukMembers);

    QRoomTransactionModel *pendingTxs() const;
    QRoomTransactionModelPtr pendingTxsPtr() const;
    void setPendingTxs(const QRoomTransactionModelPtr &pendingTxsPtr);

    QRoomTransactionModel* txs() const;
    QRoomTransactionModelPtr txsPtr() const;
    void setTxs(const QRoomTransactionModelPtr &txs);
    void startGetPendingTxs();
    void transactionChanged(const QString &tx_id, const int status, const int height);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void downloadTransactionThread(Conversation cons, const QString &roomid);
    void updateCancelWallet(const QString &init_event_id);
    void updateCancelTransaction(const Conversation cons);
    bool isDownloaded() const;
    bool extractNunchukEvent(const QString &matrixType, const QString &init_event_id, const QJsonObject& json, Conversation &cons) ;
    nunchuk::Wallet walletImport() const;
    void setWalletImport(const nunchuk::Wallet &walletImport);

private:
    Quotient::Room          *m_room;
    QConversationModelPtr   m_conversation;
    time_t                  m_lasttimestamp;
    QNunchukRoomUsersPtr    m_users;
    QString                 m_typingNames;
    QRoomWalletPtr          m_roomWallet;
    QString                 m_lastMessage;
    QList<DracoUser>        m_nunchukMembers;
    QRoomTransactionModelPtr m_pendingTxs;
    QRoomTransactionModelPtr m_txs;
    bool                     m_downloaded;
    QRoomTransaction       *m_pinTransaction;
    nunchuk::Wallet         m_walletImport;
    bool                    m_IsEncrypted;
private:
    bool validatePendingEvent(const QString& txnId);
    bool extractNunchukEvent(const RoomEvent& evt, Conversation &cons) ;
    QString eventToString(const RoomEvent& evt, Qt::TextFormat format = Qt::RichText, bool removeReply = true) const ;
    void receiveMessage(int fromIndex, int toIndex);
    Conversation createConversation(const RoomEvent& evt) ;
    void nunchukConsumeEvent(const RoomEvent& evt) ;
    void nunchukConsumeSyncEvent(const RoomEvent& evt) ;
    bool joinWalletWithMasterSigner(const QString& id, bool &needXpub);
    bool joinWalletWithSingleSigner(const QString& xfp);
    bool joinWalletUseSignerFromWalletImport(const QString& id,const QString& xfp);

public slots:
    void highlightCountChanged();
    void notificationCountChanged();
    void pendingEventAboutToMerge(Quotient::RoomEvent* serverEvent, int pendingEventIndex);
    void pendingEventChanged(int pendingEventIndex);
    void messageSent(QString txnId, QString eventId);
    void aboutToAddNewMessages(RoomEventsRange events);
    void addedMessages(int fromIndex, int toIndex);
    void aboutToAddHistoricalMessages(RoomEventsRange events);
    void unreadMessagesChanged(Quotient::Room* room);
    void typingChanged();
    void roomWalletCreated(const QString &roomId);
    QString postEvent(const QString& eventType, const QJsonObject& content);
    QString postJson(const QString& matrixType, const QJsonObject& content);
    void slotFinishedDownloadTransaction(nunchuk::RoomTransaction room_tx, nunchuk::Transaction tx, const Conversation& cons);
    void slotFinishFinalizeWallet(QString what, int type, int code);
    void slotFinishCancelWallet(QString what, int type, int code);
    void slotFinishedGetPendingTxs(QRoomTransactionModelPtr txs);
    void slotUpdateInitEventId(const Conversation cons);
signals:
    void userCountChanged();
    void userNamesChanged();
    void roomNameChanged();
    void conversationChanged();
    void lasttimestampChanged();
    void usersChanged();
    void typingNamesChanged();
    void sharedWalletChanged();
    void allHisLoadedChanged();
    void incomingMessage(QString sender, QString message);
    void roomWalletChanged();
    void lastMessageChanged();
    void noticeService();
    void pendingTxsChanged();
    void txsChanged();
    void unreadCountChanged();
    void signalFinishedDownloadTransaction(nunchuk::RoomTransaction room_tx, nunchuk::Transaction tx, const Conversation& cons);
    void nunchukSyncMasterSignersCompleted(std::vector<nunchuk::MasterSigner> masterSigners);
    void nunchukSyncRemoteSignersCompleted(std::vector<nunchuk::SingleSigner> remoteSigners);
    void nunchukSyncWalletsCompleted(std::vector<nunchuk::Wallet> wallets);
    void signalStartFinalizeWallet();
    void signalStartCancelWallet();
    void signalFinishFinalizeWallet(QString what, int type, int code);
    void signalFinishCancelWallet(QString what, int type, int code);
    void signalFinishedGetPendingTxs(QRoomTransactionModelPtr txs);
    void pinTransactionChanged();
};
Q_DECLARE_METATYPE(Conversation)
Q_DECLARE_METATYPE(nunchuk::RoomTransaction)

typedef QSharedPointer<QNunchukRoom> QNunchukRoomPtr;

class QNunchukRoomListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int              currentIndex    READ currentIndex   WRITE setCurrentIndex   NOTIFY currentIndexChanged)
    Q_PROPERTY(QNunchukRoom*    currentRoom     READ currentRoom                            NOTIFY currentRoomChanged)
    Q_PROPERTY(int              totalUnread     READ totalUnread                            NOTIFY totalUnreadChanged)
    Q_PROPERTY(int              count           READ count                                  NOTIFY countChanged)
public:
    QNunchukRoomListModel(Connection *c);
    ~QNunchukRoomListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int count() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    QNunchukRoomPtr getRoomByIndex(const int index) const;
    QNunchukRoomPtr getRoomById(const QString& id) const;
    int getIndex(const QString& id) const;
    Quotient::Connection *connection();
    void downloadRooms();
    void checkNunchukSyncRoom();
    int currentIndex() const;
    void setCurrentIndex(int index);
    QNunchukRoom *currentRoom() const;
    QNunchukRoomPtr currentRoomPtr() const;
    void setCurrentRoom(const QNunchukRoomPtr &currentRoom);
    void sendMessage(const QString& msg);
    int totalUnread();
    void requestSort();
    QString getRoomIdByWalletId(const QString &wallet_id);
    void transactionChanged(const QString &wallet_id, const QString &tx_id, const int status, const int height);
    void updateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &memo);
    enum RoomRoles {
        room_id,
        room_name,
        room_joinstate,
        room_unreadmsg_count,
        room_last_timestamp,
        room_last_message,
        room_typing_users,
        room_wallet_ready,
        room_avatar_url,
        room_users_count,
        room_is_encrypted
    };

    void doAddRoom(QNunchukRoomPtr r);
    void removeRoomByIndex(const int index);
    void removeAll();

    // Room operation
    void forgetRoom(const int index);
    void leaveCurrentRoom();
    void leaveRoom(const int index);
    void joinRoom(QString roomAliasOrId);
    void createRoomChat(const QStringList invitees_id, const QString &topic, const QString &name, QVariant firstMessage = QVariant());
    bool allHisLoaded();
private:
    Quotient::Connection*   m_connection;
    int                     m_currentIndex;
    QNunchukRoomPtr         m_currentRoom;
    QList<QNunchukRoomPtr>  m_data;
    QList<QNunchukRoomPtr>  m_servive;
    QTimer                  m_time;
private:
    bool containsRoom(const QString& id);
    bool containsServiceRoom(const QString& id);
    bool containsSyncRoom();

signals:
    void currentIndexChanged();
    void currentRoomChanged();
    void totalUnreadChanged();
    void noticeService();
    void finishedDownloadRoom();
    void countChanged();
public slots:
    bool hasContact(const QString &id);
    void newRoom(Quotient::Room* room);
    void invitedRoom(Quotient::Room* room, Quotient::Room* prev);
    void joinedRoom(Quotient::Room* room, Quotient::Room* prev);
    void leftRoom(Quotient::Room* room, Quotient::Room* prev);
    void aboutToDeleteRoom(Quotient::Room* room);
    void loadedRoomState(Quotient::Room* room);
    // refresh room
    void refresh(QNunchukRoomPtr room, const QVector<int>& roles = {});
    void resort();
};

typedef QSharedPointer<QNunchukRoomListModel> QNunchukRoomListModelPtr;
bool sortRoomListTimeAscending(const QNunchukRoomPtr &v1, const QNunchukRoomPtr &v2);
bool sortRoomListByTimeDescending(const QNunchukRoomPtr &v1, const QNunchukRoomPtr &v2);

#endif // QNUNCHUKROOM_H