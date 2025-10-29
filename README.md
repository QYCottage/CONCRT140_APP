# Mod Loader

##  Overview

This project is a **proxy DLL** designed for the **GDK version of Minecraft** to load **mods**.  
It is implemented based on the **WinHTTP** library.

Place the compiled `winhttp.dll` in the same directory as **`Minecraft.Windows.exe`**,  
or simply run **`install.bat`** for one-click installation.  

After installation, put your mod `.dll` files into the **`mods`** folder —  
they will be automatically loaded when the game starts.

## ⚙️ Usage

1. **Place the DLL file**  
   Copy the compiled `winhttp.dll` to the same folder as `Minecraft.Windows.exe`.

2. **One-click installation**  
   Run `install.bat` to automatically place the DLL into the correct path.

3. **Add your mods**  
   Create a folder named `mods` in your Minecraft directory (if it doesn’t exist),  
   then put all your mod `.dll` files inside this folder.

4. **Start the game**  
   Launch the GDK version of Minecraft — all mods located in the `mods/` folder  
   will be automatically loaded on startup.

##  Notes

- Mods must be in `.dll` format to be detected and loaded.
