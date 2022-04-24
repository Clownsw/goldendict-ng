#ifndef GLOBAL_GLOBALBROADCASTER_H
#define GLOBAL_GLOBALBROADCASTER_H

#include <QObject>
#include "config.hh"

struct ActiveDictIds
{
  QString word;
  QStringList dictIds;
};

class GlobalBroadcaster : public QObject
{
  Q_OBJECT
private:
  Config::Preferences * preference;

public:
  void setPreference( Config::Preferences * _pre );
  Config::Preferences * getPreference();
  GlobalBroadcaster( QObject * parent = nullptr );
  static GlobalBroadcaster * instance();
signals:
  void emitDictIds( ActiveDictIds ad );
};

#endif // GLOBAL_GLOBALBROADCASTER_H
