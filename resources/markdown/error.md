## Error Q&A

Here you can find common issues that might cause problems when running the game:

### Problem: The game launches, but no images are displayed / the screen appears transparent, and only the music can be heard.  
**Solution:** This occurs in some Linux versions because the game automatically pauses when the window is minimized or loses focus when executed. Press the **P** key to resume the game normally.

---

### Problem: The game freezes unexpectedly.  
**Solution:**  
- **If this happens at the end of a scene:** The program might be taking extra time to clean up assets. Please wait for a moment.  
- **If this happens in a menu or during gameplay:** It's possible the window lost focus (e.g., by clicking outside the window), which causes the game to pause automatically. Press the **P** key to resume.

---

### Problem: Some sprites or tilesets are missing / appear invisible or incorrect.  
**Solution:** On certain Linux versions, the system might enforce stricter checks for file names, such as case sensitivity and encoding. While this isn't an issue on Windows (where files are accessed without these checks), it might cause missing assets on Linux. This problem should not occur if your assets names have been standardized.

---

### Problem: The UI appears behind other sprites, behind the background, or sprites are displayed in the wrong order.  
**Solution:** In rare cases, when returning to a previously loaded scene, the system may load assets from the cache, causing sprites to appear in the wrong order (e.g., the character you made might be obscured by a sprite that shouldn't overlap). This is an unusual error and depends on how you manage scenes, but restarting the game should resolve it by clearing the cache.