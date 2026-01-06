frontend/
├─ index.html
├─ visualization.js
├─ supabaseClient.js
├─ dataStore.js
├─ viz/
│ ├─ Scene.js
│ ├─ ExperienceRing.js
│ └─ PulseMapper.js
└─ style.css

## `index.html`

**What it does:**  
The basic HTML page.

**Why it’s needed:**  
It loads the visualization and provides a place for the Three.js canvas to appear.

---

## `visualization.js`

**What it does:**  
Main control file.

**Why it’s needed:**

- Starts the Three.js scene
- Receives pulse data from Supabase
- Sends the data to the visualization

This is where everything is connected.

---

## `supabaseClient.js`

**What it does:**  
Connects the website to Supabase.

**Why it’s needed:**  
Keeps the database connection separate from the visualization logic.

---

## `dataStore.js`

**What it does:**  
Stores the current visitor’s data in the browser.

**Why it’s needed:**  
Makes it easy to collect, replay, or compare a session later.

---

## `viz/Scene.js`

**What it does:**  
Sets up the Three.js scene, camera, and lighting.

**Why it’s needed:**  
Keeps the basic Three.js setup out of the main logic.

---

## `viz/ExperienceRing.js`

**What it does:**  
Draws the circular visualization of the exhibition.

**Why it’s needed:**  
This file turns time and pulse data into a visual “experience ring”.

---

## `viz/PulseMapper.js`

**What it does:**  
Converts pulse values into colors and sizes.

**Why it’s needed:**  
Defines how calm or intense moments look visually.

---

## `style.css`

**What it does:**  
Styles the page (background, text, layout).
