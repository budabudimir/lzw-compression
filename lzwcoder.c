#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 256
#define MAX_SIZE 65536
#define MOD 100003
#define P   337

int last[MOD];
int next[MAX_SIZE];
int indx[MAX_SIZE];
int lenght[MAX_SIZE];
char *words[MAX_SIZE];
int pos = 0;

int hash(char *wrd, int len) {
   int i, h = P;
   for (i = 0; i < len; ++i) {
      h = (P * h + wrd[i]) % MOD;
   }
   return h % MOD;
}

int get(char *wrd, int len) {
   int i;
   int h = hash(wrd, len);

   for (i = last[h]; i != -1; i = next[i]) {
      if (len == lenght[i] && !memcmp(words[i], wrd, len)) {
         return indx[i];
      }
   }

   return -1;
}

void add(char *wrd, int len) {
   int h = hash(wrd, len);
   next[pos] = last[h];
   indx[pos] = pos;
   lenght[pos] = len;
   words[pos] = calloc(len, 1);
   memcpy(words[pos], wrd, len);
   last[h] = pos++;
}

void write(unsigned short x, FILE *fout) {
   fwrite(&x, 2, 1, fout);
}

void generate(FILE *fin, FILE *fout) {
   int i = 0;
   char wrd[MAX_SIZE];
   memset(wrd, 0, sizeof wrd);

   for (i = 0; i < ALPHA; ++i) {
      char c[2] = { i, 0 };
      add(c, 1);
   }

   char c = 0; i = 0;
   while (fread(&c, 1, 1, fin)) {
      wrd[i++] = c;
      int x = get(wrd, i);

      if (x == -1) {
         if (pos < MAX_SIZE) {
            add(wrd, i);
         }

         x = get(wrd, i - 1);
         write(x, fout); 

         wrd[(i = 1) - 1] = c;
      }
   }

   unsigned int x = get(wrd, i);
   write(x, fout);
}

int main(int argc, char *argv[]) {
   memset(next, -1, sizeof next);
   memset(last, -1, sizeof last);

   FILE *in = fopen(argv[1], "rb");
   FILE *out = fopen(argv[2], "wb");
   
   generate(in, out);

   fclose(in);
   fclose(out);

   return 0;
}

