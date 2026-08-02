# Changelog

All notable changes to SUP F4SE will be documented in this file.

## [0.77] - 2025-12-22

### Added
- Address Library integration for version-independent addressing
- `SUP_AddressLib.h` - Address Library database loader header
- `SUP_AddressLib.cpp` - Runtime address resolution implementation
- `F4SE_Compat.h` - F4SE compatibility layer
- Support for Fallout 4 Next Generation (v1.10.980+)
- Support for Fallout 4 Anniversary Edition (v1.11.x)
- Compatibility with F4SE 0.7.7

### Changed
- Replaced all hardcoded addresses with Address Library ID lookups
- Updated initialization to load Address Library database at startup
- Modified all function hooks to use dynamically resolved addresses

### Mapped Address Library IDs (35 functions)
- Workshop/Power Grid: 14 functions
- Quest events: 3 functions
- Radio functions: 3 functions
- Actor functions: 5 functions
- Console/Script: 2 functions
- Map/Cell functions: 3 functions
- Combat functions: 3 functions
- Utility functions: 2 functions

### Disabled (addresses not found in NG database)
- Gameloop hook
- OnRemovePlayerMapMarker hook
- Crosshair handler
- Time multiplier access

### Technical Notes
- This update was created with Claude AI assistance via Claude Code CLI
- Approximately 500,000 tokens used across development sessions
- 27,786 lines of code analyzed across 55 source files

## [Previous Versions]

See original mod page: https://www.nexusmods.com/fallout4/mods/17295
