import { supabase } from "./supabaseClient.js";
import { sessionState, addEvent } from "./dataStore.js";
import { createScene } from "./viz/Scene.js";
import { ExperienceRing } from "./viz/ExperienceRing.js";

const { scene, camera, renderer } = createScene();
const ring = new ExperienceRing(scene);

// Example: set session ID manually (later via NFC)
sessionState.currentSessionId = "test-session-456";
document.getElementById("sessionLabel").innerText =
  `Session: ${sessionState.currentSessionId}`;

// --- SUPABASE REALTIME ---
supabase
  .channel("pulse-events")
  .on(
    "postgres_changes",
    {
      event: "INSERT",
      schema: "public",
      table: "events",
      filter: `session_id=eq.${sessionState.currentSessionId}`
    },
    payload => {
      addEvent(payload.new);
      ring.addEvent(sessionState.events.at(-1));
    }
  )
  .subscribe();

// --- RENDER LOOP ---
function animate() {
  requestAnimationFrame(animate);
  ring.group.rotation.z += 0.0005; // slow drift
  renderer.render(scene, camera);
}

animate();

// --- TESTING: ADD RANDOM EVENT EVERY 3s --- DELETE LATER!
setInterval(() => {
  ring.addEvent({
    bpm: 60 + Math.random() * 60,
    time: new Date(),
    artworkId: "test"
  });
}, 3000);

