#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QtWidgets>

#include <QDomDocument>

class parser:public QAbstractItemModel{
    Q_OBJECT

private:
    QObject* PointerToTree;
    QObject* ActiveObj=Q_NULLPTR;
    QModelIndex HelpingIndexObj=QModelIndex();

    QTextEdit* windowForTextErrors=Q_NULLPTR;

public:
    parser(QObject* parent=nullptr);
    ~parser();

    QObject* isPointerNode() const;

    void setHelpingIndexObj(QModelIndex);
    QModelIndex isHelpingIndexObj() const;

    void getWindowForTextErrors(const QString&);

    void addFiles(const QStringList*);
    friend void addElementInTree(const QDomNode&,QObject*);

    QObject* objByIndex(const QModelIndex&) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:

    void slotMakeActive();
};

#endif
