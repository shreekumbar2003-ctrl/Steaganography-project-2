#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(int argc,char *argv[],EncodeInfo *encInfo)
{
/*
    ->check argv[2] have ".bmp" as last 4 char
        *if not ,print error msg ,return r_failure
    encInfo -> src_image_fname =argv[2]

    encInfo -> secret_fname =argv[3]

    ->check argv[4]==NULL
        encInfo ->stego_image_fname="output.bmp"

    ->else
        * validate argv[4] is".bmp"
            =>if not,print error msg,return e_failure
        *encInfo -> stego_image_fname=argv[4]

    ->call open_file(encInfo)==e_failure
        return e_failure

    return e_success
    */
    if(argc<4)
    {
        printf("Invalid Input\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For dencoding: ./a.out -d stego.bmp [decode.txt]\n");
        return e_failure;
    }

    if(strcmp(argv[2]+strlen(argv[2])-4,".bmp")!=0)
    {
        printf("Error:Source image is not .bmp file\n");
        return e_failure;
    }
    encInfo->src_image_fname=argv[2];
    encInfo->secret_fname=argv[3];

    if(argv[4]==NULL)
    {
        encInfo->stego_image_fname="output.bmp";
    }
    else
    {
        if(strcmp(argv[4]+strlen(argv[4])-4,".bmp")!=0)
        {
            printf("ERROR:Output image is not .bmp file\n");
            return e_failure;
        }
        encInfo->stego_image_fname=argv[4];
    }
    if(open_files(encInfo)==e_failure)
    {
        return e_failure;
    }
    return e_success;
}

/*Status open_files(EncodeInfo *encInfo)
{
    
        ->open 'encInfo -> src_image_fname' file in read mode
            *if ret value is NULL,print error,return e-failure
            fptr_src_image=fopen()

        ->open 'encInfo -> secret_file' file in read mode
            *if ret value is NULL,print error,return e-failure
            fptr_secret=fopen()

        ->open 'encInfo -> stego_image_fname' file in write mode
            fptr_stego_image=fopen()

        ->return e_success
        
    
    encInfo->fptr_src_image=fopen(encInfo->src_image_fname,"r");
    if(encInfo->fptr_src_image==NULL)
    {
        printf("ERROR:Unable to open source image\n");
        return e_failure;
    }

    encInfo->fptr_secret=fopen(encInfo->secret_fname,"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("ERROR:Unable to open secret file\n");
        return e_failure;
    }

    encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
    if(encInfo->fptr_stego_image==NULL)
    {
        printf("Error:Unable to open stego image\n");
        return e_failure;
    }
    return e_success;
        
}*/

Status do_encoding(EncodeInfo *encInfo)
{
    /*
        //call Check_capacity(encInfo)==e_failure
            print error msg,return e_failure
            
        // Call copy_bmp_header(fptr_src_file,fptr_dest_file)
            print error msg,return e_failure

        //Call encode_magic_string(MAGIC_STRING,encInfo)==e_failure
            print erroe msg,return e_failurels

        //Call encode_secret_file_extn_size(encInfo)==e_failure
            print error msg,return e_failure

        //Call encode_secret_file_extn(extn_secret_file,encInfo)==e_failure
            print error msg,return e_failure

        //Call encode_secret_file_size(size_secret_file,encInfo)==e_failure
            print error msg,return e_failure

        //Call encode_secret_file_data(encInfo)==e_failure
            print error msg,return e_failure
            
        //Call copy_remaining_img_data(fptr_src_file,fptr_dest_file)==e_failure
            print error msg,return e_failure

    */
    if(check_capacity(encInfo)==e_failure)
    {
        printf("Error:Failed to check capacity\n");
        return e_failure;
    }

    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("Error:Failed to copy BMP header\n");
        return e_failure;
    }

    if(encode_magic_string(MAGIC_STRING,encInfo)==e_failure)
    {
        printf("Error:Failed to encode magic string\n");
        {
            return e_failure;
        }
    }

    if(encode_secret_file_extn_size(encInfo)==e_failure)
    {
        printf("Error:Failed to encode extension size\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_failure)
    {
        printf("Error:Failed to encode extension\n");
        return e_failure;
    }

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure)
    {
        printf("Error:Failed to encode secret file size\n");
        return e_failure;
    }

    if(encode_secret_file_data(encInfo)==e_failure)
    {
        printf("Error:Failed to encode secret file data\n");
        return e_failure;
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure)
    {
        printf("Error:Failed to copy remaining image data\n");
        return e_failure;
    }

    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    /*
        ->call get_image_size_for_bmp(encode->fptr_src_image)
            image_capacity=get_image_size()
        ->Call get_file_size(encode -> fptr_secret)
            size_secret=get_file-size()

        ->check ((14 + size_secret_file)*8)>image_cpacity
            return e_failure
        ->return e_success
    */
    //get image capacity
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);

    //get secter file size
    encInfo->size_secret_file=get_file_size(encInfo -> fptr_secret);

    //check capacity
    if((14 + encInfo->size_secret_file)*8>encInfo->image_capacity)
    {
        printf("Error: Insufficient image capacity\n");
        return e_failure;
    }
    return e_success;


}

