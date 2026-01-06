import * as THREE from "three";

export function bpmToColor(bpm) {
  const t = THREE.MathUtils.clamp((bpm - 60) / 80, 0, 1);
  return new THREE.Color().setHSL(0.6 - t * 0.6, 1, 0.5);
}

export function bpmToRadius(bpm) {
  return THREE.MathUtils.mapLinear(bpm, 60, 140, 2.2, 3.5);
}
