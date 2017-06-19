
/*
 * This demonstratew adios no-xml API
 *
 *
 */


#include <iostream>
#include <mpi.h>
#include <adios.h>
#include <random>
#include <unordered_map>
#include <string>
#define MASTER 0

static int64_t                               adios_handle;
static int64_t                               adios_group_id;
static uint64_t                              adios_groupsize;
static uint64_t                              adios_totalsize;
static uint64_t                              data_id;

static MPI_Comm                               comm = MPI_COMM_WORLD;
typedef std::uniform_int_distribution<uint64_t> UniformRnd;
static const char*          out_file    =  "no_xml";
static std::random_device   generator;

void open (const char* filename) {
  adios_open (&adios_handle, "report", filename, "w", comm);
}

void write (float* buffer, uint64_t offset, uint64_t global_size, uint64_t batch_size){
  uint64_t off        = offset;
  uint64_t glob       = global_size;
  uint64_t batch      = batch_size;
  adios_groupsize = 4 \
                + 4 \
                + 4 \
                + 4 * (batch_size);
  adios_group_size (adios_handle, adios_groupsize, &adios_totalsize);

  adios_write (adios_handle, "global_size", &glob);
  adios_write (adios_handle, "batch_size",  &batch);
  adios_write (adios_handle, "offset",      &off);
  adios_write_byid (adios_handle, data_id,   buffer);
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
  data_id  = adios_define_var (adios_group_id, "data",         "", adios_real, "batch_size", "global_size", "offset");
}


int main (int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <total_elems> <deviation>" << std::endl;
    return -1;
  }
  int                                   rank;
  int                                   nb_ranks;
  uint64_t                              total_size      = atoi(argv[1]);
  double                                rank_dev        = atof(argv[2]);
  float*                                buffer          = nullptr;
  uint64_t                              batch_size      = 0;
  uint64_t                              offset          = 0;
  std::vector<uint64_t>                 batch_sizes     = {};
  std::vector<uint64_t>                 offsets         = {};
  std::unordered_map <uint64_t, bool>   rank_allocated;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nb_ranks);
  initAdios();
  if (rank == MASTER) {
    UniformRnd rank_selection  (0, nb_ranks-1);
    batch_sizes.resize (nb_ranks, 0);
    offsets.resize (nb_ranks, 0);
    for (int i = 0 ; i < nb_ranks; i++) {
      rank_allocated[i] = false;
    }
    uint64_t    var_chunk = (total_size / nb_ranks)*rank_dev;
    uint64_t    min_chunk = (total_size / nb_ranks) - var_chunk;
    uint64_t    nb_allocated = 0;
    uint64_t    start = 0;
    UniformRnd  size_selection (0, var_chunk*2);
    while ((start <= total_size) && (nb_allocated < nb_ranks)) {
      uint64_t      rank_candidate = rank_selection (generator);
      do {
        rank_candidate = rank_selection (generator);
      } while (rank_allocated[rank_candidate] == true);
      nb_allocated++;

      uint64_t chunk_size =  min_chunk + size_selection(generator);
      chunk_size          =  (start + chunk_size) < total_size ? chunk_size: total_size - start;
      offsets         [rank_candidate]    = start;
      batch_sizes     [rank_candidate]    = chunk_size;
      rank_allocated  [rank_candidate]    = true;
      start +=chunk_size;
    }
  }
  MPI_Scatter (batch_sizes.data(), 1, MPI_UINT64_T, &batch_size, 1, MPI_UINT64_T, 0, comm);
  MPI_Scatter (offsets.data(),     1, MPI_UINT64_T, &offset,     1, MPI_UINT64_T, 0, comm);
  std::cout << "[" << rank << "]:" << batch_size << ";" << offset << std::endl;
  buffer = (float*) malloc (batch_size * sizeof(float));
  for (int i = 0; i < batch_size; i++) {
    buffer[i] = rank;
  }
  open (out_file);
  write (buffer, offset, total_size, batch_size);
  close();
  adios_finalize(rank);
  MPI_Finalize();
  return 0;
}

