# Snowcookie
A 3D Game Engine

## Build options
Place double quotation marks ("") around filenames in build option commands.
### Post build options
- [ ] Copy sce_corelib.dll post build to sce_sandbox output folder
            
      IF EXIST "$(TargetDir)$(ProjectName).dll" (xcopy /Q /E /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\sce_sandbox" > nul) ELSE (xcopy /Q /Y /I "$(TargetDir)$(ProjectName).dll" "$(SolutionDir)bin\Debug-windows-x86_64\sce_sandbox" > nul)
