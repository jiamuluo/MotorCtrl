#ifndef TYPES_H
#define TYPES_H

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

typedef unsigned char UINT8;
typedef unsigned short UINT16;

typedef enum
{
    ERROR_TIMEOUT = -20,
    ERROR_INPUTINVALID,
    ERROR_NODATA,
    ERROR_WRONGDATA,
    ERROE_NO = 0,

}ERROR_INDEX;
#endif // TYPES_H
