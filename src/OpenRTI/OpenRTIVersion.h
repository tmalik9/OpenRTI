
#pragma once

// Majornumber of OPENRTI library
#define OPENRTI_MAJOR_NUMBER              2
#define OPENRTI_MAJOR_NUMBER_STRING      "2"

// Minornumber of OPENRTI library
#define OPENRTI_MINOR_NUMBER              0
#define OPENRTI_MINOR_NUMBER_STRING      "0"

// Buildnumber of OPENRTI library
#define OPENRTI_BUILD_NUMBER              0
#define OPENRTI_BUILD_NUMBER_STRING      "0"

// Patchnumber of OPENRTI library
#define OPENRTI_PATCH_NUMBER              3
#define OPENRTI_PATCH_NUMBER_STRING      "3"

#define YEAR_STRING                     "2021"

// Product version
#define OPENRTI_PRODUCT_VERSION          OPENRTI_MAJOR_NUMBER,OPENRTI_MINOR_NUMBER,0,0
#define OPENRTI_PRODUCT_VERSION_STRING   OPENRTI_MAJOR_NUMBER_STRING "." OPENRTI_MINOR_NUMBER_STRING "\000"

#if ( OPENRTI_PATCH_NUMBER )
#define OPENRTI_FILE_VERSION             OPENRTI_MAJOR_NUMBER,OPENRTI_MINOR_NUMBER,OPENRTI_BUILD_NUMBER,OPENRTI_PATCH_NUMBER
#define OPENRTI_FILE_VERSION_STRING      OPENRTI_MAJOR_NUMBER_STRING "." OPENRTI_MINOR_NUMBER_STRING "." OPENRTI_BUILD_NUMBER_STRING "." OPENRTI_PATCH_NUMBER_STRING "\000"
#else
#define OPENRTI_FILE_VERSION             OPENRTI_MAJOR_NUMBER,OPENRTI_MINOR_NUMBER,OPENRTI_BUILD_NUMBER
#define OPENRTI_FILE_VERSION_STRING      OPENRTI_MAJOR_NUMBER_STRING "." OPENRTI_MINOR_NUMBER_STRING "." OPENRTI_BUILD_NUMBER_STRING "\000"
#endif

// Company & copyright information
#define AUTHOR_NAMES                     "Mathias Froehlich, with additions from Vector Informatik GmbH"
#define OPENRTI_PRODUCT_NAME             "OpenRTI"
#define OPENRTI_COPYRIGHT_STRING         "\251 Copyright 1992-" YEAR_STRING " " AUTHOR_NAMES
