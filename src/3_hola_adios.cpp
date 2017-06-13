#include <iostream>
#include <cstring> 
/*
 *     _ _           _            _       _ _           _ 
 *    (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
 *    | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
 *    | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
 *    \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

 *
 *  TODO We got data structure to write in a file and retreive
 *       We will write structure as a byte stream and try to see if we can access via attributes ???
 * */ 


struct HOLA_ADIOS { 
 char first_line 	[53];
 char second_line	[53];
 char third_line 	[53];
 char forth_line 	[53];
 char fifth_line 	[53];
};

std::ostream& operator<<(std::ostream& s, const HOLA_ADIOS& hola) {
  s << hola.first_line  << std::endl;
  s << hola.second_line << std::endl;
  s << hola.third_line  << std::endl;
  s << hola.forth_line  << std::endl;
  s << hola.fifth_line  << std::endl;
  return s;
}
int main (int argc, char** argv) {
  HOLA_ADIOS hola;
  strncpy (hola.first_line,   R"( _ _           _            _       _ _           _ )", sizeof(hola.first_line));
  strncpy (hola.second_line,  R"((_) |__   ___ | | __ _     / \   __| (_) ___  ___| |)", sizeof(hola.first_line));
  strncpy (hola.third_line,   R"(| | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |)", sizeof(hola.first_line));
  strncpy (hola.forth_line,   R"(| | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|)", sizeof(hola.first_line));
  strncpy (hola.fifth_line,   R"(\_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_))", sizeof(hola.first_line));


  // write struct in ADIOS file
  //
  // read struct from ADIOS file
  std::cout << hola << std::endl;
  return 0;
}
