
/*
 * This demonstrate Sub Blocks of array writting using no XML API
 * Each rank will have 2 chunk of data to write in data array
 *
 *
 */


#include <iostream>
#include <mpi.h>
#include <adios.h>
#include <random>
#include <algorithm>
#include <string>
#define MASTER 0

static int64_t                               adios_handle;
static int64_t                               adios_group_id;
static uint64_t                              adios_groupsize;
static uint64_t                              adios_totalsize;
static uint64_t                              data_id1, data_id2;

static MPI_Comm                               comm = MPI_COMM_WORLD;
typedef std::uniform_int_distribution<uint64_t> UniformRnd;
static const char*          out_file    =  "no_xml_several_chunk";
static std::random_device   generator;

void open (const char* filename) {
  adios_open (&adios_handle, "report", filename, "w", comm);
}

void write (float* buffer, uint64_t off[2], uint64_t global_size, uint64_t batch_size){
  adios_groupsize = 4 \
                + 4 \
                + 4 \
                + 4 * (batch_size);
  adios_group_size (adios_handle, adios_groupsize, &adios_totalsize);

  adios_write (adios_handle, "global_size", &global_size);
  adios_write (adios_handle, "batch_size",  &batch_size);
  for (int i = 0; i < 2; i++) {
    adios_write (adios_handle, "offset",      &off[i]);
    adios_write (adios_handle, "data",        &buffer[batch_size*i]);
    // adios_write_byid (adios_handle, data_id,  &buffer[batch_size[0]*i]);
  }
}

void close () {
  adios_close(adios_handle);
}

void initAdios () {
  adios_init_noxml    (comm);
  adios_declare_group (&adios_group_id,"report", "", adios_stat_no);
  adios_select_method (adios_group_id, "MPI",    "verbose=3", "");
  adios_define_var    (adios_group_id, "global_size",  "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "batch_size",   "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "offset",       "", adios_integer, 0,0,0);
  adios_define_var    (adios_group_id, "offset",       "", adios_integer, 0,0,0);
  data_id1  = adios_define_var (adios_group_id, "data",         "", adios_real, "batch_size", "global_size", "offset");
  data_id2  = adios_define_var (adios_group_id, "data",         "", adios_real, "batch_size", "global_size", "offset");
}


int main (int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <total_elems> <deviation>" << std::endl;
    return -1;
  }
  int                                   rank;
  int                                   nb_ranks;
  uint64_t                              total_size      = atoi(argv[1]);
  float*                                buffer          = nullptr;
  uint64_t                              batch_size      = 0;
  uint64_t                              offset     [2]  = {0,0};
  std::vector<uint64_t>                 offsets         = {};
  std::vector<uint64_t>                 ranks;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nb_ranks);
  initAdios();
  batch_size = total_size / (nb_ranks*2); // each rank have 2 chunks to write
  if (rank == MASTER) {
    offsets.resize (nb_ranks*2, 0);
    ranks.resize (nb_ranks*2, 0);
    for (auto i = 0; i < ranks.size();i++) ranks[i]   = i;
    std::random_shuffle (ranks.begin(), ranks.end());
    uint64_t stride = 0;
    for (auto r : ranks) {
      offsets[r] = stride;
      stride += batch_size;
    }
  }
  MPI_Scatter (offsets.data(),     2, MPI_UINT64_T, offset,     2, MPI_UINT64_T, 0, comm);
  std::cout << "[" << rank << "]:[0]" << batch_size << ";" << offset[0] << std::endl;
  std::cout << "[" << rank << "]:[1]" << batch_size << ";" << offset[1] << std::endl;
  
  buffer = (float*) malloc (batch_size * 2 * sizeof(float));
  for (int i = 0; i < batch_size*2; i++) {
    buffer[i] = rank;
  }
  open (out_file);
  write (buffer, offset, total_size, batch_size);
  close();
  adios_finalize(rank);
  MPI_Finalize();
  return 0;
}

