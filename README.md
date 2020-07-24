# Gruppe Adler PAA Photoshop Plugin

<p align="center">
  <img src="./AdlerPS.png">
</p>

PaaPhotoshopPlugin is a Photoshop File Format Plugin for opening and saving PAA images in Photoshop. For more information on the PAA file format check out the  [public PAA file format specification](https://community.bistudio.com/wiki/PAA_File_Format).

# Installation

## Windows
Move the `PaaFormat.8bi` into `<Photoshop Installation Directory>\Plug-ins\`.

## MacOS
Move the `PaaFormat.plugin` into `Applications/Adobe Photoshop/Plug-ins/`.

# Features
- `Open`, `Open As` menu commands can be used to read `*.paa` files.
- `Save`, `Save As` menu commands can be used to write `*.paa` files.

# Limitations
- As per the PAA file format specification only images where the height and width is a power of 2 (`2^n`) are supported.
- Only the "RGB Color" image mode is currently support.
- Only the DXT1/DXT5 PAA Formats are currently supported.
