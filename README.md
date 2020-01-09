# Bl2ModelConverter [![AppVeyor build status](https://ci.appveyor.com/api/projects/status/github/AdventureT/bl2modelconverter?branch=master&svg=true)](https://ci.appveyor.com/project/krlmlr/r-appveyor/branch/master) 
This Program extracts trb files from the Prime engine.


## How to use? :file_folder:
Clone/Download the repo to your desktop. Open the solution with **Visual Studio 2019** and run it.

## Supported Tags/Labels
| Tag/Label | Stands for    | Can :heavy_check_mark:                               | Can't :x:
|-----------|---------------|------------------------------------------------------|--------------------------------------------------------
| PMDL      | Model File    | Extract vertices, faces, normals, uvs from this tag  | Extract some other model data which is unknown for now.
| PTEX      | Texture File  | Extract dds file                                     | There are some unknown parts in this tag.
| PCOL      | Collsion File | Extract Collsion Model file and a Havok 2010.1 file  | There are some unknown parts in this tag.
| tskl      | Skeleton File | Extract nothing for now :grimacing: :soon:           | There are many unknown parts in this tag.
