#include "ff.h"

FRESULT f_open(FIL* fr, const TCHAR* name, BYTE ot)
{
    char open_param[5];
    memset(open_param, 0, 5);
    char* ptr = open_param;

    if (ot == FA_OPEN_EXISTING || ot == FA_READ)
    {
        *ptr = 'r';
        ++ptr;
    }
    else if (ot == FA_WRITE)
    {
        *ptr = 'w';
        ++ptr;
    }

    *ptr = 'b';

    errno_t ret = fopen_s(fr, name, open_param);
    if (ret == 0)
    {
        return FR_OK;
    }

    return FR_NO_FILE;
}

FRESULT f_read(FIL* f, void* b, UINT size, UINT* rred_o)			/* Read data from a file */
{
    size_t rred = fread(b, 1, size, *f);
    if (rred_o != NULL)
        *rred_o = rred;

    return FR_OK;
}

FRESULT f_lseek(FIL* f, DWORD pos)
{
    int ret = fseek(*f, pos, SEEK_SET);

    if (ret == 0)
        return FR_OK;

    return FR_INT_ERR;
}

FRESULT f_close(FIL* f)								/* Close an open file object */
{
    fclose(*f);
    return FR_OK;
}

FRESULT f_write(FIL* f, const void* buf, UINT size, UINT* wrote_o)	/* Write data to a file */
{
    size_t wrote = fwrite(buf, 1, size, *f);
    if (wrote_o != NULL)
        *wrote_o = wrote;

    if (wrote < size)
        return FR_DISK_ERR;

    return FR_OK;
}
