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
  uint8_t PRN;  //
  uint16_t phi; // Code Phase
  SWV V;
  bool showCode;
} CONFIG;

void initconfig(CONFIG *cfg)
{
  cfg->PRN = 135;
  cfg->phi = 0;
  cfg->showCode = false;
}

void processArgs(int argc, char *argv[], CONFIG *cfg)
{
  static int len, i, ch = ' ';
  static char usage[] =
      "cautil: CA Code Utility\n"
      "usage: cautil <PRN> [phase] [options]\n"
      "       PRN             which PRN\n"
      "       phase           code phase\n"
      "       -s              show the CA Code\n"
      "       -v              print version information\n"
      "       -?|h            print this usage message\n"
      "  defaults: PRN=135 phase=0\n";

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
      case 's':
        cfg->showCode = true;
      break;
      case 'v':
        fprintf(stdout, "%s: GitCI:%s %s v%.1d.%.1d.%.1d\n",
                cfg->V.Name, cfg->V.GitCI, cfg->V.BuildDate,
                cfg->V.Major, cfg->V.Minor, cfg->V.Patch);
        exit(0);
        break;
      case '?':
        fprintf(stdout, "%s", usage);
        exit(0);
      break;
      case 'h':
        fprintf(stdout, "%s", usage);
        exit(0);
      break;
      default:
        fprintf(stdout, "%s", usage);
        exit(0);
      break;
      }
    }
    else {
      if (i == 2) {
        cfg->phi = strtol(argv[2], NULL, 10) % 1023;
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
  G2INIT sv(cnfg.PRN, 0); // Don't shift it yet
  //G2INIT sv(cnfg.PRN, cnfg.phi);

  if (cnfg.showCode == true) {
  for (ph=0; ph < 1023; ph++) 
    fprintf(stdout, "%.1X", sv.CACODE[ph]);
  fprintf(stdout, "\n");
  }

  fprintf(stdout, "G1[%.4d]:o%.4o G2[%.4d]:o%.4o G1:0x%.3X G2:0x%.3X CA:%.1X\n",
   cnfg.phi, sv.G1a[cnfg.phi], cnfg.phi, sv.G2a[cnfg.phi],
   sv.G1a[cnfg.phi], sv.G2a[cnfg.phi], sv.CACODE[cnfg.phi]);
  //printf("First 10: %.4oo\n", sv.F10);   // First 10 bits of CA Code
  return 0;
}