uint get_file_size(FILE *fptr)
{
    /*
        ->move the offset to last pos
        -> return ftell()
    */
    fseek(fptr,0,SEEK_END);
    uint size = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return size;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        ->move the file pointers to the SEEK_SET
        ->declare the buff[54]
        ->Read 54 bytes from src file
        ->Write 54 bytes to dest file

        ->return e_success
    */
    fseek(fptr_src_image,0,SEEK_SET);
    fseek(fptr_dest_image,0,SEEK_SET);

    char buffer[54];

    fread(buffer,54,1,fptr_src_image);

    fwrite(buffer,54,1,fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        declare a buff of 8 bytes
        ->loop for(length )
        ->Read 8 bytes from the src_file into buff
        encode_byte_to_lsb(magic_string[1],buff)
        write the encoded buff to output_file
    */

    char buffer[8];
    for(int i=0;i<strlen(magic_string);i++)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);

        encode_byte_to_lsb(magic_string[i],buffer);

        fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        for(int i=7;i>=0;i--)
        {
            ->get the ith bit is set or not
                =>if set,set the LSB of image_buffer[]
                =>if clear,clear the LSB of image_buffer[]
        }
    */
    for(int i=7;i>=0;i--)
    {
        if((data>>i)&1)
        {
            image_buffer[7-i]=image_buffer[7-i]|1;
        }
        else
        {
            image_buffer[7-i]=image_buffer[7-i]& ~1;
        }
    }
    return e_success;
}

Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    /*
        ->char *dot = strchr(secret_file_name,'.')
        ->strcpy(extn_secret_file,dot)
        ->Declare a buff[32]

        ->Read 32 bytes  from src_file into buff
        ->call encode_size_to_lsb(strlen(extn_secret_file),buff)
        ->Write 32 bytes of buff to output_file
    */
    char *dot;
    char buffer[32];

    dot=strchr(encInfo->secret_fname,'.');

    strcpy(encInfo->extn_secret_file,dot);

    fread(buffer,32,1,encInfo->fptr_src_image);

    encode_size_to_lsb(strlen(encInfo->extn_secret_file),buffer);

    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_size_to_lsb(int size,char *Image_buff)
{
    /*
        for(int i=31;i>=0;i--)
        {
            ->get the ith bit is set or not
                =>if set,set the LSB of image_buffer[]
                =>if clear,clear the LSB of image_buffer[]
        }
        //find error part and return e_failure
        return e_success;
        
    */
    for(int i=31;i>=0;i--)
    {
        if((size>>i)&1)
        {
            Image_buff[31-i]=Image_buff[31-i] | 1;
        }
        else
        {
            Image_buff[31-i]=Image_buff[31-i] & ~1;
        }
    }
    return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        declare buff[8]

        ->Read 8 bytes from src_image
        ->encode_byte_to_lsb(file_extn[],buff)
        ->Write the 8 bytes of buff to output_file

        return e_success
    */
    char buffer[8];

    for(int i=0;i<strlen(file_extn);i++)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i],buffer);

        fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;

}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        Declare the buff[32]
    
        -> Read the 32 bytes from src_image
        _> Call encode_size_to_lsb(file size,buff)
        ->Write the 32 bytes of buff to output_file

        return e_success;
    */
    char buffer[32];

    fread(buffer,32,1,encInfo->fptr_src_image);

    encode_size_to_lsb(file_size,buffer);

    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        Declare buff[8],data
    => loop till EOF of secret_file
        ->Read 8 bytes from src-file
        ->Read 1 byte from secret_file
        ->encode_byte_to_lsb(data,buff)
    */
    char buffer[8];
    char data;

    while((data=fgetc(encInfo->fptr_secret))!=EOF)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);

        encode_byte_to_lsb(data,buffer);

        fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        Declare a char as data
    =>loop till EOF of src_file
        ->Read a cha from src_file
        ->Write the data to dest_file

        return e_success
    */

    char data;
    while(fread(&data,1,1,fptr_src)==1)
    {
        fwrite(&data,1,1,fptr_dest);
    }
    return e_success;
}

