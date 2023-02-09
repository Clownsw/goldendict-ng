#include "iframeschemehandler.h"

#include <QTextCodec>

IframeSchemeHandler::IframeSchemeHandler(QObject * parent):QWebEngineUrlSchemeHandler(parent){

}
void IframeSchemeHandler::requestStarted(QWebEngineUrlRequestJob *requestJob)
{
  QUrl url = requestJob->requestUrl();

  // website dictionary iframe url
  url = QUrl( Utils::Url::queryItemValue( url, "url" ) );
  QNetworkRequest request;
  request.setUrl( url );
  request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy);

  QNetworkReply * reply = mgr.get( request );

  auto finishAction     = [ = ]() -> void
  {
    QByteArray contentType = "text/html";

    QBuffer * buffer = new QBuffer( requestJob );

    QByteArray replyData = reply->readAll();
    QString articleString;

    QString codecName;
    auto contentTypeV = reply->header(QNetworkRequest::ContentTypeHeader);
    if(contentTypeV.isValid())
    {
      auto _ct = contentTypeV.toString();
      auto index = _ct.indexOf("charset=");
      if(index>-1){
        codecName=_ct.mid(index+QString("charset=").size());
        qDebug()<<codecName;
      }
    }

    QTextCodec * codec = QTextCodec::codecForUtfText( replyData, QTextCodec::codecForName( codecName.toUtf8() ) );
    if(codec)
      articleString      = codec->toUnicode( replyData );
    else
      articleString = QString::fromUtf8(replyData);
    // Handle reply data
    // 404 response may have response body.
    if( reply->error() != QNetworkReply::NoError && articleString.isEmpty())
    {
      if(reply->error()==QNetworkReply::ContentNotFoundError){
        buffer->deleteLater();
        //work around to fix QTBUG-106573
        requestJob->redirect(url);
        return;
      }
      QString emptyHtml = QString( "<html><body>%1</body></html>" ).arg( reply->errorString() );
      buffer->setData( emptyHtml.toUtf8() );
      requestJob->reply( contentType, buffer );
      return;
    }

    // Change links from relative to absolute

    QString root = reply->url().scheme() + "://" + reply->url().host();
    QString base = root + reply->url().path();

    QRegularExpression baseTag( "<base\\s+.*?>",
                             QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption );
    
    QString baseTagHtml = "<base href=\"" + base + "\">";

    QString depressionFocus ="<script type=\"application/javascript\"> HTMLElement.prototype.focus=function(){console.log(\"focus() has been disabled.\");}</script>"
                      "<script type=\"text/javascript\" src=\"qrc:///scripts/iframeResizer.contentWindow.min.js\"></script>"
                      "<script type=\"text/javascript\" src=\"qrc:///scripts/iframe-defer.js\"></script>";
    
    // remove existed base tag
    articleString.remove( baseTag ) ;

    QRegularExpression headTag( "<head\\b.*?>",
                                QRegularExpression::CaseInsensitiveOption
                                  | QRegularExpression::DotMatchesEverythingOption );
    auto match = headTag.match( articleString, 0 );
    if( match.hasMatch() )
    {
      articleString.insert( match.capturedEnd(), baseTagHtml );
      articleString.insert( match.capturedEnd(), depressionFocus );
    }
    else
    {
      // the html contain no head element
      // just insert at the beginning of the html ,and leave it at the mercy of browser(chrome webengine)
      articleString.insert( 0, baseTagHtml );
      articleString.insert( 0, depressionFocus );
    }

    if(codec)
      buffer->setData(codec->fromUnicode(articleString));
    else
      buffer->setData(articleString.toUtf8());

  #ifdef Q_OS_WIN32
    requestJob->reply(contentTypeV.toByteArray() , buffer );
  #else
    //use "text/html;charset=utf-8" will make the response html text rendered on the webpage?
    requestJob->reply(contentType , buffer );
  #endif
  };
  connect( reply, &QNetworkReply::finished, requestJob, finishAction );

  connect( requestJob, &QObject::destroyed, reply, &QObject::deleteLater );
}
