# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/James/esp/v5.3/esp-idf/components/bootloader/subproject"
  "F:/MIC_TO_EAR_V1/build/bootloader"
  "F:/MIC_TO_EAR_V1/build/bootloader-prefix"
  "F:/MIC_TO_EAR_V1/build/bootloader-prefix/tmp"
  "F:/MIC_TO_EAR_V1/build/bootloader-prefix/src/bootloader-stamp"
  "F:/MIC_TO_EAR_V1/build/bootloader-prefix/src"
  "F:/MIC_TO_EAR_V1/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/MIC_TO_EAR_V1/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/MIC_TO_EAR_V1/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
