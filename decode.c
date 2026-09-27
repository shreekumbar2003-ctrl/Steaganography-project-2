#include<stdio.h>
#include<string.h>

#include "decode.h"
#include "types.h"
#include "common.h"



Status open_decode_files(DecodeInfo *decInfo)
{
    //src image file
    decInfo->fptr_src_image=fopen(decInfo->src_image_fname,"r");

    // Do Error handling
    if (decInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    return e_success;

}


Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo)
{
    if(argc<3)
    {
        printf("Invalid input\n");
        printf("For decoding: ./a.out -d stego.bmp [output.txt]\n");
        
        return e_failure;
    }

    if(strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("ERROR:Source image is not .bmp file\n");
        return e_failure;
    }

    decInfo->src_image_fname=argv[2];

    if(argv[3]==NULL)
    {
        decInfo->secret_fname="decoded.txt";
    }
    else
    {
        decInfo->secret_fname=argv[3];
    }
    if(open_decode_files(decInfo)==e_failure)
    {
        return e_failure;
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    
    if(decode_magic_string(MAGIC_STRING,decInfo)==e_failure)
    {
        return e_failure;
    }

    if(decode_secret_file_extn_size(decInfo)==e_failure)
    {
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo)==e_failure)
    {
        return e_failure;
    }

    if(decode_secret_file_size(decInfo)==e_failure)
    {
        return e_failure;
    }

    if(decode_secret_file_data(decInfo)==e_failure)
    {
        return e_failure;
    }
    return e_success;
}

/*decode one byte from image*/
Status decode_byte_from_lsb(char *image_buffer,char *data)
{
    *data=0;

    for(int i=0;i<8;i++)
    {
        *data=*data<<1;
        if(image_buffer[i] & 1)
        {
            *data=*data | 1;
        }
    }
    return e_success;
}

/* decode magic string */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);
    char buffer[8];
    char data;

    for(int i=0;i<strlen(magic_string);i++)
    {
        fread(buffer,8,1,decInfo->fptr_src_image);

        decode_byte_from_lsb(buffer,&data);
        if(data!=magic_string[i])
        {
            return e_failure;
        }
    }
    return e_success;
}

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer,32,1,decInfo->fptr_src_image);

    if(decode_size_from_lsb(buffer,&decInfo->extn_size)==e_failure)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_size_from_lsb(char *image_buffer,int *size)
{
    *size=0;
    for(int i=31;i>=0;i--)
    {
        *size=*size << 1;
        if(image_buffer[31-i] & 1)
        {
            *size=*size | 1;
        }
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];

    for(int i=0;i<decInfo->extn_size;i++)
    {
        fread(buffer,8,1,decInfo->fptr_src_image);
        decode_byte_from_lsb(buffer, &decInfo->extn_secret_file[i]);
    }
    decInfo->extn_secret_file[decInfo->extn_size]='\0';

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer,32,1,decInfo->fptr_src_image);

    if(decode_size_from_lsb(buffer,&decInfo->size_secret_file)==e_failure)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char filename[50];
    strcpy(filename,decInfo->secret_fname);
    char *dot=strrchr(filename,'.');
    if(dot!=NULL)
    {
        *dot='\0';
    }
    strcat(filename,decInfo->extn_secret_file);
    decInfo->fptr_secret=fopen(filename,"w");
    if(decInfo->fptr_secret==NULL)
    {
        perror("fopen");
        return e_failure;
    }
    
    char buffer[8];
    char data;

    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        fread(buffer,8,1,decInfo->fptr_src_image);

        if(decode_byte_from_lsb(buffer,&data)==e_failure)
        {
            return e_failure;
        }
        fwrite(&data,1,1,decInfo->fptr_secret);
    }
    fclose(decInfo->fptr_secret);
    return e_success;
}
