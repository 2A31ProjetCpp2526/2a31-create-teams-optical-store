# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Gestion_de_Fournisseur_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Gestion_de_Fournisseur_autogen.dir\\ParseCache.txt"
  "Gestion_de_Fournisseur_autogen"
  )
endif()
