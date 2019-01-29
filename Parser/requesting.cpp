#include "requesting.h"

Requesting::Requesting(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
}

Requesting::~Requesting()
{
    delete manager;
}

QByteArray Requesting::pageText(QUrl pageUrl)
{
    QNetworkRequest request(pageUrl);
    QNetworkReply *reply = manager->get(request);
    connect(reply,SIGNAL(finished()), &eventLoop, SLOT(quit()));     //I think this way of blocking is dangerous. 
    eventLoop.exec();                                                //Why not to use reply->waitForReadyRead(ms) and reply->isFinished() instead?
    reply->deleteLater();
    return reply->readAll();
}

QByteArray Requesting::olxPhoneText(QUrl referer, QString id, QString token)
{
    if(id == "" || token == "") return "";
    QUrl url("https://olx.uz/ajax/misc/contact/phone/"+id+"/?pt="+token);
    QNetworkRequest request(url);
    request.setRawHeader(QString("Referer").toUtf8(), referer.toEncoded());
    QNetworkReply *reply = manager->get(request);
    connect(reply,SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    reply->deleteLater();
    return reply->readAll();
}

QByteArray Requesting::uyBorAdList(int page)
{
    QUrl post("https://uybor.uz/api/listing/search");
    QUrlQuery postData;
    postData.addQueryItem("page", QString(page));
    postData.addQueryItem("operation_type_code","sale");
    postData.addQueryItem("category_id", "23");
    postData.addQueryItem("region_id", "13");
    QNetworkRequest request(post);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    reply->deleteLater();
    return reply->readAll();
}

QByteArray Requesting::uyBorPhoneText(QString id, QString token, QUrl referer)
{
    QUrl post("https://uybor.uz/listing/search/get-phones");
    QUrlQuery postData;
    postData.addQueryItem("id", id);
    QNetworkRequest request(post);
    request.setRawHeader(QString("Referer").toUtf8(), referer.toEncoded());
    request.setRawHeader(QString("X-CSRF-TOKEN").toUtf8(), token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    reply->deleteLater();
    return reply->readAll();
}
