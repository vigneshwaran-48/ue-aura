# Aura Framework

A lightweight **Lyra-inspired gameplay framework for Unreal Engine** designed for **single-player indie games**.
Aura provides a modular architecture built around **Enhanced Input, Gameplay Ability System (GAS), and data-driven gameplay features** while avoiding the complexity of large sample projects.

The goal of Aura is to give solo developers a **clean, reusable gameplay foundation** that can be easily migrated between projects.

---

# Why Aura?

Unreal’s Gameplay Ability System is extremely powerful but can be difficult to integrate into small projects. The Lyra Starter Game demonstrates best practices, but its architecture can be overwhelming for smaller teams or solo developers.

Aura extracts the **most useful ideas from Lyra** and simplifies them into a compact framework.

Aura focuses on:

* Modular gameplay systems
* Data-driven design
* Reusable architecture
* Clean GAS integration

---

# Core Architecture

Aura is structured as a reusable Unreal Engine module.

```text
Enhanced Input
      ↓
Gameplay Tags
      ↓
Aura Ability System Component
      ↓
Gameplay Abilities
```

Gameplay features such as weapons, equipment, and character abilities are implemented using **Ability Sets and Gameplay Tags**, allowing gameplay logic to remain highly modular.

---

# Features

## GAS Integration

Aura provides a clean integration between **Enhanced Input** and **Gameplay Ability System**.

Input flow:

```text
Input Action
   ↓
Input Config
   ↓
Gameplay Tag
   ↓
AbilitySystemComponent
   ↓
Gameplay Ability Activation
```

Abilities are activated using **input gameplay tags** rather than hardcoded bindings.

---

## Ability Sets

Abilities are granted using **Ability Sets** defined in data assets.

Ability Sets can contain:

* Gameplay Abilities
* Gameplay Effects
* Attribute Sets

Ability Sets make it easy to configure character loadouts without modifying code.

Example:

```text
PlayerAbilitySet
 ├ Jump
 ├ Dodge
 └ Interact

SwordAbilitySet
 ├ LightAttack
 └ Block
```

---

## Ability Set Handles

Ability Sets return **handles** when granted.

Handles allow safe removal of abilities later.

Example usage:

```text
AbilitySet->GiveToAbilitySystem(ASC, Handles);
Handles.TakeFromAbilitySystem(ASC);
```

This allows dynamic gameplay features such as:

* weapon abilities
* temporary buffs
* equipment bonuses
* character class abilities

---

## Equipment System

Aura includes a modular **equipment system inspired by Lyra**.

The system separates equipment into three layers:

```text
EquipmentDefinition
EquipmentInstance
EquipmentActor
```

### EquipmentDefinition

Data asset describing an item.

Example:

```text
SwordDefinition
 ├ SlotTag
 ├ AbilitySet
 ├ ActorClass
 └ AttachSocket
```

### EquipmentInstance

Runtime representation of an equipped item.

Responsible for:

* granting ability sets
* storing ability handles
* spawning equipment actors

### EquipmentActor

The visual representation of the equipment in the world.

Examples:

* weapon meshes
* armor meshes
* cosmetic items

---

## Equipment Slots

Characters use **slot-based equipment**.

Slots are defined using **Gameplay Tags**.

Example slot tags:

```text
Equipment.Slot.Weapon.Primary
Equipment.Slot.Weapon.Secondary
Equipment.Slot.Armor.Head
Equipment.Slot.Armor.Chest
Equipment.Slot.Cosmetic.Hat
```

Each equipment item declares which slot it occupies.

When equipping a new item:

1. The system checks the slot
2. Removes the currently equipped item
3. Spawns the new equipment instance
4. Grants its ability set

---

# Module Structure

