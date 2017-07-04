
/*
 * This demonstrate Sub Blocks of array writting using no XML API
 * Each rank will have 2 chunk of data to write in data array
 * Each rank is writting is own rank ID on file to easy visualize results
 *
 */


#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <adios.h>
#define MASTER 0

static int64_t                               adios_handle;
static int64_t                               adios_group_id;
static uint64_t                              adios_groupsize;
static uint64_t                              adios_totalsize;
static uint64_t                              data_id1, data_id2;

static MPI_Comm                              comm = MPI_COMM_WORLD;
static const char*          out_file    =  "no_xml_several_chunk";

void open (const char* filename) {
  adios_open (&adios_handle, "report", filename, "w", comm);
}

void write (char* buffer, uint64_t global_size, uint64_t batch_size, int rank){
  adios_groupsize = 4 \
                + 4 \
                + 4 \
                + 4 * (batch_size);
  size_t offset   = batch_size*rank;
  adios_group_size (adios_handle, adios_groupsize, &adios_totalsize);
  adios_write (adios_handle, "global_size", &global_size);
  adios_write (adios_handle, "batch_size",  &batch_size);
  adios_write (adios_handle, "offset",      &offset);
  adios_write (adios_handle, "data",        buffer);
}

void close () {
  adios_close(adios_handle);
}

void initAdios () {
  adios_init_noxml    (comm);
  adios_declare_group (&adios_group_id,"report", "", adios_stat_no);
  adios_select_method (adios_group_id, "MPI",    "verbose=0", "");
  adios_define_var    (adios_group_id, "global_size",  "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "batch_size",   "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "offset",       "", adios_integer, 0,0,0);
  data_id1  = adios_define_var (adios_group_id, "data",         "", adios_byte, "batch_size", "global_size", "offset");
}


int main (int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <nb_floats_per_rank>" << std::endl;
    return -1;
  }
  int                                   rank;
  int                                   nb_ranks;
  uint64_t                              total_size      = 0;
  char*                                 buffer          = NULL;
  uint64_t                              batch_size      = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nb_ranks);
  initAdios();
  total_size = batch_size * nb_ranks;
  buffer = (char*) malloc (batch_size);
  open (out_file);
  write (buffer, total_size, batch_size, rank);
  close();
  adios_finalize(rank);
  MPI_Finalize();
  return 0;
}

