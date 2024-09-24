# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/James/esp/v5.3/esp-idf/components/bootloader/subproject"
  "D:/MIC_TO_EAR/build/bootloader"
  "D:/MIC_TO_EAR/build/bootloader-prefix"
  "D:/MIC_TO_EAR/build/bootloader-prefix/tmp"
  "D:/MIC_TO_EAR/build/bootloader-prefix/src/bootloader-stamp"
  "D:/MIC_TO_EAR/build/bootloader-prefix/src"
  "D:/MIC_TO_EAR/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/MIC_TO_EAR/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/MIC_TO_EAR/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
