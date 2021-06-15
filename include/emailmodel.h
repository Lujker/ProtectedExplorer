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
public:
    explicit EmailModel(const std::vector<Abonent>& abonents, QObject* parent = nullptr);
    void initModelData();
    void initAddressBook();
    int status() const;
    void setStatus(int status);

public slots:
    void setOutputLetters();
    void setInputLetters();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    /*!
     * \brief The Letter struct описывает структуру одного сообщения
     */
    struct Letter{
        Letter(): let_id(0), let_status(0)
        {}
        int         let_id;     ///id сообщения в БД
        int         let_status; ///id статуса сообщения
        std::string let_path;   ///путь к контейнеру письма
        std::string title;      ///название сообщения
        std::string date;       ///дата получения/отправки

        bool operator==(const Letter& let) const;
        bool operator!=(const Letter& let) const;
        bool operator<(const Letter& let) const;
    };

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
        DATE
    };


private:
    const std::vector<Abonent>& m_ref_abonents;
    std::vector<Letter>         m_letters;
    int                         m_status;
};
#endif // EMAILMODEL_H
