#include <exception>
#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <set>
#include <iconv.h>
#include <string>
#include <errno.h>

//const wchar_t* charset = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789!@#$%^&*()-_+=|\\[]{}:;\"'<,>.?/";

class MyException
{
    public:
        MyException(int ecode, const char* emsg)
        {
            code = ecode;
            if (emsg != NULL)
                msg = strdup(emsg);
            else
                msg = NULL;
        }

        MyException(const MyException& m):
            code(m.code)
        {
            if (m.msg != NULL)
            {
                msg = strdup(m.msg);
            }
            else
            {
                msg = NULL;
            }
        }

        ~MyException()
        {
            if (msg != NULL)
                free(msg);
        }

        int getCode() const { return code; }
        const char* getMessage() const { return msg; }

    private:
        int code;
        char* msg;
};


struct Glyph
{
    signed char x, y, ax;
    unsigned char width, height;
    void* bitmap;
    unsigned int glyphIndex;
    
    char code;

    unsigned char extra;
};

void doAll(const char* file, const char* charsetFile, const char* charsetFileEncoding, double size, const char* out)
{
    iconv_t icvt = iconv_open("CP1251", charsetFileEncoding);
    iconv_t unicode = iconv_open("wchar_t", charsetFileEncoding);
    FILE* ch = fopen(charsetFile, "rb");
    if (ch == NULL)
        throw MyException(errno, "Error opening charset");

    std::map<wchar_t, char> wcharset;

    while (!feof(ch))
    {
        char buf_in[256];
        char buf_out[1024];
        memset(buf_in, 0, 256);
        memset(buf_out, 0, 1024);

        size_t s = fread(buf_in, 1, 255, ch);
        printf("rred: %lu\n", s);
    
        char* in_pos = (char*)buf_in;
        char* out_pos = (char*)buf_out;
        size_t in = s;
        size_t out = 1023;
        iconv(icvt, &in_pos, &in, &out_pos, &out);

        wchar_t wbuf_out[1024];
        in = s;
        out = 1023*sizeof(wchar_t);
        in_pos = (char*)buf_in;
        out_pos = (char*)wbuf_out;
        memset(wbuf_out, 0, sizeof(wchar_t)*1024);
        iconv(unicode, &in_pos, &in, &out_pos, &out);
        size_t i = 0;
        while (wbuf_out[i] != 0 && buf_out[i] != 0)
        {
            wcharset.insert(std::make_pair(wbuf_out[i], buf_out[i]));

            printf("%04X -> %02X\n", wbuf_out[i], buf_out[i]);
            ++i;
        }
    }
    fclose(ch);

    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error != 0)
        throw MyException(error, "initializing ft library");
    
    FT_Face face;
    error = FT_New_Face(library, file, 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    { //... the font file could be opened and read, but it appears ... that its font format is unsupported 
        throw MyException(error, "Error opening font: wrong format");
    } 
    else if (error != 0)
    { 
        //... another error code means that the font file could not ... be opened or read, or simply that it is broken...
        throw MyException(error, "Error opening font");
    }

    std::map<wchar_t, char>::const_iterator it = wcharset.begin();
    FT_F26Dot6 ftFontSize = size * 64;
    FT_Set_Char_Size(face, ftFontSize, ftFontSize, 72, 72);
    int px_ascender = face->size->metrics.ascender;
    int px_descender = face->size->metrics.descender;

    unsigned long pxh = px_ascender - px_descender;

    printf("Ascender: %f, descender: %f, height: %f\n", px_ascender / 64.0, px_descender / 64.0, face->size->metrics.height/64.0);
    
    unsigned int fontHeight = face->size->metrics.height/64.0;

    std::map<wchar_t, Glyph> glyphs;

    while (it != wcharset.end())
    {
        wchar_t c = it->first;
        error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error != 0)
        {
            throw MyException(error, "Error opening glyph");
        }

        FT_Bitmap& bitmap = face->glyph->bitmap;

        printf("Symbol %x: %u x %u, pitch: %u\n", (unsigned int)c,  bitmap.width, bitmap.rows, bitmap.pitch);
        int top = face->glyph->bitmap_top;
        int left =  face->glyph->bitmap_left;
        printf("Symbol %x: top: %d, left %d\n", (unsigned int)c, top, left);

        size_t bmpByteWidth = (bitmap.width + 1) / 2;
        size_t bmpByteHeight = bitmap.rows;
        size_t bmpByteSize = bmpByteWidth * bmpByteHeight;
        printf("bmp: width: %lu, height: %u, size (bytes): %lu\n", bmpByteWidth, bmpByteHeight, bmpByteSize);
        glyphs[c].bitmap = malloc(bmpByteSize*2);
        memset(glyphs[c].bitmap, 0, bmpByteSize);

        int extra = (bmpByteWidth * 2) - bitmap.width;
        glyphs[c].x = face->glyph->bitmap_left - extra;
        glyphs[c].y = px_ascender/64 - top;
        printf("advance.x: %ld\n", face->glyph->advance.x / 64);
        glyphs[c].ax = face->glyph->advance.x / 64;

        glyphs[c].width = bitmap.width + extra;
        glyphs[c].height = bmpByteHeight;
        glyphs[c].glyphIndex = FT_Get_Char_Index(face, c);
        
        //size_t from = (px_ascender/64 - top) * bmpByteWidth;
        //printf("from: (%u - %u)*%lu = %lu\n", px_ascender / 64, top, bmpByteWidth, from);
        size_t from = 0;
        printf("extra: %d\n", extra);
        for (int y = 0; y < bitmap.rows; ++y)
        {
            for (int x = 0; x < bitmap.width; ++x)
            {
                unsigned char a0 = bitmap.buffer[x+y*bitmap.pitch];

                size_t pos1 = y*bmpByteWidth + (x + extra) / 2 + from;
                unsigned char halfbyte = (x + extra) % 2;

                unsigned char value = a0 / 16;

                value = value << (halfbyte * 4);

                unsigned char* ptr1 = (unsigned char*)(glyphs[c].bitmap) + pos1;
                *ptr1 = *ptr1 | value;

            }
        }

        unsigned char* ptr = (unsigned char*)glyphs[c].bitmap;
        for (int y = 0; y < glyphs[c].height; ++y)
        {
            for (int x = 0; x < (glyphs[c].width+1)/2; ++x)
            {
                unsigned char lo = *ptr & 0x0F;

                if (lo != 0)
                    printf("%02X", lo);
                else
                    printf("  ");

                unsigned char hi = (*ptr >> 4) & 0x0F;
                if (hi != 0)
                    printf("%02X", hi);
                else
                    printf("  ");

                ptr++;
            }
            printf("\n");
        }
        
        ++it;
    }

    // Kerning
    printf("iconv: %p\n", icvt);
    if (icvt == (iconv_t)-1)
    {
        throw MyException(1, "error initializing iconv");
    }

    std::map<unsigned short, signed char> kernmap;
    for (std::map<wchar_t, Glyph>::iterator it1 = glyphs.begin(); it1 != glyphs.end(); ++it1)
    {
        char c1 = wcharset[it1->first];
        it1->second.code = c1;
        for (std::map<wchar_t, Glyph>::iterator it2 = glyphs.begin(); it2 != glyphs.end(); ++it2)
        {
            char c2 = wcharset[it2->first];
            it2->second.code = c2;

            FT_Vector v;
            FT_Get_Kerning(face, it1->second.glyphIndex, it2->second.glyphIndex, FT_KERNING_DEFAULT, &v);
            
            if (v.x != 0)
            {
                signed char kx = v.x / 64;
                printf("Kerning for %04X and %04X: %d (%ld)\n", (unsigned int)it1->first, (unsigned int)it2->first, kx, v.x);

                unsigned short key = (unsigned char)c2;
                key = key << 8 | c1;
                printf("c1 = %02X, c2 = %02X, key = %04X\n", c1, c2, key);

                kernmap[key] = kx;
            }
        }
    }

    iconv_close(icvt);
    
    
    // writing file
    FILE* f = fopen(out, "wb");
    if (f == NULL)
        throw MyException(error, "opening output file");
    
    // header
    unsigned short charset_size = glyphs.size();
    fwrite(&charset_size, 2, 1, f);
    fwrite(&fontHeight, 1, 1, f);
    printf("Size: %u\n", charset_size);
    
    // glyphs
    for (std::map<wchar_t, Glyph>::iterator it1 = glyphs.begin(); it1 != glyphs.end(); ++it1)
    {
        Glyph& g = it1->second;
        printf("Write header: %02X, %02X, %02X, %02X, %02X\n", g.code, g.x, g.y, g.width, g.height);
        fwrite(&g.code, 1, 1, f);
        fwrite(&g.x, 1, 1, f);
        fwrite(&g.y, 1, 1, f);
        fwrite(&g.ax, 1, 1, f);
        fwrite(&g.width, 1, 1, f);
        fwrite(&g.height, 1, 1, f);
        
        size_t size = g.height;
        size *= g.width;
        size /= 2;
        fwrite(g.bitmap, size, 1, f);
    }
    
    // kerning
    unsigned short kernmap_size = kernmap.size();
    printf("Writing kerning of size %u\n", kernmap_size);
    fwrite(&kernmap_size, sizeof(unsigned short), 1, f);
    for (std::map<unsigned short, signed char>::const_iterator it = kernmap.begin();
         it != kernmap.end(); ++it)
    {
        printf("Kerning: %04X -> %d\n", it->first, it->second);
        fwrite(&it->first, sizeof(unsigned short), 1, f);
        fwrite(&it->second, sizeof(signed char), 1, f);
    }
    
    fclose(f);
    FT_Done_Face(face);

    error = FT_Done_FreeType(library);
    if (error != 0)
        throw MyException(error, "deinitializing ft library");
}



