#ifndef _COMMON_H
#define _COMMON_H

#undef ERROR
#include "config_for_unittests.h"
#include "utilities.h"

#include <fcntl.h>
#ifdef HAVE_GLOB_H
# include <glob.h>
#endif
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "stdio.h"
#include <comdef.h>
#include <Wbemidl.h>
#include <vector>
#include <stdlib.h>
#include <tchar.h>


#include <iomanip>

#include "config.h"


#include "base\commandlineflags.h"
#include "glog\logging.h"
#include "glog\raw_logging.h"

# pragma comment(lib, "wbemuuid.lib")

#ifdef HAVE_LIB_GFLAGS
#include <gflags/gflags.h>
#endif


using namespace std;
using namespace GOOGLE_NAMESPACE;       //±ØÐë°üº¬


#endif _COMMON_H