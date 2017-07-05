
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

static int                                   rank;
static int                                   nb_ranks;
static uint64_t                              total_size;
static char*                                 buffer;
static uint64_t                              batch_size;
static uint64_t                              splitting;
static uint64_t                              max_buffer_size; // buffer size in megabytes
static const char*                           method;

static MPI_Comm                              comm         = MPI_COMM_WORLD;
static const char*                           out_file     = "no_xml_several_chunk";

void open (const char* filename) {
  adios_open (&adios_handle, "report", filename, "w", comm);
}

void write (char* buffer){
  adios_groupsize = 4 \
                + 4 \
                + 4 \
                + 4 * (batch_size);
  adios_group_size (adios_handle, adios_groupsize, &adios_totalsize);
  adios_write (adios_handle, "global_size", &total_size);
  adios_write (adios_handle, "batch_size",  &batch_size);
  for (int i = 0 ; i < splitting; i++) {
    size_t offset   = batch_size*rank + batch_size*nb_ranks*i;
    adios_write (adios_handle, "offset",      &offset);
    adios_write (adios_handle, "data",        buffer);
  }
}

void close () {
  adios_close(adios_handle);
}

void initAdios (const char* method, uint64_t max_buffer_size) {
  adios_init_noxml    (comm);
  adios_set_max_buffer_size (max_buffer_size);
  adios_declare_group (&adios_group_id,"report", "", adios_stat_no);
  adios_select_method (adios_group_id, method,    "verbose=0", "");
  adios_define_var    (adios_group_id, "global_size",  "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "batch_size",   "", adios_integer, 0,0,0);
  for (int i = 0; i < splitting; i++) {
    adios_define_var    (adios_group_id, "offset",       "", adios_integer, 0,0,0);
    data_id1  = adios_define_var (adios_group_id, "data",         "", adios_byte, "batch_size", "global_size", "offset");
  }
}


int main (int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "usage: " << argv[0] << " <nb bytes per write per rank> <ADIOS buffer size> <transport method> <nb writes per rank>" << std::endl;
    return -1;
  }
  total_size      = 0;
  buffer          = NULL;
  batch_size      = atoi(argv[1]);
  max_buffer_size = atoi(argv[2]); // buffer size in megabytes
  method          = argv[3];
  splitting       = atoi(argv[4]);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nb_ranks);
  initAdios(method, max_buffer_size);
  total_size = batch_size * nb_ranks * splitting;
  buffer = (char*) malloc (batch_size);
  open (out_file);
  write (buffer);
  close();
  adios_finalize(rank);
  MPI_Finalize();
  return 0;
}
