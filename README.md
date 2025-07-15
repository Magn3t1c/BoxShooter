# 🎯 FPS Box Shooter

*My first Unreal Engine 5 FPS game with a fully functional real-time score system!*

![UE5](https://img.shields.io/badge/Engine-Unreal%205-blue)
![C++](https://img.shields.io/badge/Language-C%2B%2B-orange)
![Blueprint](https://img.shields.io/badge/UI-Blueprint-green)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen)

**Created by:** Magn3t1c  
**Completed:** July 15, 2025  
**Development Time:** Several intense debugging sessions! 😅

## 🚀 Project Overview

This is a first-person shooter game built in Unreal Engine 5 where players shoot destructible boxes to earn points. The game features a real-time UI score system that updates instantly when targets are hit.

## 🎮 How to Play

1. **Movement**: Use `WASD` keys to move around
2. **Look Around**: Use your `Mouse` to look and aim
3. **Jump**: Press `Space` to jump
4. **Shoot**: `Left Click` to fire and hit targets
5. **Score Points**: Hit the boxes to increase your score!

## 🛠️ Technical Implementation

### Game Logic Architecture

The game follows a clean separation of concerns:

- **AFPSBoxShooterCharacter (C++)**: Main player controller handling input, shooting mechanics, and score management
- **ABoxActor (C++)**: Target objects that can be destroyed and award points
- **WBP_ScoreWidget (Blueprint)**: User interface widget for displaying the score
  **ABoxSpawner (C++)**: Handles the JSON and fetches the data to spawn the boxes
- **Enhanced Input System**: Modern UE5 input handling for responsive controls

