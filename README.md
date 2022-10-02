# Snowcookie
---
--- 
## A 3D Game Engine
---
--- 
### Contents
1. Project setup
2. Platform
--- 
--- 
## **1. Project setup**
### **Overview:**
---
Project created through [Premake 5.0](https://premake.github.io/download) via a `lua script` to set properties and a `batch` file to generate the project. Premake 5.0 is limited to Visual Studio on Windows and CodeLite on Linux.
### **Microsoft Windows**
#### **Microsoft Visual Stuido 2022:**
Due to the import/export defines (`SCE_EXPORT/SCE_IMPORT`) being set on projects to indicate whether they are a DLL or a EXE, some files must be created on initial start up to get the project compiling. The files are as follows:

### Inital  files:
1. Sandbox - main.cpp
2. Engine - sce_pch.h
3. Engine - sce_pch.cpp
4. Engine - defines.h
5. Engine - include.h
6. Engine - application.h
7. Engine - application.cpp

In Engine `#pragma once` needs adding to all header files (.h), in defines use the `SCE_EXPORT/SCE_IMPORT` defines to set the DLL-linkage and in application simple class that uses the DLL-linkage API define. Also each `source file` in the Engine project will require the precompiled header file to be `included` at the start.

##### **Errors/Bugs Microsoft Visual Stuido 2022:**
*(Due to a bug in Premake 5.0 the `post build event` requires directories encapsulating in double quotes ("") and the trailing forward slash (/) removing from the end of the first directory in Visual Studio on Windows, it should look something like the following.)*

##### *```IF EXIST "$(TargetDir)$(ProjectName).dll" (xcopy /Q /E /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\Sandbox" > nul) ELSE (xcopy /Q /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\Sandbox" > nul)```*

#### **Entry point**
In `Sandbox main.cpp` create a `main` function and include `sce_engine.h`. Then derive a `Sandbox Application class` from the base class `Application class`. Write in the `CreateApplication function` which links a pointer back to the `main function` inside the Engine DLL.
Then in `Engine entry-point.h` create an `extern Application* CreateAppliction function decleration` which can be called in `main function`.

---
---  
## 2. Platform
### **Overview:**
---

Encapsulates the Kernal specific calling on the specified current platform the engine is running on. This abstracts common shared tasks such as memory allocation, printing to console and creating a window/surface, etc, etc and so on.
### **New and Delete override(s):**
---
The engine specific main `new` is overriding to include a `MemoryTag` identifier, `void* operator new(size_t nSize, MemoryTag tag)`. This is for specifying the memory type. The remaining `new` and `delete` functions are platform specific overrides and are require to be placed in the correct preprocessor section with the correct header includes.
### **enum NodeStatus:**
---
Enumerated type for setting if a node is free to be used or occupied and in currently in use.
### **struct MemoryNode:**
---
MemoryNode is a type that contains all the information we need to stroe and locate a memory node.
### **class MemoryNodeList:**
---
Manages the `MemoryNodeList`'s and keeps track of them. Essentially just a linked list which the `MemoryNodeList` iterates through to store, use, free and delete both data and nodes. This class is not thread safe and is not designed to be used independently, and is designed to be the base class for `PlatformAllocator`.
### **class PlatformAllocator:**
---
This class is in charge of managing Memory allocation on the `Kernel`. It is designed to be used in conjunction with the `MemoryNodeList` and cannot be used correctly indepdnetly of the `MemoryNodeList` class. This class is thread safe and uses platform specific calls for allocation and deallocation.
### **class class Platform:**
---