int main(int argc, char ** argv)
{
    if (argc < 6)
    {
        fprintf(stderr, "format: %s FONTFILE CHARSETFILE CHARSETFILEENCODING FONTSIZE BINFILE_OUT\n\n", argv[0]);

        fprintf(stderr, "\tFONTFILE    - font file, TTF/OTF\n");
        fprintf(stderr, "\tCHARSETFILE - file, containing all symbols to load from file\n");
        return 1;
    }

    int n = 1;

    const char* file = argv[n++];
    const char* charsetFile = argv[n++];
    const char* charsetFileEncoding = argv[n++];
    char* err = NULL;
    double size = strtof(argv[n++], &err);
    if (*err != 0)
        size = 0;
    const char* out = argv[n++];

    if (size < 0.1)
    {
        fprintf(stderr, "Error: font size is too small\n\n");
        return 1;
    }

    try
    {
//void doAll(const char* file, const char* charsetFile, const char* charsetFileEncoding, double size, const char* out)
        doAll(file, charsetFile, charsetFileEncoding, size, out);
    }
    catch (MyException& ex)
    {
        fprintf(stderr, "Error: 0x%02X (%s)\n", ex.getCode(), ex.getMessage());
        
    }
    catch (...)
    {
        fprintf(stderr, "Unknown error");
        return 1;
    }

    return 0;
}
