/*
 *     _ _           _            _       _ _           _ 
 *    (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
 *    | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
 *    | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
 *    \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

 *
 *  write 3 character of a string in a file by 4 MPI ranks
 *  We then can use adios binaries to read the file
 */

#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <adios.h>

#define MASTER 0

static const size_t nb_chars      = 12;
// for now we replace leading ¡ by ! to get same number of characters per rank  (we fall on a prime otherwise T_T)
static const char*  data          = u8"!Hola Adios!";
static const char*  data_filename = "data/0.bp";
int main (int argc, char** argv) {
  int       rank;
  int       com_size;
  MPI_Comm  comm = MPI_COMM_WORLD; 
  
  uint64_t  adios_groupsize, adios_totalsize;
  int64_t   adios_handle;
  
  size_t    batch_size            = 0;

  /* Initialization */
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &com_size);
  adios_init ("xml/0_config.xml", comm);
  batch_size = nb_chars /com_size;
  if (rank == MASTER)
    std::cout << data << std::endl;

  adios_open  (&adios_handle, "hola", data_filename, "w", comm);
  adios_write (adios_handle, "batch_size", &batch_size);
  adios_write (adios_handle, "/hola_msg", &data[rank*batch_size]);
  adios_close (adios_handle);

  adios_finalize(rank);
  MPI_Finalize ();
  return 0;
}
