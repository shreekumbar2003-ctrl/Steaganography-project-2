#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Src image*/
    char *src_image_fname;
    FILE *fptr_src_image;

    /*  secret file */
    char *secret_fname;
    FILE *fptr_secret;

    /* secret file information */
    char extn_secret_file[MAX_FILE_SUFFIX+1];
    int extn_size;
    int size_secret_file;
}DecodeInfo;

/* check operation type */
OperationType check_operation_type(char opt);

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* open files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode size from LSB */
Status decode_size_from_lsb(char *image_buffer, int *size);

/* Decode one byte from LSB */
Status decode_byte_from_lsb(char *image_buffer,char *data);


/**/

#endif