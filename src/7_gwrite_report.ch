adios_groupsize = 4 \
                + 4 \
                + 4 \
                + 4 * (batch_size);
adios_group_size (adios_handle, adios_groupsize, &adios_totalsize);
adios_write (adios_handle, "global_size", &global_size);
adios_write (adios_handle, "offset", &offset);
adios_write (adios_handle, "batch_size", &batch_size);
adios_write (adios_handle, "data", buffer);
