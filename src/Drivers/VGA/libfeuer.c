// LIB LITHIUM CXX BUT IN C

void vga_init() {
    set_vga_mode_13h();
}

//void vga_drawline_vert(int x, int y, char pixel) {
//    for (int i = y; i < y + length; ++i) {
//        if (i >= 0 && i < width*height) {
//            vga_setpixel(x, i, pixel);
//        }
//    }
//}

void vga_drawline_hori(int x, int y, char pixel) {
    int length = x - y;
    for (int i = x; i < x + length; ++i) {
        draw_pixel(i, y, pixel);
    }
}

//void vga_drawrect(int x, int y, int width, int height, char pixel) {
//}
