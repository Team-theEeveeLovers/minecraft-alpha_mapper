# Minecraft Alpha Mapper
### A WIP program to map Alpha Minecraft worlds

This is a ***VERY*** incomplete program for mapping Minecraft worlds saved in versions using the Alpha Level Format. It was used starting in Infdev 20100327 except in Infdev 20100624, which used a different save format immediately scrapped afterward due to immensely increased filesize. It stopped being used with the release of Beta 1.3, meaning the last version to use this format was Beta 1.2_02.

## Current progress:
* Level Info Loading and Displaying:  ``VERY INCOMPLETE``
* Chunk Loading and Displaying:  ``SOMEWHAT INCOMPLETE``


## BUILDING

To build this program, use CMake. First, configure the CMake cache. Then, build the external libraries like SDL2 and zlib. Finally, build Minecraft Alpha Mapper.



## Useful Resources
### These resources helped in the development of this program
* [Java Edition Alpha Level Format - Minecraft Wiki](https://minecraft.wiki/w/Java_Edition_Alpha_level_format)
* [Official NBT Specification](https://web.archive.org/web/20110723210920/http://www.minecraft.net/docs/NBT.txt)
