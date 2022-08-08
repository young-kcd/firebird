@echo off
docker run --rm -v %cd%\..\..\..:C:\firebird -v %cd%\..\..\..\output:C:\firebird-out -v %cd%\..\..\..\builds\install_images:C:\firebird\builds\install_images asfernandes/firebird-builder:5 %1
