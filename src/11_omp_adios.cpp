#include <iostream>
#include <cstring> 
/*
 *     ___  __  __ ____       _    ____ ___ ___  ____
 *    / _ \|  \/  |  _ \     / \  |  _ \_ _/ _ \/ ___|
 *   | | | | |\/| | |_) |   / _ \ | | | | | | | \___ \
 *   | |_| | |  | |  __/   / ___ \| |_| | | |_| |___) |
 *    \___/|_|  |_|_|     /_/   \_\____/___\___/|____/
 *
 *
 *  Thread Safe ADIOS using no-xml API ?
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

  adios_init_noxml    (comm);
  adios_declare_group (&adios_group_id,"report", "", adios_stat_no);
  adios_select_method (adios_group_id, "MPI",    "verbose=3", "");
  adios_define_var    (adios_group_id, "line_size",  "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "nb_lines",   "", adios_integer, 0,0,0);

  for (int i = 0; i < 5; i++)
  {
    adios_define_var             (adios_group_id, "line_idx", "", adios_integer, 0,0,0);
    data_id1  = adios_define_var (adios_group_id, "data",     "", adios_real, "1, line_size", "nb_lines, line_size", "line_idx,0");
  }
  adios_open  (&adios_handle, "hola", data_filename, "w", comm);
  adios_write (adios_handle, "line_size", &line_size);
  adios_write (adios_handle, "nb_lines", &nb_lines);
#pragma omp parallel for
  for (int i = 0; i < 5; i++) {
    adios_write (adios_handle, "line_idx", &i);
    adios_write (adios_handle, "data", &hola[i]);
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
