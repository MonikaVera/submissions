// Az array_accumulater osztály template megkapja template paraméterként, hogy
// azok a tömbök, amelyekkel dolgozni fog, milyen típusú elemeket tárolnak.
// Konstruktor paraméterként megkap egy megfelelő típusú tömböt, aminek hatására
// a tömbben az elemek értéke az adott indexig tartó eredeti tömbelemek
// részletösszege lesz, tehát a tömb 0 indexű eleme megmarad, a tömb 1 indexű
// eleme a a tömb 0 és 1 indexű elemének az összege lesz, és így tovább. Amikor a
// tömb(ök)ről lekerül az array_accumulater objektum, a tömb(ök) visszaáll(nak)
// az eredeti állapotba. Az add művelettel további tömbök adhatóak az
// accumulaterhez, ilyenkor az összegzés az előző tömb végétől folytatódik az
// összegzés. Opcionális template paraméterként az összeadás művelet tetszőleges
// bináris műveletre cserélhető.


#include <iostream>
#include "arracc.h"
#include <string>
#include <functional>
#include "arracc.h"

struct custom_op
{

  double operator()( double lhs, double rhs ) const
  {
    return lhs + rhs + 2;
  }

};

bool check()
{
  int s[] = { 6, 3, 4, 8, 2 };
  std::string ws[] = { "A", "red", "house", "is", "built" };
  bool b = false;

  if ( !b )
  {
    array_accumulater<int> aci( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    array_accumulater<std::string> acs( ws,
                                        sizeof( ws ) / sizeof( ws[ 0 ] ) );

    b = s[ 3 ] == 21 && s[ 4 ] == 23 && ws[ 2 ].size() == 1U * s[ 1 ];
  }
  if ( !b || "red" != ws[ 1 ] || 2 != s[ 4 ] )
  {
    return false;
  }

  b = false;
  int t[] = { 8, 2 };
  int a[] = { 1, 1, 2, 1 };
  if ( !b )
  {
    array_accumulater<int> aci( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    aci.add( t, sizeof( t ) / sizeof( t[ 0 ] ) );
    aci.add( a, sizeof( a ) / sizeof( a[ 0 ] ) );

    const array_accumulater<std::string> acs( ws,
                                              sizeof( ws ) / sizeof( ws[ 0 ] ) );

    b = ws[ 3 ].size() == aci.size() && 9 == s[ 1 ] &&
        33 == t[ 1 ] && 34 == a[ 0 ] && 5 == acs.size();
  }
  if ( !b || 1 != a[ 0 ] || 3 != t[ 1 ] + a[ 1 ] )
  {
    return false;
  }

  b = false;
  double d[] = { 1.5, 2.7 };
  if ( !b )
  {
    array_accumulater<int, std::multiplies<int> > m( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    m.add( a, sizeof( a ) / sizeof( a[ 0 ] ) );
    array_accumulater<double, custom_op> acd( d, sizeof( d ) / sizeof( d[ 0 ] ) );

    b = 576 == s[ 3 ] && 2 == a[ 0 ] / s[ 3 ] && d[ 1 ] > 5.5;
  }

  return b && 1 == a[ 0 ] && 4 == s[ 2 ];
}

int main()
{
  std::cout << "Your solution is "
            << (check() ? "" : "not ")
            << "ready for submission."
            << std::endl;
}