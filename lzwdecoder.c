#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 65536
#define MAX SIZE
#define ALPHA 256

char trans[MAX];
int prev[MAX], first[MAX], pos = 0;

FILE *fin, *fout;

void output(int x) {
   if (x >= 0) {
      output(prev[x]);
      fwrite(&trans[x], 1, 1, fout);
   }
}

int main(int argc, char *argv[]) {
   memset(prev, -1, sizeof prev);

   for (pos = 0; pos < ALPHA; ++pos) {
      trans[pos] = pos;
      first[pos] = pos;
   }

   fin = fopen(argv[1], "rb");
   fout = fopen(argv[2], "wb");
   
   int old = SIZE, new;
   char c;

   while (fread(&new, 2, 1, fin)) {
      if (new >= pos) {
         output(old);
         fwrite(&c, 1, 1, fout);
         c = trans[first[old]];
      } else {
         output(new);
         c = trans[first[new]];
      }

      if (old < SIZE && pos < SIZE) {
         first[pos] = first[old];
         prev[pos] = old;
         trans[pos++] = c;
      }

      old = new;
   }

   fclose(fin);
   fclose(fout);

   return 0;
}

