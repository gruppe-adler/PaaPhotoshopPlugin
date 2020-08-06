<p align="center">
  <img src="./AdlerPS.png">
</p>

# Gruppe Adler PAA Photoshop Plugin

PaaPhotoshopPlugin is a Photoshop File Format Plugin for opening and saving PAA images in Photoshop. For more information on the PAA file format check out the  [public PAA file format specification](https://community.bistudio.com/wiki/PAA_File_Format).

# Installation

## Windows
- Download the latest `PaaFormat.8bi` from https://github.com/gruppe-adler/PaaPhotoshopPlugin/releases.
- Move the `PaaFormat.8bi` into `<Photoshop Installation Directory>\Plug-ins\`.

<!---
## MacOS
Move the `PaaFormat.plugin` into `Applications/Adobe Photoshop/Plug-ins/`.
--->

# Features
- `Open`, `Open As` menu commands can be used to read `*.paa` files.
- `Save`, `Save As` menu commands can be used to write `*.paa` files.

# Limitations
- As per the PAA file format specification only images where the height and width is a power of 2 (`2^n`) are supported.
- Only the "RGB Color" image mode is currently support.
- Only the DXT1/DXT5 PAA Formats are currently supported.

# Dependencies
* [Adobe Photoshop SDK](https://console.adobe.io/downloads/ps)
* [grad_aff](https://github.com/gruppe-adler/grad_aff/tree/dev)

# Building
- Download and unpack the latest [Adobe Photoshop SDK](https://console.adobe.io/downloads/ps)
- `git clone` this repository into `adobe_photoshop_sdk_[version]/pluginsdk/samplecode/format`
- Build and install [grad_aff](https://github.com/gruppe-adler/grad_aff)
- Set the environment variables `GRAD_AFF` and `GRAD_AFF_DEBUG` to the directory where you installed grad_aff
- Set the environment variable `PS_DIR` to your Adobe Photoshop installation directory
- Open the `PaaFormat.sln` with Microsoft Visual Studio 2019 or later and build the solution