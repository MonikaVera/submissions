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

struct custom_op
{

  double operator()( double lhs, double rhs ) const
  {
    return lhs + rhs + 2;
  }

};

template <typename T>
void printArr(const T* arr, int size) {
  for(int i=0; i<size; i++) {
    std::cout << arr[i] << ' ';
  }
  std::cout << std::endl;
}

bool check()
{
  int s[] = { 6, 3, 4, 8, 2 };
  std::string ws[] = { "A", "red", "house", "is", "built" };
  bool b = false;
  std::cout << "S: ";
  printArr(s, sizeof( s ) / sizeof( s[ 0 ] ));
  std::cout << "WS: ";
  printArr(ws, sizeof( ws ) / sizeof( ws[ 0 ] ));

  if ( !b )
  {
    array_accumulator<int> aci( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    std::cout << "Accumulated sum of S: ";
    printArr(s, sizeof( s ) / sizeof( s[ 0 ] ));
    array_accumulator<std::string> acs( ws,
                                        sizeof( ws ) / sizeof( ws[ 0 ] ) );
    std::cout << "Accumulated WS: ";
    printArr(ws, sizeof( ws ) / sizeof( ws[ 0 ] ));
    b = s[ 3 ] == 21 && s[ 4 ] == 23 && ws[ 2 ].size() == 1U * s[ 1 ];
  }
  if ( !b || "red" != ws[ 1 ] || 2 != s[ 4 ] )
  {
    return false;
  }

  std::cout << "S: ";
  printArr(s, sizeof( s ) / sizeof( s[ 0 ] ));
  b = false;
  int t[] = { 8, 2 };
  int a[] = { 1, 1, 2, 1 };
  std::cout << "T: ";
  printArr(t, sizeof( t ) / sizeof( t[ 0 ] ));
  std::cout << "A: ";
  printArr(a, sizeof( a ) / sizeof( a[ 0 ] ));

  if ( !b )
  {
    array_accumulator<int> aci( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    aci.add( t, sizeof( t ) / sizeof( t[ 0 ] ) );
    aci.add( a, sizeof( a ) / sizeof( a[ 0 ] ) );
    std::cout << "Accumulated S + T: ";
    printArr(t, sizeof( t ) / sizeof( t[ 0 ] ));
    std::cout << "Accumulated S + T + A: ";
    printArr(a, sizeof( a ) / sizeof( a[ 0 ] ));

    const array_accumulator<std::string> acs( ws,
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
    array_accumulator<int, std::multiplies<int> > m( s, sizeof( s ) / sizeof( s[ 0 ] ) );
    std::cout << "Accumulated product of S: ";
    printArr(s, sizeof( s ) / sizeof( s[ 0 ] ));
    m.add( a, sizeof( a ) / sizeof( a[ 0 ] ) );
    array_accumulator<double, custom_op> acd( d, sizeof( d ) / sizeof( d[ 0 ] ) );

    b = 576 == s[ 3 ] && 2 == a[ 0 ] / s[ 3 ] && d[ 1 ] > 5.5;
  }

  return b && 1 == a[ 0 ] && 4 == s[ 2 ];
}

int main()
{
  std::cout << (check() ? "correct" : "not correct") << std::endl;
}