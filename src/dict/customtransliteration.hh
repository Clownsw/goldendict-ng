#pragma once


#include <vector>
#include "transliteration.hh"

// Support for Belarusian transliteration
namespace CustomTranslit {

class CustomTransTable: public Transliteration::Table
{
public:
  CustomTransTable() = default;

  explicit CustomTransTable( const QString & content );

private:
  void parse( const QString & content );
};


std::vector< sptr< Dictionary::Class > > makeDictionaries( Config::CustomTrans const & );

} // namespace CustomTranslit