```text
Aura
 ├ AbilitySystem
 │   ├ AuraAbilitySystemComponent
 │   ├ AuraAbilitySet
 │   └ AbilitySetHandles
 │
 ├ Input
 │   ├ AuraEnhancedInputComponent
 │   └ AuraInputConfig
 │
 ├ Character
 │   └ AuraCharacter
 │
 └ Equipment
     ├ AuraEquipmentDefinition
     ├ AuraEquipmentInstance
     └ AuraEquipmentManagerComponent
```

---

# Quick Start

This section explains how to integrate the **Aura module** into a new Unreal Engine project and quickly set up a playable character using the framework.

---

## 1. Add the Aura Module

Copy the **Aura module** into your project's `Source` directory.

Project structure example:

```text
YourProject
 ├ Source
 │   ├ Aura
 │   └ YourProject
```

After copying the module, regenerate project files and rebuild the project.

---

## 2. Enable Required Modules

Ensure your project depends on the following Unreal Engine modules:

* `GameplayAbilities`
* `GameplayTags`
* `GameplayTasks`
* `EnhancedInput`

These should already be listed inside `Aura.Build.cs`.

---

## 3. Create an Aura Character

Create a Blueprint based on:

```text
AuraCharacter
```

This character already includes:

* Ability System Component
* Enhanced Input integration
* Equipment Manager
* Ability Set support

Example:

```text
BP_PlayerCharacter → Parent Class: AuraCharacter
```

---

## 4. Create Input Actions

Create input actions using **Enhanced Input**.

Example input actions:

```text
IA_Attack
IA_Jump
```

Add them to an **Input Mapping Context**.

Example:

```text
IMC_Player
```

---

## 5. Create an Aura Input Config

Create a data asset based on:

```text
AuraInputConfig
```

Map input actions to gameplay tags.

Example:

```text
IA_Attack → InputTag.Ability.Attack
IA_Jump   → InputTag.Ability.Jump
```

Assign this Input Config to the character.

---

## 6. Create Gameplay Abilities

Create gameplay abilities for your character.

Example:

```text
GA_Jump
GA_Attack
```

Abilities should respond to input tags defined in the input config.

---

## 7. Create an Ability Set

Create a data asset based on:

```text
AuraAbilitySet
```

Add abilities to the set with the corresponding tags defined in the InputConfig.

Example:

```text
PlayerAbilitySet
 ├ GA_Jump
 └ GA_Attack
```

Assign this ability set to your character so abilities are granted at runtime.

---

## 8. Create Equipment Definitions (Optional)

Create equipment assets based on:

```text
AuraEquipmentDefinition
```

Example sword definition:

```text
DA_Sword
 ├ SlotTag = Equipment.Slot.Weapon.Primary
 ├ AbilitySet = SwordAbilitySet
 ├ ActorClass = BP_SwordActor
 └ AttachSocket = hand_r
```

Equipping the item will:

* spawn the equipment actor
* attach it to the character
* grant its ability set

---

## 9. Equip Equipment at Runtime

Use the equipment manager on the character:

```cpp
EquipmentManager->EquipItem(SwordDefinition);
```

The system will automatically:

1. Check the equipment slot
2. Remove existing equipment in that slot
3. Create a new equipment instance
4. Grant its abilities
5. Spawn and attach the equipment actor

---

## 10. Play

Once the character, input config, and ability sets are configured:

1. Set your Aura character as the **default pawn**
2. Launch the game
3. Abilities will activate automatically when the mapped inputs are triggered.

You now have a working **Aura gameplay setup**.

---

# Current Status

Implemented systems:

* Enhanced Input → GAS integration
* Gameplay tag input system
* Ability Sets
* Ability Set Handles
* Equipment system
* Slot-based equipment management

---

# Planned Features

Future updates may include:

* Interaction system
* Inventory system
* UI integration
* weapon switching system
* save/load support

---

# Design Goals

Aura is designed with the following goals:

* Simple to understand
* Easy to extend
* Fully data-driven
* Reusable across projects
* Minimal boilerplate code

---

# Acknowledgements

Aura is inspired by the architecture of the **Lyra Starter Game** sample project from Unreal Engine.
