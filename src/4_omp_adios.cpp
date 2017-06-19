#include <iostream>
#include <cstring> 
/*
 *     _ _           _            _       _ _           _ 
 *    (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
 *    | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
 *    | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
 *    \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

 *
 *  ADIOS is not threads safe: This example demonstrate 1 MPI rank and several openMP threads
 *  this coredump inside adios lib
 *
 *
 */


#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <adios.h>

static const char*  data_filename = "data/4.bp";
static const size_t line_size     = 53;
static const size_t nb_lines      = 5;

int main (int argc, char** argv) {
  int64_t  adios_handle;
  char      hola  [5][53];
  MPI_Comm  comm = MPI_COMM_WORLD;
  MPI_Init (&argc, &argv);
  adios_init ("xml/4_config.xml", comm);
  omp_set_num_threads(5);
  strncpy (&hola[0][0],  R"( _ _           _            _       _ _           _ )", 53);
  strncpy (&hola[1][0],  R"((_) |__   ___ | | __ _     / \   __| (_) ___  ___| |)", 53);
  strncpy (&hola[2][0],  R"(| | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |)", 53);
  strncpy (&hola[3][0],  R"(| | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|)", 53);
  strncpy (&hola[4][0],  R"(\_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_))", 53);

  //NOTE: moving adios_open and close inside parallel section still raise an error at runtime inside adios lib
  adios_open  (&adios_handle, "hola", data_filename, "w", comm);
#pragma omp parallel for
  for (int i = 0; i < 5; i++) {
 //   adios_write (adios_handle, "line_size", 53);   cannot pass a constant
 //   adios_write (adios_handle, "nb_lines", 5);     cannot pass a constant
    adios_write (adios_handle, "line_size", &line_size);
    adios_write (adios_handle, "nb_lines", &nb_lines);
    adios_write (adios_handle, "line_idx", &i);
    adios_write (adios_handle, "/hola_msg", &hola[i]);
  }
  adios_close (adios_handle);


  // write struct in ADIOS file
  //
  for (int i = 0; i < 5; i++)
    std::cout << hola[i] << std::endl;

  adios_finalize(0);
  MPI_Finalize();
  return 0;
}
