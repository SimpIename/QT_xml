#include "parser.h"

parser::parser(QObject* parent):QAbstractItemModel(parent){
    PointerToTree=new QObject(this);
}

parser::~parser(){
    if(windowForTextErrors!=Q_NULLPTR) delete windowForTextErrors;
    delete PointerToTree;
}

QObject* parser::isPointerNode() const{
    return PointerToTree;
}

void parser::setHelpingIndexObj(QModelIndex index){
    HelpingIndexObj=index;
}

QModelIndex parser::isHelpingIndexObj() const{
    return HelpingIndexObj;
}

void parser::getWindowForTextErrors(const QString& textError){
    windowForTextErrors=new QTextEdit();
    windowForTextErrors->setWindowTitle(tr("Окно ошибок парсинга"));
    windowForTextErrors->setReadOnly(true);
    windowForTextErrors->setText(textError);
    windowForTextErrors->show();
}

void addElementInTree(const QDomNode& node,QObject*parent){
    if (!(node.isNull())) {
        if((!(node.isText()))&&(node.isElement())){
            QObject* newNode=new QObject(parent);
            newNode->setProperty("nameObj",node.toElement().tagName());
            if(node.hasChildNodes()){
                QDomNodeList ListNodes=node.childNodes();
                for(int i=0;i<ListNodes.count();++i){
                    addElementInTree(ListNodes.at(i),newNode);
                }
            }
        }
        if(node.isText()){
            QObject* newNode=new QObject(parent);
            newNode->setProperty("nameObj",node.toText().data());
        }
    }
}

void parser::addFiles(const QStringList* nameFiles){
    QString TextError=Q_NULLPTR;
    int NumberError=0;
    for(const auto nameAndPathFile:*nameFiles){
        QDomDocument document;
        QFile file(nameAndPathFile);
        if (file.open(QIODevice::ReadOnly)){
            if (document.setContent(&file)) {
                QFileInfo fileInfo(nameAndPathFile);
                QString nameFile=fileInfo.fileName();
                QString absoluteFilePath=fileInfo.absoluteFilePath();
                bool NoReplay=true;
                for(const auto obj:PointerToTree->children()){
                    if(nameFile==obj->property("nameObj").toString()) NoReplay=false;
                }
                if(NoReplay){
                    QObject* FirstElem=new QObject(PointerToTree);
                    FirstElem->setProperty("nameObj",nameFile);
                    FirstElem->setProperty("absoluteFilePath",absoluteFilePath);
                    QDomElement element = document.documentElement();
                    addElementInTree(element,FirstElem);
                }
                else {
                    ++NumberError;
                    TextError=TextError+QString::number(NumberError)+tr(". Файл ")+nameFile+tr(" уже открыт")+"\n";
                }
            }
            else {
                ++NumberError;
                TextError=TextError+QString::number(NumberError)+tr(". Не загружаются данные из ")+nameAndPathFile+"\n";
            }
        }
        else {
            ++NumberError;
            TextError=TextError+QString::number(NumberError)+tr(". Невозможно открыть ")+nameAndPathFile+"\n";
        }
        file.close();
    }
    if(ActiveObj==Q_NULLPTR){
        objByIndex(index(0,0))->setProperty("font",true);
        ActiveObj=objByIndex(index(0,0));
    }
    if(TextError!=Q_NULLPTR) getWindowForTextErrors(TextError);
}

QObject* parser::objByIndex(const QModelIndex &index) const{
    if(!index.isValid())
        return PointerToTree;
    return static_cast<QObject*>(index.internalPointer());
}

QModelIndex parser::index(int row, int column, const QModelIndex &parent) const{
    if(!(hasIndex(row,column,parent))){
        return QModelIndex();
    }
    QObject* parentObj=objByIndex(parent);
    return createIndex(row,column,parentObj->children().at(row));

}

QModelIndex parser::parent(const QModelIndex &index) const{
    QObject* childObj=objByIndex(index);
    QObject* parentObj=childObj->parent();
    if(parentObj==PointerToTree){
        return QModelIndex();
    }
    QObject* grandParentObj=parentObj->parent();
    int row=grandParentObj->children().indexOf(parentObj);
    return createIndex(row,0, parentObj);
}

int parser::rowCount(const QModelIndex &parent) const{
    return objByIndex(parent)->children().count();
}

int parser::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return 1;
}

QVariant parser::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if(role==Qt::DisplayRole){
        return objByIndex(index)->property("nameObj");
    }
    if(role==Qt::FontRole){
        if(objByIndex(index)->property("font")==true){
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    return QVariant();
}

void parser::slotMakeActive(){
    QObject* obj=objByIndex(HelpingIndexObj);
    ActiveObj->setProperty("font",false);
    obj->setProperty("font",true);
    ActiveObj=obj;
}

