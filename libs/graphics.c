// libs/graphics.c - Implementação real para o modo 320x200

#include <libs/graphics.h>

// O modo 320x200x256 usa um buffer de vídeo linear no endereço 0xA0000
unsigned char* video_memory = (unsigned char*)0xA0000;
int screen_width = 320;
int screen_height = 200;

void put_pixel(int x, int y, int color) {
    // Checagem de limites para não escrever fora da tela
    if (x < 0 || x >= screen_width || y < 0 || y >= screen_height) {
        return;
    }
    int offset = y * screen_width + x;
    video_memory[offset] = color;
}

// Função para desenhar um retângulo preenchido (útil para o cursor do mouse)
void fill_rect(int x, int y, int width, int height, int color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}