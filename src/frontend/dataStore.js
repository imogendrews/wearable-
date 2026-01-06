export const sessionState = {
  events: [],
  currentSessionId: null
};

export function addEvent(row) {
  sessionState.events.push({
    bpm: row.value?.bpm,
    location: row.value?.location || null,
    artworkId: row.artwork_id,
    time: new Date(row.created_at)
  });
}
