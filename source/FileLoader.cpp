#include <QDebug>
#include <QByteArray>
#include <QNetworkReply>
#include <QDir>
#include <QFile>

#include "FileLoader.h"

dxs::FileLoader::FileLoader(QObject *parent)
    : QObject(parent)
    , _forceUseNetwork(false)
{
    connect (&_nam, SIGNAL(finished(QNetworkReply*)), SLOT(finishedRequest(QNetworkReply*)));
    connect (&_nam, SIGNAL(error(QNetworkReply*)),    SLOT(errorRequest(QNetworkReply*)));
}

//--------------------------------------------------------------------------
dxs::FileLoader::~FileLoader()
{

}

//--------------------------------------------------------------------------
void dxs::FileLoader::finishedRequest (QNetworkReply *reply)
{
    _error = "";
    QByteArray data = reply->readAll();
    _fileContent = QString::fromUtf8(data);
    _fileContent = _fileContent.simplified();

    /* Если _forceUseNetwork сохранять в файл не надо */
    if (!_fileName.isEmpty() && !_forceUseNetwork) {
        QDir dir;
        dir.mkdir("cache/");
        QFile file ("cache/" + _fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write ((char *)&_version, sizeof(_version));
            file.write (data);
        } else {
            qDebug() << "Can't create file, error:" << file.errorString();
        }
    }

    reply->deleteLater();
    fileReady();
}

//--------------------------------------------------------------------------
void dxs::FileLoader::errorRequest(QNetworkReply *replyWithError) {
    _error = replyWithError->errorString();
    replyWithError->deleteLater();
    emit errorReady();
}

//--------------------------------------------------------------------------
void dxs::FileLoader::requestFile() {
    requestFile(_fileName, _url, _version);
}
//--------------------------------------------------------------------------
void dxs::FileLoader::requestFile (const QString &fileName, const QString &url, int version)
{
    _fileName = fileName;
    _url = url;
    _version = _version;

    QFile file ("cache/" + fileName);

    if (!_forceUseNetwork
        && file.exists()
        && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "requestFile:: readFromFile";
        int fileVersion;
        int readedCount = file.read ((char *)&fileVersion, sizeof(fileVersion));

        // TODO Возможо стоит использовать знак > при сравнении.
        if (readedCount == sizeof(fileVersion) && fileVersion == version) {
            _fileContent = "";
            QString line;
            QTextStream t( &file );

            do {
                line = t.readLine();
                if (!line.isNull()) {
                    _fileContent += line + "\n";
                }
            } while (!line.isNull());

            if (_fileContent != "") {
                //qDebug() << "requestFile::fileReady";
                emit fileReady();
                file.close();
                return;
            }
        }

        //qDebug() << "requestFile:: error of readingFile";
        file.close();
        file.remove();
    }

    //qDebug() << "requestFile::get from net";
    _nam.get(url);
}

//--------------------------------------------------------------------------
void dxs::FileLoader::writeToFile(const QString &fileName, const QString &content)
{
    QFile file (fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write (content.toUtf8());
    }
}

//--------------------------------------------------------------------------
QString dxs::FileLoader::readFromFile(const QString &fileName)
{
    QFile file (fileName);

    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content;
        QString line;
        QTextStream t( &file );

        do {
            line = t.readLine();
            content += line + "\n";
        } while (!line.isNull());

        return content;
    }

    return "";
}

//--------------------------------------------------------------------------
void dxs::FileLoader::clearCache()
{
    QFile file ("cache/" + _fileName);
    file.remove();
}

//--------------------------------------------------------------------------
void dxs::FileLoader::classBegin() {

}

//--------------------------------------------------------------------------
void dxs::FileLoader::componentComplete()
{
    if (!_url.isEmpty()) {
        requestFile ();
    }
}
