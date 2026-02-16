# MMPlusMods
Various mods for Hatsune Miku: Project DIVA Mega Mix+

## DivaLZMA
Adds support for LZMA compression alongside gzip compression

## OPD Play Gen
Generates OPD files on demand if not present. Also allows files to be cached.
It only generates cache for missing motions or modules. If FARC with OPD is present, and OPD file inside that FARC is present too, it won't do anything and will load existing data.

## OPD Plus
Small patch that allows additional OPD motion sets and motions
Motion sets and motions are specified in `rom/rob/mod_OPDPlus.toml` inside mod folder.

Syntax:
```
mot_set_ids = [ Mot Set ID #1, Mot Set ID #2, ... ]

# Warning! OPD for specific Motion IDs only works if the OPD for **some** Mot Set ID is also loaded at the same time during PV!
mot_ids = [ Motion ID #1, Motion ID #2, ... ]
```

## OpenLyricLimit
Small patch that allows 1000 lines of lyrics

## PotatoMM
Mod for potato PC users

## Sprite Draw Limit
Breaks limit for Sprite Draw. (Prevents displaying of mess on screen if too many sprites are shown)

## SSAA Enable
Name tells it. It enables SSAA in-game. 3D stuff will be rendered at twice the native resolution.
