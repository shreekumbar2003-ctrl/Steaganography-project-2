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
    }
    else
    {
        printf("Unsupported operation\n");
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
