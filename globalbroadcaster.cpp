#include "globalbroadcaster.h"
#include <QGlobalStatic>
#include "utils.hh"

Q_GLOBAL_STATIC( GlobalBroadcaster, bdcaster )
GlobalBroadcaster::GlobalBroadcaster( QObject * parent ) : QObject( parent )
{
}

GlobalBroadcaster * GlobalBroadcaster::instance()
{
  return bdcaster;
}
void GlobalBroadcaster::setPreference( Config::Preferences * p )
{
  preference = p;
}
Config::Preferences * GlobalBroadcaster::getPreference()
{
  return preference;
}

void GlobalBroadcaster::addWhitelist(QString url){
  whitelist.push_back(url);
  auto baseUrl=::getHostBase(url);
  whitelist.push_back(baseUrl);
}

bool GlobalBroadcaster::existedInWhitelist(QString url){
  return std::find(whitelist.begin(), whitelist.end(), url) != whitelist.end();
}

void GlobalBroadcaster::setGroupId(unsigned groupId){
  this->groupId = groupId;
}

unsigned GlobalBroadcaster::getGroupId(){
  return groupId;
}
// namespace global
