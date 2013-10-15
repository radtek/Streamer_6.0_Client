#ifndef OSNVERSION_H_
#define OSNVERSION_H_


#define VER_COMPANYNAME_STR         "EIM Inc"

#define VER_PRODUCTNAME_STR         "EIM OsnSolution"

#define VER_LEGALCOPYRIGHT_YEARS    "2007"

#define VER_LEGALCOPYRIGHT_STR 	"Copyright (c) EIM Corporation. All rights reserved."


#define VER_LEGALTRADEMARKS_STR    "OsnSolution (R) is a registered trademark of EIM Corporation."


#define VER_PRODUCTBUILD	    30

#define VER_PRODUCTBUILD_QFE 		6008

#define VER_PRODUCTMAJORVERSION     5

#define VER_PRODUCTMINORVERSION    0

#ifdef __BUILDMACHINE__
#undef __BUILDMACHINE__
#endif

#define __BUILDMACHINE__ BuildMachine

#define __BUILDDATE__   01/09/2013

#endif
