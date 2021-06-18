#ifndef EMAILMODEL_H
#define EMAILMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQuickItem>
/// stl модули
#include <list>
#include <algorithm>
#include <set>
/// написанные модули
#include <settingscontroller.h>
#include <loger.h>
#include <databaseaccessor.h>
#include <databasequery.h>
/// модули SQL
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/*!
 * \brief The EmailModel class
 * \details Класс определяет почтовый виджет для работы с формированием и отправкой/принятем контейнеров
 * в/из черного ящика.
 *
 * \author Zelenskiy V.P.
 * \version 1.0
 * \date 10.06.2021
 * \warning
 */
class EmailModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit EmailModel(std::vector<Abonent>& abonents, QObject* parent = nullptr);
    virtual ~EmailModel();
    virtual void    initModelData();
    virtual void    initAddressBook();
    virtual void    setStatus(const int status);
    int     status() const;

public slots:
    void    setOutputLetters();
    void    setInputLetters();

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int         rowCount(const QModelIndex &parent) const override;
    virtual int         columnCount(const QModelIndex &parent) const override;
    virtual QVariant    data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    enum Status{
        EMPTY = 0,
        IS_OK = 10,
        INIT_AB_ERROR,
        INIT_LET_ERROR,
        DB_ACCEPT_FAILED
    };

    enum LetterRoles{
        TITLE = Qt::UserRole+1,
        STATUS,
        DATE,
        ATACH_COUNT,
        FROM,
        TO
    };

private:
    void setAbonentsFromRESULT(db::RESULT& result, std::set<Abonent> &ab_arr);
    void setLettersFromRESULT(db::RESULT& result, std::vector<Letter>& let_arr);

protected:
    std::vector<Abonent>&           m_ref_abonents;
    std::vector<Letter>             m_letters;
    int                             m_status;
};
#endif // EMAILMODEL_H
