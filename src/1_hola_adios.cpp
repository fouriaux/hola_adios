/*
 *     _ _           _            _       _ _           _ 
 *    (_) |__   ___ | | __ _     / \   __| (_) ___  ___| |
 *    | | '_ \ / _ \| |/ _` |   / _ \ / _` | |/ _ \/ __| |
 *    | | | | | (_) | | (_| |  / ___ \ (_| | | (_) \__ \_|
 *    \_|_| |_|\___/|_|\__,_| /_/   \_\__,_|_|\___/|___(_)

 *  ADIOS READ 
 *  read 3 characters per rank of a string in a file using 4 MPI ranks, and agregate
 */

#include <iostream>
#include <assert.h>
#include <mpi.h>
#include <adios.h>
#include <adios_read.h>

#define MASTER 0

static const char*  data_filename = "data/0.bp";
int main (int argc, char** argv) {
  int       rank;
  int       com_size;
  MPI_Comm  comm = MPI_COMM_WORLD; 

  char*     msg_part   = nullptr;
  char*     data       = nullptr;
  size_t    batch_size = 0;

  /* Initialization */
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &com_size);
  adios_init ("xml/0_config.xml", comm);
  adios_read_init_method(ADIOS_READ_METHOD_BP, comm, "verbose=1");

  /*
   * Easy way to read: you know exactly size of what you want to retrieve msg_part
   * if (ADIOS_FILE* fp = adios_read_open(data_filename, ADIOS_READ_METHOD_BP, comm, ADIOS_LOCKMODE_NONE, 0.0)) {
   *    adios_schedule_read (fp, NULL, "/hola_msg", 0,1,   &msg_part);
   *    adios_perform_reads (fp, 1);  // performs all reads in blocking mode
   *    adios_read_close(fp);
   * }
   */

  if (ADIOS_FILE* fp = adios_read_open(data_filename, ADIOS_READ_METHOD_BP, comm, ADIOS_LOCKMODE_NONE, 0.0)) {
    ADIOS_SELECTION* process_select = adios_selection_writeblock(rank);

    // query information in order to allocate variable
    ADIOS_VARINFO* hola_msg = adios_inq_var (fp, "/hola_msg");
    adios_inq_var_blockinfo (fp, hola_msg);
    assert (com_size == hola_msg->nblocks[0]);
    batch_size = hola_msg->dims[0];
    if (rank == MASTER) {
      std::cout << "nb_blocks  = " << hola_msg->nblocks[0]  << std::endl;
      std::cout << "batch_size = " <<  batch_size << std::endl;
    }
    // allocate necessary buffers
    msg_part = (char*) malloc (sizeof(char)*batch_size);
    if (rank == MASTER) {
      data = (char*) malloc (sizeof(char)*batch_size*com_size);
    }

    // perform read operations
    adios_schedule_read (fp, process_select, "/hola_msg", 0,1,   msg_part);
    adios_perform_reads (fp, 1);                      // commit all schedule reads, in blocking mode
    adios_free_varinfo (hola_msg);
    adios_read_close(fp);
  }

  MPI_Gather(msg_part, batch_size, MPI_CHAR, data, batch_size, MPI_CHAR, MASTER, comm);
  if (rank == MASTER) {
    std::cout << "data        = " << data << std::endl;
  }
  adios_finalize(rank);
  MPI_Finalize ();
  return 0;
}
