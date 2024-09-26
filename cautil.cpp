#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
//#include "G2DELAY.h"
#include "G2INIT.h"
#include "version.h"

typedef struct
{
  FILE *IN, *OUT;
  uint8_t PRN;
  uint16_t phi;
  SWV V;
} CONFIG;

void initconfig(CONFIG *cfg)
{
  cfg->PRN = 135;
  cfg->phi = 0;
}

void processArgs(int argc, char *argv[], CONFIG *cfg)
{
  static int len, i, ch = ' ';
  static char usage[] =
      "cautil: CA Code Utility\n"
      "usage: cautil <PRN> [options]\n"
      "       PRN             which PRN\n"
      "       -v              print version information\n"
      "       -?|h            print this usage message\n"
      "  defaults:\n";

  if (argc < 2)
  {
    fprintf(stdout, "%s: GitCI:%s %s v%.1d.%.1d.%.1d\n",
            cfg->V.Name, cfg->V.GitCI, cfg->V.BuildDate,
            cfg->V.Major, cfg->V.Minor, cfg->V.Patch);
    printf("%s", usage);
    exit(1);
  }
  else {
    cfg->PRN = strtol(argv[1], NULL, 10);
  }
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      ch = argv[i][1];
      switch (ch)
      {
      case 'v':
        fprintf(stdout, "%s: GitCI:%s %s v%.1d.%.1d.%.1d\n",
                cfg->V.Name, cfg->V.GitCI, cfg->V.BuildDate,
                cfg->V.Major, cfg->V.Minor, cfg->V.Patch);
        exit(0);
        break;
      default:
        break;
      }
    }
  }
};

int main(int argc, char *argv[])
{
  CONFIG cnfg;
  uint16_t ph;

cnfg.V.Major = MAJOR_VERSION;
cnfg.V.Minor = MINOR_VERSION;
cnfg.V.Patch = PATCH_VERSION;
#ifdef CURRENT_HASH
  strncpy(cnfg.V.GitCI, CURRENT_HASH, 40);
#endif
#ifdef CURRENT_DATE
  strncpy(cnfg.V.BuildDate, CURRENT_DATE, 17);
#endif
#ifdef CURRENT_NAME
  strncpy(cnfg.V.Name, CURRENT_NAME, 10);
#endif

  initconfig(&cnfg);
  processArgs(argc, argv, &cnfg);
  //G2DELAY sv(cnfg.PRN, cnfg.phi);
  G2INIT sv(cnfg.PRN, cnfg.phi);

  for (ph=0; ph < 1023; ph++) {
    printf("%.1X", sv.CACODE[ph]);
  } 
  printf("\n");
  for (ph=0; ph < 1023; ph++) {
    printf("%.4X ", sv.G1a[ph]);
  }
  printf("\n");
  for (ph=0; ph < 1023; ph++) {
    printf("%.4X ", sv.G2a[ph]);
  }
  printf("\n");
  printf("First 10: %.4oo\n", sv.F10);   // First 10 bits of CA Code
  return 0;
}
