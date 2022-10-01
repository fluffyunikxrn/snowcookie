# Snowcookie

## A 3D Game Engine
---
--- 
### Contents
1. Project setup
2. Memory Manager
3. Logger
--- 
--- 
## 1. Project setup

Project created through [Premake 5.0](https://premake.github.io/download) via a `lua script` to set properties and a `batch` file to generate the project. Premake 5.0 is limited to Visual Studio on Windows and CodeLite on Linux.

*(Due to a bug in Premake 5.0 the `post build event` requires directories encapsulating in double quotes ("") and the trailing forward slash (/) removing from the end of the first directory in Visual Studio on Windows, it should look something like the following.)*

*```IF EXIST "$(TargetDir)$(ProjectName).dll" (xcopy /Q /E /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\Sandbox" > nul) ELSE (xcopy /Q /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\Sandbox" > nul)```*

Due to the import/export defines (`SCE_EXPORT/SCE_IMPORT`) being set on projects to indicate whether they are a DLL or a EXE, some files must be created on initial start up to get the project compiling. The files are as follows:

1. Sandbox - main.cpp
2. Engine - sce_pch.h
3. Engine - sce_pch.cpp
4. Engine - defines.h
5. Engine - include.h
6. Engine - application.h
7. Engine - application.cpp

In Engine `#pragma once` needs adding to all header files (.h), in defines use the `SCE_EXPORT/SCE_IMPORT` defines to set the DLL-linkage and in application simple class that uses the DLL-linkage API define.

In Sandbox create a `main` function and include `include.h` and create/destroy a `applciation` object in the main function, to ensure everything is working. The result should be a comnpiled Sandbox EXE file which calls the `application` object from the DLL and successfully runs.

--- 
## 2. Memory Manager


