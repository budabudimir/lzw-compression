#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 256
#define MAX_SIZE 65536
#define MOD 100003
#define P   337

#define NOT_IN_TABLE -1

int last[MOD];
int next[MAX_SIZE];
int indx[MAX_SIZE];
int lenght[MAX_SIZE];
char *words[MAX_SIZE];
int pos = 0;

int hash(char *word, int len) {
   int i, h = P;
   for (i = 0; i < len; ++i) {
      h = (P * h + word[i]) % MOD;
   }
   return h % MOD;
}

int get(char *word, int len) {
   int i;
   int h = hash(word, len);

   for (i = last[h]; i != -1; i = next[i]) {
      if (len == lenght[i] && !memcmp(words[i], word, len)) {
         return indx[i];
      }
   }

   return NOT_IN_TABLE;
}

void add(char *word, int len) {
   int h = hash(word, len);
   next[pos] = last[h];
   indx[pos] = pos;
   lenght[pos] = len;
   words[pos] = calloc(len, 1);
   memcpy(words[pos], word, len);
   last[h] = pos++;
}

void write(unsigned short x, FILE *fout) {
   fwrite(&x, 2, 1, fout);
}

void generate(FILE *fin, FILE *fout) {
   int i = 0;
   char word[MAX_SIZE];
   memset(word, 0, sizeof word);

   for (i = 0; i < ALPHA; ++i) {
      char c[2] = { i, 0 };
      add(c, 1);
   }

   char c = 0; i = 0;
   while (fread(&c, 1, 1, fin)) {
      word[i++] = c;
      int x = get(word, i);

      if (x == NOT_IN_TABLE) {
         if (pos < MAX_SIZE) {
            add(word, i);
         }

         x = get(word, i - 1);
         write(x, fout); 

         word[(i = 1) - 1] = c;
      }
   }

   unsigned int x = get(word, i);
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

