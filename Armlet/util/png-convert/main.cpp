#include <stdio.h>
#include <png.h>
#include <stdint.h>

int read_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk)
{
    return 0;
}


bool doAll(const char* png, const char* bmp, bool bw)
{
    FILE* in = fopen(png, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", png);
        return false;
    }
    unsigned char h[8];
    fread(h, 1, 8, in);
    bool isPng = !png_sig_cmp(h, 0, 8);
    if (!isPng)
    {
        fprintf(stderr, "File %s dosn't seem to be PNG\n", png);
        return false;
    }
    fseek(in, 0, SEEK_SET);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fprintf(stderr, "png_create_read_struct error\n");
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,
                (png_infopp)NULL, (png_infopp)NULL);
        fprintf(stderr, "png_create_info_struct error\n");
        return false;
    }

    png_init_io(png_ptr, in);

//    void* user_chunk_ptr = NULL;
//    png_set_read_user_chunk_fn(png_ptr, user_chunk_ptr, read_chunk_callback);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_GRAY_TO_RGB|PNG_TRANSFORM_SWAP_ALPHA, NULL);

    size_t png_w = png_get_image_width(png_ptr, info_ptr);
    size_t png_h = png_get_image_height(png_ptr, info_ptr);
    printf("width: %lu, height: %lu\n", png_w, png_h);
    if (png_w > 0x00FF || png_h > 0x00FF)
    {
        fprintf(stderr, "Image too large (%lux%lu)\n", png_w, png_h);
        return false;
    }
    size_t bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    size_t channels = png_get_channels(png_ptr, info_ptr);
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    printf("bit_depth: %lu\n", bit_depth);
    printf("channels: %lu\n", channels);
    printf("rowbytes: %lu\n", rowbytes);

    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);

    FILE* out = fopen(bmp, "wb");
    char H;
    if (bw)
    {
        H = 'B';
    }
    else
    {
        H = 'C';
    }
    fwrite(&H, 1, 1, out);
    fwrite(&png_w, 1, 1, out);
    fwrite(&png_h, 1, 1, out);

    for (size_t y = 0; y < png_h; ++y)
    {
        png_bytep row = row_pointers[y];
        uint32_t* p = (uint32_t*)row;
        for (size_t x = 0; x < png_w; ++x, ++p)
        {
            printf("%08X ", *p);
            uint8_t a = ((*p & 0x000000FF) >>  0);
            uint8_t r = ((*p & 0x0000FF00) >>  8);
            uint8_t g = ((*p & 0x00FF0000) >> 16);
            uint8_t b = ((*p & 0xFF000000) >> 24);
            if (bw)
            {
                uint8_t l = 0.299 * r + 0.587 * g + 0.114 * b;
                uint8_t c = ((a / 17) << 4) |
                            (l / 17);
                fwrite(&c, 1, 1, out);
            }
            else
            {
                uint16_t c = (((uint16_t)a / 17) << 12) |
                             (((uint16_t)r / 17) << 8) |
                             (((uint16_t)g / 17) << 4) |
                             (((uint16_t)b / 17) << 0);
                fwrite(&c, 2, 1, out);
            }
        }
        printf("\n");
    }
    fclose(out);
    fclose(in);
    return true;
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s IN_FILE.PNG OUT_FILE [BW|COLOR]\n", argv[0]);
        return 1;
    }
    bool bw_mode = false;
    if (argc > 3 && strncmp(argv[3], "BW", 3) == 0)
    {
        bw_mode = true;
    }

    bool r = doAll(argv[1], argv[2], bw_mode);
    if (r)
        return 0;
    else
        return 1;
}
