// generate an image of the world
#include "../include/generator.h"
#include "../include/util.h"
#include "../include/finders.h"
#include <stdio.h>

int64_t S64(const char *s) {
  int64_t i;
  char c ;
  int scanned = sscanf(s, "%" SCNd64 "%c", &i, &c);
  if (scanned == 1) return i;
  if (scanned > 1) {
    // TBD about extra data found
    return i;
    }
  // TBD failed to scan;
  return 0;
}

int main(int argc, char **argv)
{
    unsigned char biomeColors[256][3];

    // Initialize a color map for biomes.
    initBiomeColors(biomeColors);

    // Initialize a stack of biome layers.
    LayerStack g;
    setupGenerator(&g, MC_1_17);
    // Extract the desired layer.
    Layer *layer = &g.layers[L_SHORE_16];

    uint64_t seed = S64(argv[1]);
    applySeed(&g, seed);
    Pos worldSpawn = getSpawn(MC_1_17, &g, NULL, seed);
    int areaX = (worldSpawn.x / 16) - 64, areaZ = (worldSpawn.z / 16) - 64;
    unsigned int areaWidth = 128, areaHeight = 128;
    unsigned int scale = 1;
    unsigned int imgWidth = areaWidth*scale, imgHeight = areaHeight*scale;

    // Allocate a sufficient buffer for the biomes and for the image pixels.
    int *biomeIds = allocCache(layer, areaWidth, areaHeight);
    unsigned char *rgb = (unsigned char *) malloc(3*imgWidth*imgHeight);

    // Apply the seed only for the required layers and generate the area.

    genArea(layer, biomeIds, areaX, areaZ, areaWidth, areaHeight);

    // Map the biomes to a color buffer and save to an image.
    biomesToImage(rgb, biomeColors, biomeIds, areaWidth, areaHeight, scale, 2);

    //for(int i=0; i<imgWidth*imgHeight; i++) {
    //  printf("%u %u %u\n", rgb[i], rgb[i+imgWidth*imgHeight], rgb[i+imgWidth*imgHeight*2]);
    //}
    int pixelRed = 2;

    rgb[pixelRed] = 255;
    rgb[pixelRed+imgWidth*imgHeight] = 0;
    rgb[pixelRed+imgWidth*imgHeight*2] = 0;

    savePPM("biomes_at_layer.ppm", rgb, imgWidth, imgHeight);

    // Clean up.
    free(biomeIds);
    free(rgb);

    return 0;
}
