#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    //-> Call check_operation_type(argv[1][1])==e_encode
    /*
        ->call read_and_validate_encode_args(argv,&encInfo)==e_success
            =>call do_encoding(&encodeInfo)==e_success
                print "Encoding is success"
    */
    if(argc<2)
    {
        printf("Invalid Input\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For dencoding: ./a.out -d stego.bmp [decode.txt]\n");
        return 1;
    }
    
    if(check_operation_type(argv[1][1])==e_encode)
    {
        if(read_and_validate_encode_args(argc,argv,&encInfo)==e_success)
        {
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding is success\n");
            }
            else
            {
                printf("Encoding is failed\n");
            }
        }
        else
        {
            printf("Encoding arguments are Invalid\n");
        }
    }
    else if(check_operation_type(argv[1][1])==e_decode)
    {
        if(read_and_validate_decode_args(argc,argv,&decInfo)==e_success)
        {
            if(do_decoding(&decInfo)==e_success)
            {
                printf("Decoding is success\n");
            }
            else
            {
                printf("Decoding is failed\n");
            }
        }
        else
        {
            printf("Decoding arguments are Invalid\n");
        }
    }
    else
    {
        printf("Invalid Operation\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For dencoding: ./a.out -d stego.bmp [decode.txt]\n");
        return 1;
    }
    
    return 0;
}

OperationType check_operation_type(char opt)
{
    /*
        *check opt is'e'
            return e_encode;
        *check opt is 'd'
            return e_encode;
        *else
            return e_unsupported;
    */
    if(opt=='e')
    {
        return e_encode;
    }
    else if(opt=='d')
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
    return 0;
}
