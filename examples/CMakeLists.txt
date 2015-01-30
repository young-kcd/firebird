include(SourceGroups)

###############################################################################
#
# examples
#
###############################################################################

########################################
# EXECUTABLE empbuild
########################################

if (NOT CMAKE_CROSSCOMPILING)

add_custom_command(
    OUTPUT empbuild.fdb
    DEPENDS
        isql
        ${CMAKE_CURRENT_SOURCE_DIR}/empbuild/empbld.sql
    COMMAND ${CMAKE_COMMAND} -E remove empbuild.fdb
    COMMAND isql -q -i ${CMAKE_CURRENT_SOURCE_DIR}/empbuild/empbld.sql
)
add_custom_command(
    OUTPUT empbuild.c
    DEPENDS
        gpre
        messages
        ${CMAKE_CURRENT_SOURCE_DIR}/empbuild/empbuild.e
        empbuild.fdb
    COMMENT "Calling GPRE for empbuild.e"
    COMMAND gpre -r -m -n -z ${CMAKE_CURRENT_SOURCE_DIR}/empbuild/empbuild.e empbuild.c
)

add_executable          (empbuild empbuild.c empbuild.fdb ${CMAKE_CURRENT_SOURCE_DIR}/empbuild/empbuild.e)
target_link_libraries   (empbuild yvalve)
set_output_directory    (empbuild . FORCE)
add_dependencies_cc     (empbuild gfix engine12 fbtrace)
project_group           (empbuild Examples)

file(GLOB files
    "${CMAKE_CURRENT_SOURCE_DIR}/empbuild/*.sql"
    "${CMAKE_CURRENT_SOURCE_DIR}/empbuild/*.inp"
)
foreach(F ${files})
    get_filename_component(name ${F} NAME)
    add_custom_command(TARGET empbuild POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${F} ${name})
endforeach()

set(EMPBUILD_CMD empbuild employe2.fdb)
set_win32(EMPBUILD_CMD set PATH=%PATH%\\\\;${output_dir} COMMAND ${EMPBUILD_CMD})
set_unix (EMPBUILD_CMD PATH=${output_dir}/bin ${EMPBUILD_CMD})
set_apple(EMPBUILD_CMD DYLD_LIBRARY_PATH=${output_dir}/plugins:${output_dir}/lib ${EMPBUILD_CMD})

add_custom_command(
    OUTPUT employe2.fdb
    DEPENDS
        empbuild
        ${files}
    COMMAND ${CMAKE_COMMAND} -E remove employe2.fdb
    COMMAND ${EMPBUILD_CMD}
)
add_custom_target       (employee_db DEPENDS employe2.fdb SOURCES ${files})
project_group           (employee_db Examples)

endif() # if (NOT CMAKE_CROSSCOMPILING)

########################################
# SHARED LIBRARY udrcpp_example
########################################

file(GLOB udrcpp_example_src "udr/*")

add_library             (udrcpp_example SHARED ${udrcpp_example_src})
target_link_libraries   (udrcpp_example yvalve)
set_output_directory    (udrcpp_example plugins/udr)
project_group           (udrcpp_example Examples)


########################################
# SHARED LIBRARY dbcrypt_example
########################################

add_library             (dbcrypt_example SHARED dbcrypt/DbCrypt.cpp)
set_target_properties   (dbcrypt_example PROPERTIES OUTPUT_NAME DbCrypt_example)
set_output_directory    (dbcrypt_example plugins)
project_group           (dbcrypt_example Examples)


########################################
# SHARED LIBRARY cryptkeyholder_example
########################################

add_library             (cryptkeyholder_example SHARED dbcrypt/CryptKeyHolder.cpp)
set_target_properties   (cryptkeyholder_example PROPERTIES OUTPUT_NAME CryptKeyHolder_example)
set_output_directory    (cryptkeyholder_example plugins)
project_group           (cryptkeyholder_example Examples)

###############################################################################
