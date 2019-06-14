#include <stdio.h>
#include "bitmap.h"

int
main ()
{
    Bitmap *bmp;
    
    bmp = create_bitmap(640, 480);
    if(bmp == NULL)
        return 1;
    write_bitmap(bmp, "test.bmp");
    destroy_bitmap(bmp);
    remove("test.bmp");
    return 0;
}
