import * as THREE from "three";
import { bpmToColor, bpmToRadius } from "./PulseMapper.js";

export class ExperienceRing {
  constructor(scene) {
    this.scene = scene;
    this.group = new THREE.Group();
    scene.add(this.group);

    this.startTime = null;
  }

  addEvent(event) {
    if (!this.startTime) this.startTime = event.time;

    const elapsed =
      (event.time - this.startTime) / 1000; // seconds

    const angle = elapsed * 0.15; // time → angle mapping
    const radius = bpmToRadius(event.bpm);

    const geometry = new THREE.CircleGeometry(0.06, 16);
    const material = new THREE.MeshBasicMaterial({
      color: bpmToColor(event.bpm),
      transparent: true,
      opacity: 0.85
    });

    const dot = new THREE.Mesh(geometry, material);

    dot.position.set(
      Math.cos(angle) * radius,
      Math.sin(angle) * radius,
      0
    );

    this.group.add(dot);
  }
}
