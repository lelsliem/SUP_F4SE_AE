# SUP F4SE - Future Development TODO

## Priority: Address Update System Refactor

**Target:** January 2025 (after token refresh)

### Goal
Rework the address resolution system to make it easier to update when Bethesda releases new game patches. Create documentation so Claude can efficiently update addresses without extensive reverse-engineering each time.

### Current Pain Points
1. Address Library IDs are hardcoded in `SUP_AddressLib.h`
2. When IDs aren't found, requires manual research to locate new IDs
3. No structured process for mapping old addresses to new Address Library IDs
4. Some addresses couldn't be found in NG database (disabled features)

### Proposed Changes

#### 1. External Address Configuration
- [ ] Move Address Library IDs to external config file (JSON or INI)
- [ ] Allow runtime loading of address mappings
- [ ] Support fallback/default values for missing addresses
- [ ] Log which addresses failed to resolve for easy debugging

#### 2. Address Mapping Documentation
- [ ] Create `ADDRESS_MAPPING.md` with:
  - Table of all functions and their purposes
  - Current Address Library IDs
  - Original hardcoded offsets (for reference)
  - Function signatures for identification
  - Instructions for finding new IDs

#### 3. Claude Update Guide
- [ ] Create `CLAUDE_UPDATE_GUIDE.md` with:
  - Step-by-step process for updating addresses
  - How to use Address Library database tools
  - How to identify functions by signature
  - Testing checklist
  - Common issues and solutions

#### 4. Automation Tools
- [ ] Improve `parse_ng_db.py` for better address searching
- [ ] Create script to validate all addresses resolve correctly
- [ ] Create script to compare addresses between game versions

### Files to Modify
- `SUP_AddressLib.h` - Refactor ID storage
- `SUP_AddressLib.cpp` - Add config file loading
- `SUP_F4SE.cpp` - Update initialization

### Files to Create
- `addresses.json` or `addresses.ini` - External address config
- `ADDRESS_MAPPING.md` - Address documentation
- `CLAUDE_UPDATE_GUIDE.md` - Instructions for AI-assisted updates
- `validate_addresses.py` - Address validation script

---

## Other Future Improvements

### Re-enable Disabled Features
- [ ] Find Address Library ID for `Main_RadioUpdate`
- [ ] Find Address Library ID for `Main_OnRemovePlayerMapMarker`
- [ ] Find Address Library ID for `Address_TimeMultiplier`
- [ ] Fix Crosshair handler for NG

### Code Quality
- [ ] Add more detailed logging for troubleshooting
- [ ] Consider splitting large header files
- [ ] Add version detection for multiple game versions

---

## Notes

**Created:** December 22, 2025
**Last Updated:** December 22, 2025
**Session Stats:** ~500,000 tokens, ~8-10 hours development time

This update was created with Claude (Anthropic AI) via Claude Code CLI.
The refactor will make future updates much faster and more reliable.
