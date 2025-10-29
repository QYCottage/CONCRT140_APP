# Mod Loader

## Overview

This project is a **proxy DLL** for the **GDK version of Minecraft**, designed to load **mods**.  
It is implemented using the **WinHTTP** library.

By placing the compiled `winhttp.dll` in the same directory as **`Minecraft.Windows.exe`**,  
it intercepts the game’s loading process and automatically loads all `.dll` mods from the `mods/` folder.

---

## Installation and Usage

### 1. Place the DLL File

Copy the compiled `winhttp.dll` into the same directory as `Minecraft.Windows.exe`.

### 2. One-Click Installation (Recommended)

Run the provided `install.bat` script.  
It will automatically copy the DLL file to the correct path.

### 3. Add Your Mods

Create a folder named `mods` in your Minecraft directory (if it does not already exist),  
and place your `.dll` mod files in that folder.

### 4. Launch the Game

Start the GDK version of Minecraft.  
All `.dll` mod files located in the `mods/` folder will be automatically loaded on startup.

---

## Notes

- Only `.dll` mod files are recognized and loaded.
- Ensure `winhttp.dll` and `Minecraft.Windows.exe` are in the same directory.

---
