/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#include "QRest.h"
#include "AppModel.h"
#include "QOutlog.h"

QString    QRest::m_dracoToken      = "";
QByteArray QRest::m_machineUniqueId = QSysInfo::machineUniqueId();

QRest::QRest() : m_networkManager(new QNetworkAccessManager())
{
    m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
}

QRest::~QRest()
{

}

QString QRest::dracoToken()
{
    return m_dracoToken;
}

void QRest::setDracoToken(const QString token)
{
    m_dracoToken = token;
}

QByteArray QRest::machineUniqueId()
{
    return m_machineUniqueId;
}

QJsonObject QRest::postSync(const QString &cmd, QJsonObject data, int& reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->post(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::postSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("POST %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value = paramsQuery.value(key);
            params.addQueryItem(key, value);
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->post(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::getSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("GET %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!data.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, data.keys()) {
            QJsonValue value = data.value(key);
            params.addQueryItem(key, value.toString());
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::getSync(const QString &cmd, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("GET %1").arg(command));
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!data.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, data.keys()) {
            QJsonValue value = data.value(key);
            params.addQueryItem(key, value.toString());
        }
        url.setQuery(params);
    }
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->get(requester_));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::putSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("PUT %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->put(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::putSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QJsonObject ret;
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value = paramsQuery.value(key);
            params.addQueryItem(key, value);
        }
        url.setQuery(params);
    }
    QFunctionTime f(QString("PUT %1").arg(url.toString()));
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->put(requester_, QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::deleteSync(const QString &cmd, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QFunctionTime f(QString("DELETE %1").arg(command));
    QJsonObject ret;
    QNetworkRequest requester_(QUrl::fromUserInput(command));
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}

QJsonObject QRest::deleteSync(const QString &cmd, QMap<QString, QString> paramsQuery, QMap<QString, QString> paramsHeader, QJsonObject data, int &reply_code, QString &reply_msg)
{
    QString command = commandByNetwork(cmd);
    QUrl url = QUrl::fromUserInput(command);
    if(!paramsQuery.isEmpty()){
        QUrlQuery params;
        foreach(const QString& key, paramsQuery.keys()) {
            QString value = paramsQuery.value(key);
            params.addQueryItem(key, value);
        }
        url.setQuery(params);
    }
    QFunctionTime f(QString("DELETE %1").arg(url.toString()));
    QJsonObject ret;
    QNetworkRequest requester_(url);
    QString headerData = QString("Bearer %1").arg(dracoToken());
    requester_.setRawHeader("Authorization", headerData.toLocal8Bit());
    requester_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requester_.setRawHeader("Connection", "keep-alive");
    requester_.setRawHeader("x-nc-device-id", machineUniqueId());
    requester_.setRawHeader("x-nc-app-version", qApp->applicationVersion().toUtf8());
    requester_.setRawHeader("x-nc-device-class", "Desktop");
    requester_.setRawHeader("x-nc-os-name", QSysInfo::productType().toUtf8());
    qint64 maximumBufferSize = 1024 * 1024;
    requester_.setAttribute(QNetworkRequest::MaximumDownloadBufferSizeAttribute, maximumBufferSize);
    // Add addional params
    for(QString param : paramsHeader.keys()) {
        requester_.setRawHeader(QByteArray::fromStdString(param.toStdString()), QByteArray::fromStdString(paramsHeader.value(param).toStdString()));
    }
    std::unique_ptr<QNetworkReply, std::default_delete<QNetworkReply>> reply(m_networkManager->sendCustomRequest(requester_, "DELETE", QJsonDocument(data).toJson()));
    QEventLoop eventLoop;
    QObject::connect(reply.get(),   &QNetworkReply::finished,   &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        reply_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        reply_msg  = reply->errorString();
        QByteArray response_data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response_data);
        ret = json.object();
    }
    else{
        reply_code = reply->error();
        reply_msg  = reply->errorString();
        if(reply_code >= QNetworkReply::ConnectionRefusedError && reply_code <= QNetworkReply::UnknownNetworkError){
            reply_msg = STR_CPP_111;
        }
        AppModel::instance()->showToast(reply_code, reply_msg, EWARNING::WarningType::EXCEPTION_MSG);
    }
    reply.release()->deleteLater();
    return ret;
}
