
/*
 * This demonstrate Sub Blocks of array writting using no XML API
 * Each rank will have 2 chunk of data to write in data array
 * Each rank is writting is own rank ID on file to easy visualize results
 *
 */


#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#define MASTER 0


static int                                   rank;
static int                                   nb_ranks;
static uint64_t                              total_size;
static char*                                 buffer;
static uint64_t                              batch_size;
static uint64_t                              splitting;
static const char*                           method;


static int*                                  displacements = NULL;
static MPI_Comm                              comm         = MPI_COMM_WORLD;
static MPI_File                              fh;
static MPI_Datatype                          arraytype;
static MPI_Offset                            disp;

static const char*                           out_file     = "mpiio.dat";

void open (const char* filename) {
  displacements = (int*) malloc (splitting*(sizeof(int)));
  for (int i = 0; i < splitting; i++){
    displacements[i] = nb_ranks*batch_size*i;
  }
  disp = rank*batch_size; // begining of file viewed from rank
  MPI_Type_create_indexed_block (splitting, batch_size, b_disp, MPI_BYTE, &arraytype);
  MPI_Type_commit(&arraytype);

int result =
  MPI_File_open ( comm, out_file,
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh
                );
  if (result != MPI_SUCCESS)
      std::cerr << "Error opening the file " << std::endl;
  MPI_File_set_view  (fh, disp, MPI_BYTE,  arraytype, "native", MPI_INFO_NULL);
  MPI_Barrier(MPI_COMM_WORLD);
}

void write (char* buffer){
  MPI_File_write_all (fh, buffer, batch_size*splitting, MPI_BYTE, MPI_STATUS_IGNORE);
}

void close () {
  MPI_Barrier    (MPI_COMM_WORLD);
  MPI_File_close (&fh);
}

int main (int argc, char** argv) {
  if (argc != 4) {
    std::cerr << "usage: " << argv[0] << " <nb bytes per write per rank> <transport method> <nb writes per rank>" << std::endl;
    return -1;
  }
  total_size      = 0;
  buffer          = NULL;
  batch_size      = atoi(argv[1]);
  method          = argv[2];
  splitting       = atoi(argv[3]);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &nb_ranks);
  total_size = batch_size * nb_ranks * splitting;
  buffer     = (char*) malloc (batch_size*splitting);
  open (out_file);
  write (buffer);
  close();
  MPI_Finalize();
  return 0;
}
