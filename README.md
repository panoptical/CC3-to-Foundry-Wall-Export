Campaign Cartographer 3 to Foundry Wall Export

This addon for CC3 will allow you to export a map for use in FoundryVTT.

Currently this is only tested for use with maps on square grids, and it is assumed that walls and rooms were set up with Snap to Grid enabled.
If not, there is no guarantee that walls will line up with grid squares in Foundry.

Currently, this only works with walls created as WALLS in CC3. It won't import doors or terrain (sorry). These features may arrive in the future.
Currently, this only works with the most common wall types: POLY and PATH walls (what you get from drawing standard walls and rooms).
At this time, the program will crash if you try to use it to import walls stored as ARC or other less common wall types. Sorry! I'm working on a fix for that next.

Installation instructions
1. Download the latest release
2. Copy the DLL into your CC3 installation directory (the one with fcw32.exe in it)

Instructions for use:

1. Ensure Snap to Grid is enabled (Tools -> Snaps -> Grid Settings, and tick Grid, Snap, and Cursor Snap)
2. Type command BB (short for Bounding Box, the current method for selecting a section of map to export. This might be changed later to something more intuitive for the user.)
3. Select the Lower Left corner of the bounding box, then the upper right corner. This defines the area which is to be exported.
  - Define the box by grid intersections (multiples of 5'). If you define a box using half-grid points the walls may not line up to the grid in Foundry.
4. Select a name and location for the text file. You have to manually type the .json extension.
5. Select a name and location for the png file. You may have to mantually type the .png extension.

Now you have a corresponding Foundry-compatible json scene file and background image. Import them to Foundry in the usual way:
1. Create a new scene
2. Import the json file first (note: the json file does not contain the background image filename/path, so it will overwrite any background image in the scene)
   - right click on the scene name
   - click Import Data
4. Upload and set the background image
   - right click on the scene
   - click Configure
   - In the Basics tab, click on the icon to the right of the Background Image field
6. Adjust settings as needed.
7. Remember, importing the json file will completely overwrite any changes and reset the whole scene, so only use it on a new/blank scene.




https://github-production-user-asset-6210df.s3.amazonaws.com/1449165/433011980-ae289075-657d-4e75-88d2-adf5c4c9fd99.webm?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20250412%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20250412T054706Z&X-Amz-Expires=300&X-Amz-Signature=e6a6cbaa7e39c473919e1051eae591e6fa5af766be87162923c2766699bfede0&X-Amz-SignedHeaders=host


[Export Crop.webm](https://github.com/user-attachments/assets/3b33e441-e2fe-43ea-b277-c76fb2fc6fea)

FAQ
1. Q. What if I want scene padding?
   A. In theory if you add scene padding Foundry should keep the walls properly aligned to the map.
   There are modules such as Monk's Wall Enhancements that will offer to move things around for you - you shouldn't need these, so just answer "No" when adding padding.

2. Q. What about other VTTs?
   A. I may add compatibility for UniversalVTT at some point down the line. If anyone knows of a converter from Foundry to UniversalVTT, drop me a line and I'll add a link here.

3. Q. This is really helpful? Can I give you money?
   A. This work is completely free, but if you'd like to buy me a coffee, you can do so here: https://ko-fi.com/nealzupancic
   You can also sign up for one of my online RPGs at [StartPlaying.games](https://startplaying.games/gm/ninesides)

Credits

Thanks to [Profantasy](https://www.profantasy.com/) for making CC3 - a lovely mapmaking suite.
Thanks to Remy Monsen for his [tutorials](https://rpgmaps.profantasy.com/xp-tutorials/) on writing addons for CC3.

File output functions for Windows were created with the help of GitHub Copilot.
