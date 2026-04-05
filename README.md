# Aura Framework (Unreal Engine Plugin)

**Aura Framework** is a modular Unreal Engine plugin that provides a reusable, data-driven gameplay architecture inspired by modern UE systems like Lyra.

It focuses on clean system design using Gameplay Tags, the Gameplay Ability System (GAS), and extensible animation/UI layers — with the goal of being **plug-and-play across projects**.

---

## Key Features

### Ability System Integration

* Built on Unreal’s Gameplay Ability System (GAS)
* Uses **Ability Sets + Handles pattern** for clean grant/remove workflows
* Tag-driven ability activation

---

### Input → Tag → System Flow

* Uses Enhanced Input
* Inputs are mapped to **Gameplay Tags**
* Tags drive:

  * Ability activation
  * Gameplay logic

> Clean decoupling:
> `Input → Gameplay Tag → System Response`

---

### Equipment System

* Slot-based system driven by Gameplay Tags
* Designed to integrate seamlessly with other systems like animation overlays

---

### Animation Overlay System

A **data-driven layered animation system** designed for flexibility and reuse.

#### Supports:

* **Base Overlay** - YET TO BE IMPLEMENTED
* **Status Overlay** - YET TO BE IMPLEMENTED
* **Equipment Overlays (multiple)** - IMPLEMENTED

#### Core Concepts:

* Fixed set of **overlay slots (Gameplay Tags)**
* Fixed number of **blend mask regions**
* Each slot maps to a region

#### How it works:

* Plugin defines:

  * Slot tags
  * Overlay structure
* Child Animation Blueprints provide:

  * Actual **blend mask names**
  * Character-specific implementation

> This keeps the system generic while allowing project-specific customization.

---

### UI Framework Integration

* Built on:

  * CommonUI
  * GameUI
  * CommonGame

Includes:

* UI Manager Subsystem
* Integration with gameplay systems
* Configurable via `UDeveloperSettings`

---

### Messaging & UI Extensions

* Uses:

  * GameplayMessageSubsystem
  * UIExtension-style patterns

Enables:

* Event-driven UI updates
* Decoupled communication between gameplay and UI

---

## Design Philosophy

* **Plug-and-play** across projects
* **Data-driven over hardcoded logic**
* **Gameplay Tags as the backbone**
* Shared abstractions across systems

---

## ⚠️ Notes

* The framework depends on several Lyra-related extensions:

  * CommonUI
  * CommonGame
  * ModularGameplay
  * ModularGameplayActors
  * GameplayMessageRuntime

* These extensions can be used directly by placing them in the project’s Plugins folder. No modification to Lyra plugins is required.

---

## Summary

Aura Framework provides a unified approach where:

* Abilities, input, UI, and animation systems are connected through Gameplay Tags
* Systems remain modular and reusable
* Animation layering is predictable and extensible

---
